#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "configParser.h"

int printSize(char *buff, int buffSize, unsigned long size, bool decimalSize)
{
  int precission = 1;

  float sizef = size;
  int divider = (decimalSize) ? 1000 : 1024;
  char *unit = "B";

  if(sizef > divider)
  {
    sizef /= divider;
    unit = (decimalSize) ? "KB" : "KiB";
  } else precission = -1;
  if(sizef > divider)
  {
    sizef /= divider;
    unit = (decimalSize) ? "MB" : "MiB";
  } 
  if(sizef > divider)
  {
    sizef /= divider;
    unit = (decimalSize) ? "GB" : "GiB";
  } 
  
  int digits = precission + 2; 
  if(sizef == 0) digits++;
  else digits += log10(sizef)+1;

  char sizeValStr[25];
  snprintf(sizeValStr, digits, "%f", sizef);

  snprintf(buff, buffSize, "%s %s", sizeValStr, unit);

  return digits+1+strlen(unit);
}

void printMessage(const PtShConfig *config, const char *str, bool err)
{
  const char *pec = err ? config->errorPrefixEscapeCodes : config->successPrefixEscapeCodes;
  const char *ep = err ? config->errorPrefix : config->successPrefix;
  const char *emec = err ? config->errorMessageEscapeCodes : config->successMessageEscapeCodes;

  if(pec) printf("%s", pec);
  if(ep) printf("%s", ep);
  printf("\x1B[0m");

  if(emec) printf("%s", emec);
  printf("%s", str);
  printf("\x1B[0m\n");
}

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
