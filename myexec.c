#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[], char* envp[]) {
	pid_t pid = fork();
	int childstatus;

	if (pid == 0) {
		// child process
		printf("Starting %s.\n", argv[1]);
		printf("####################################################################################################\n");
		execv(argv[1], argv+1);
	} else if (pid > 0) {
		// parent process
		printf("Parent Pid: %d\n", getpid());
		printf("Child Pid: %d\n", pid);
		printf("Finished pid: %d\n", wait(&childstatus));
		printf("Child status: %d\n", childstatus);
	} else {
		// fork failed
		printf("fork() failed!\n");
		return 1;
	}

	return 0;
}
