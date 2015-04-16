#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
	if(argc < 2) {
		printf("ERROR: Too few arguments\n");
		return -1;
	}
	if(argc > 3) {
		printf("ERROR: Too many arguments\n");
		return -1;
	}
	char* name = NULL;
	int showResult = 0;
	if(argc == 3) {
		if(strcmp(argv[1], "-v")) {
			name = argv[2];
		} else if(strcmp(argv[2], "-v")) {
			name = argv[1];
		} else {
			printf("ERROR: Wrong arguments\n");
			return -1;
		}
		showResult = 1;
	} else {
		name = argv[1];
	}
	char* result = getenv(argv[1]);
	if(showResult && result != NULL) {
		printf("%s\n", result);
	}
	return result == NULL ? 1 : 0;
}
