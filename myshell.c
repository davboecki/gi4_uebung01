#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>
#include <unistd.h>

#define DEBUG 0

char* PATH;

int startSub(char* name, char* argv[]) {
	pid_t pid = fork();
	int childstatus;

	if (pid == 0) {
		// child process
		if(DEBUG) {
			printf("Starting %s.\n", name);
			printf("############################################\n");
		}
		execv(name, argv);
	} else if (pid > 0) {
		// parent process
		int result = wait(&childstatus);
		if(DEBUG) {
			printf("############################################\n");
			printf("Parent Pid: %d\n", getpid());
			printf("Child Pid: %d\n", pid);
			printf("Finished pid: %d\n", result);
			printf("Child status: %d\n", childstatus);
		}
	} else {
		// fork failed
		printf("fork() failed!\n");
		return 1;
	}
	return 0;
}

char** parse_input(char* line) {
	int count = 0;
	int args = 2;
	while (line[count] != '\0') {
		count++;
		if(line[count] == ' ') {
			args++;
		}
	}
	char** result = malloc(args * sizeof(char*));
	int i;
	int j = 0;
	result[0] = line;
	for (i = 0; i < count - 1; i++) {
		char c = line[i];
		if (c == ' ') {
			line[i] = '\0';
			result[++j] = line + i + 1;
		}
	}
	result[++j] = NULL;
	return result;
}

void help() {
	printf("myshell -----------\n");
	printf("   - help - This screen\n");
	printf("   - setpath - Set the PATH variable\n");
	printf("   - resetpath - Reset the PATH variable\n");
	printf("   - showpath - Show the PATH variable\n");
	printf("   - time - show the current time\n");
}

void showCurrentTime() {
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("Current local time and date: %s", asctime(timeinfo));
}

char* searchPathForProgramm(char* name) {
	char* localPath = strdup(PATH);
	char* orig = localPath;
	while (localPath != NULL) {
		char* dir = strsep(&localPath, ":");
		char* file = malloc((strlen(dir) + strlen(name) + 2) * sizeof(char));

		strcpy(file, dir);
		strcat(file, "/");
		strcat(file, name);
		if (access(file, F_OK) == 0) {
			free(orig);
			return file;
		}
		free(file);
	}
	free(orig);
	return strdup(name);
}

void handlePath(char* args[]) {
	if(args[0] == NULL || args[1] != NULL) {
		printf("ERROR: Only one argument allowed!\n");
		return;
	}
	char* newPath = args[0];
	PATH = newPath;
}

void resetPath(char* args[]) {
	PATH = getenv("PATH");
	printf("PATH reset\n");
}

void showPath(char* args[]) {
	printf("PATH: %s\n", PATH);
}

char* getWorkingDir() {
	char cwd[4096];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		char* result = malloc(strlen(cwd));
		strcpy(result, cwd);
		return result;
	}
	return NULL;
}

void cd(char* args[]) {
	if(args[0] == NULL || args[1] != NULL) {
		printf("ERROR: Only one argument allowed!\n");
		return;
	}
	char* dir = args[0];
	if(strcmp(dir, ".") == 0) {
		return;
	} else if(strcmp(dir, "..") == 0) {
		char* path = getWorkingDir();
		char* last = strrchr(path, '/');
		if(last != NULL) {
			*last = '\0';
			chdir(path);
		}
		free(path);
	} else {
		char* path = getWorkingDir();
		char* newPath = malloc(strlen(path) + strlen(dir) + 1);
		strcpy(newPath, path);
		strcat(newPath, "/");
		strcat(newPath, dir);
		struct stat info;
		if (stat(newPath, &info) != 0) {
			printf("%s doesn't exsist\n", dir);
		} else if (info.st_mode & S_IFDIR) {
			chdir(newPath);
		} else {
			printf("%s is not a directory\n", dir);
		}
		free(newPath);
		free(path);
	}
}

void handleSubCall(char* argv[]) {
	char* filepath = searchPathForProgramm(argv[0]);
	if(strcmp(filepath, argv[0]) == 0 && access(filepath, F_OK) != 0) {
		printf("Command not found.\n");
		return;
	}
	startSub(filepath, argv);
}

int knownProgramms(char* myOwn, char* args[]) {
	if(strcmp(myOwn, "help") == 0) {
		help();
		return 1;
	} else if(strcmp(myOwn, "time") == 0) {
		showCurrentTime();
		return 1;
	} else if(strcmp(myOwn, "setpath") == 0) {
		handlePath(args);
		return 1;
	} else if(strcmp(myOwn, "resetpath") == 0) {
		resetPath(args);
		return 1;
	} else if(strcmp(myOwn, "showpath") == 0) {
		showPath(args);
		return 1;
	} else if(strcmp(myOwn, "cd") == 0) {
		char* dir = getWorkingDir();
		if(dir != NULL) {
			cd(args);
			free(dir);
		} else {
			printf("ERROR: Couldn't change directory.\n");
		}
		return 1;
	} else if(strcmp(myOwn, "mypwd") == 0) {
		char* dir = getWorkingDir();
		if(dir != NULL) {
			printf("PWD: %s\n", dir);
			free(dir);
		} else {
			printf("ERROR: Couldn't change directory.\n");
		}
		return 1;
	} else if(strcmp(myOwn, "ll") == 0) {
		char* part = malloc(5);
		strcpy(part, "ls -l");
		handleSubCall(parse_input(part));
		free(part);
		return 1;
	} else {
		return 0;
	}
}

int main(int argc, char* argv[], char* envp[]) {
	add_history("exit");
	PATH = getenv("PATH");
	handleSubCall(parse_input("clear"));
	while (1) {
		char* message = "myshell$ ";
		char* path = getWorkingDir();
		if(path != NULL) {
			message = malloc(11 + strlen(path));
			message[0] = '\0';
			strcat(message, "myshell:\n");
			strcat(message, path);
			strcat(message, "$ ");
		}
		char* input = readline(message);
		if (input == NULL) {
			continue;
		}
		add_history(input);
		char** args = parse_input(input);
		if(DEBUG) {
			int i = 0;
			while(args[i] != NULL) {
				printf("%s ", args[i++]);
			}
			printf("\n");
		}
		if (strcmp(args[0], "exit") == 0) {
			return 0;
		}
		if(!knownProgramms(args[0], args + 1)) {
			handleSubCall(args);
		}
	}
	return 0;
}

