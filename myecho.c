#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) 
{
  int i;
  for(i=1;i < argc;i++)
  {
	  if(i != 1) printf(" ");
	  printf(argv[i]);
  }
  printf("\n");
  return 0;
}
