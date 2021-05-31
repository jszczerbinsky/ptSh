#include <stdio.h>
#include <stdlib.h>

void setProgressBar(int size, int percentage)
{
  if(percentage > 100) percentage = 100;
  size -= 2;
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
  printf("\x1b[40m]\x1b[0m");
  fflush(stdout);

  free(percentageStr);

}
