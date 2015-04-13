#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) 
{
  int i = 0;
  while(envp[i] != NULL)
  {
	  printf("%s\n", envp[i++]);
  }
  printf("Found %d environment variables\n", i);
  return 0;
}
