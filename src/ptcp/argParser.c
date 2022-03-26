#include <string.h>
#include <stdlib.h>
#include "ptcp.h"
#include "argDefs.h"

void getCharArgs(Args* args, char* str){
  for(int i = 1; i < strlen(str); i++)
  {
    switch(str[i])
    {
      case RECURSIVE_ARG:
        args->recursive = true;
        break;
      case INTERACTIVE_ARG:
        args->interactive = true;
        break;
    }
  }
}

void getWordArg(Args *args, char* str)
{
  if(strcmp(str, RECURSIVE_ARG_W) == 0) args->recursive = true;
  else if(strcmp(str, INTERACTIVE_ARG_W) == 0) args->interactive = true;
  else if(strcmp(str, HELP_ARG_W) == 0) args->help = true;
}

Args *parseArgs(int argc, char **argv)
{
  Args *args = (Args*)calloc(1, sizeof(Args));
  args->sourcePath = calloc(1, sizeof(char*));

  int pathsCount = 0;

  for(int i = 1; i < argc; i++)
  {
    if(argv[i][0] == '-'){
      if(argv[i][1] == '-') getWordArg(args, argv[i]);
      else getCharArgs(args, argv[i]);
    }
    else pathsCount++;
  }
  
  if(pathsCount < 2) return args;

  for(int i = 1; i < argc; i++)
  {
    if(argv[i][0] != '-')
    {
      if(args->sourcePathCount != pathsCount-1)
      {
        args->sourcePath = realloc(args->sourcePath, (args->sourcePathCount+1)*sizeof(char*));
        args->sourcePath[args->sourcePathCount] = strdup(argv[i]);
        args->sourcePathCount++;
      }
      else 
        args->destPath = strdup(argv[i]);
    }
  }

  return args;
}

void freeArgs(Args *args)
{
  for(int i = 0; i < args->sourcePathCount; i++)
    free(args->sourcePath[i]);
  free(args->sourcePath);
  free(args->destPath);
  free(args);
}
