#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argDefs.h"

void printHelpLine(const char arg, const char* argw, const char* desc)
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

void displayHelp()
{
  printf("Syntax: ptmove [ARGS] [SOURCE] [DEST]\n");
  printf("\nArguments:\n");
  printHelpLine(HELP_ARG, HELP_ARG_W, HELP_ARG_DESC);
  printHelpLine(RECURSIVE_ARG, RECURSIVE_ARG_W, RECURSIVE_ARG_DESC);
  printHelpLine(INTERACTIVE_ARG, INTERACTIVE_ARG_W, INTERACTIVE_ARG_DESC);
  printHelpLine(UPDATE_ARG, UPDATE_ARG_W, UPDATE_ARG_DESC);
}

void setProgressBar(int size, int percentage)
{
  if(percentage > 100) percentage = 100;
  size -= 3;
  int blocks = percentage*size/100;

  char *percentageStr = calloc(5, sizeof(char));
  sprintf(percentageStr, "%d%%", percentage);

  int strPos = size/2-2;

  printf("\b\r");
  printf("\x1b[40m[\x1b[0m\x1b[45m");
  for(int x = 0; x < size; x++)
  {
    if(x == blocks)
      printf("\x1b[0m");

    if(x >= strPos && x < strPos+5 && percentageStr[x-strPos] != '\0')
      printf("%c", percentageStr[x-strPos]);
    else
      printf(" ");
  }
  printf("\x1b[40m]\x1b[0m ");
  fflush(stdout);

  free(percentageStr);

}
