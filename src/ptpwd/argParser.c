#include <string.h>
#include "ptpwd.h"
#include "argDefs.h"

void getCharArgs(Args* args, char* str){
  for(int i = 1; i < strlen(str); i++)
  {
    if(str[i] == LOGICAL_PATH_ARG)
      args->logical = true;
  }
}

void getWordArg(Args *args, char* str)
{
  if(strcmp(str, LOGICAL_PATH_ARG_W) == 0) args->logical = true;
  else if(strcmp(str, HELP_ARG_W) == 0) args->help = true;
}

Args *parseArgs(int argc, char **argv)
{
  Args *args = (Args*)calloc(1, sizeof(Args));

  if(argc == 1) return args;

  for(int i = 1; i < argc; i++)
  {
    if(argv[i][0] == '-'){
      if(argv[i][1] == '-') getWordArg(args, argv[i]);
      else getCharArgs(args, argv[i]);
    }
  }

  return args;
}
