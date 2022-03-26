#include <string.h>
#include <stdio.h>

void printHelpLine(char arg, const char* argw, const char* desc)
{
  const int argwColSize = 20;
  printf("     ");

  if(arg == ' ')
    printf("   ");
  else
    printf("-%c ", arg);

  printf(" ");

  printf("%s  ", argw);
  for(int i = 0; i < argwColSize - strlen(argw); i++) printf(" ");
  printf("%s\n", desc);
}


