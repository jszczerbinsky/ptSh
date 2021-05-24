#include <string.h>
#include <stdlib.h>
#include "ptmove.h"
#include "argDefs.h"

void getCharArgs(Args* args, char* str){
  for(int i = 1; i < strlen(str); i++)
  {
    switch(str[i])
    {
      case COPY_ARG:
        args->copy = true;
        break; 
      case RECURSIVE_ARG:
        args->recursive = true;
        break;
      case INTERACTIVE_ARG:
        args->interactive = true;
        break;
      case UPDATE_ARG:
        args->update = true;
        break;
    }
  }
}

void getWordArg(Args *args, char* str)
{
  if(strcmp(str, COPY_ARG_W) == 0) args->copy = true;
  else if(strcmp(str, RECURSIVE_ARG_W) == 0) args->recursive = true;
  else if(strcmp(str, INTERACTIVE_ARG_W) == 0) args->interactive = true;
  else if(strcmp(str, UPDATE_ARG_W) == 0) args->update = true;
}

Args *parseArgs(int argc, char **argv)
{
  Args *args = (Args*)calloc(1, sizeof(Args));

  if(argc == 1) return args;

  bool sourcePath = false;

  for(int i = 1; i < argc; i++)
  {
    if(argv[i][0] == '-'){
      if(argv[i][1] == '-') getWordArg(args, argv[i]);
      else getCharArgs(args, argv[i]);
    }
    else
    {
      if(sourcePath)
      {
        args->sourcePath = argv[i];
      } else args->destPath = argv[i];
    }
  }

  return args;

}
