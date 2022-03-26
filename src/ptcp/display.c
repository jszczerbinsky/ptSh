#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ptcp.h"
#include "argDefs.h"

void displayHelp()
{
  printf("Syntax: ptcp [ARGS] [SOURCE] [DEST]\n");
  printf("\nArguments:\n");
  printHelpLine(HELP_ARG, HELP_ARG_W, HELP_ARG_DESC);
  printHelpLine(RECURSIVE_ARG, RECURSIVE_ARG_W, RECURSIVE_ARG_DESC);
  printHelpLine(DECIMAL_SIZE_ARG, DECIMAL_SIZE_ARG_W, DECIMAL_SIZE_ARG_DESC);
  printHelpLine(INTERACTIVE_ARG, INTERACTIVE_ARG_W, INTERACTIVE_ARG_DESC);
}

void setProgressBar(const PtShConfig *config, int size, int percentage)
{
  if(percentage > 100) percentage = 100;
  size -= 3;
  int blocks = percentage*size/100;

  char *percentageStr = calloc(5, sizeof(char));
  sprintf(percentageStr, "%d%%", percentage);

  int strPos = size/2-2;

  printf("\b\r");

  if(config->cpProgressBarBorderEscapeCodes)
    printf("%s", config->cpProgressBarBorderEscapeCodes);

  printf("[\x1b[0m");

  if(config->cpProgressBarEscapeCodes)
    printf("%s", config->cpProgressBarEscapeCodes);

  for(int x = 0; x < size; x++)
  {
    if(x == blocks)
      printf("\x1b[0m");

    if(x >= strPos && x < strPos+5 && percentageStr[x-strPos] != '\0')
      printf("%c", percentageStr[x-strPos]);
    else
      printf(" ");
  }

  if(config->cpProgressBarBorderEscapeCodes)
    printf("%s", config->cpProgressBarBorderEscapeCodes);

  printf("]\x1b[0m ");

  fflush(stdout);

  free(percentageStr);

}
