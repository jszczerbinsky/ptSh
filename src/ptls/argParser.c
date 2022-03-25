#include <string.h>
#include "ptls.h"
#include "argDefs.h"

void getCharArgs(Args* args, char* str){
  for(int i = 1; i < strlen(str); i++)
  {
    switch(str[i])
    {
      case ALL_ARG:
        args->all = true;
        break;
      case ALMOST_ALL_ARG:
        args->almostAll = true;
        break;
      case IGNORE_BACKUPS_ARG:
        args->ignoreBackups = true;
        break;
       case L_ARG:
        args->l = true;
        break;
      case INODE_ARG:
        args->inode = true;
        break;
      case NO_GROUP_ARG:
        args->noGroup = true;
        break;
      case TIME_ARG:
        args->time = true;
        break;
      case FULL_TIME_ARG:
        args->fullTime = true;
        break;
      case FULL_SIZE_ARG:
        args->fullSize = true;
        break;
      case DECIMAL_SIZE_ARG:
        args->decimalSize = true;
        break;
      case NUMERIC_UID_GID_ARG:
        args->numericUidGid = true;
        break;
      case REVERSE_ARG:
        args->reverse = true;
        break;
      case S_X_ARG:
        args->sExtension = true;
        break;
      case S_TIME_ARG:
        args->sTime = true;
        break;
    } 
  }
}

void getWordArg(Args *args, char* str)
{
  if(strcmp(str, HELP_ARG_W) == 0) args->help = true;
  else if(strcmp(str, ALL_ARG_W) == 0) args->all = true;
  else if(strcmp(str, ALMOST_ALL_ARG_W) == 0) args->almostAll = true;
  else if(strcmp(str, IGNORE_BACKUPS_ARG_W) == 0) args->ignoreBackups = true;
  else if(strcmp(str, INODE_ARG_W) == 0) args->inode = true;
  else if(strcmp(str, NO_GROUP_ARG_W) == 0) args->noGroup = true;
  else if(strcmp(str, FULL_TIME_ARG_W) == 0) args->fullTime = true;
  else if(strcmp(str, FULL_SIZE_ARG_W) == 0) args->fullSize = true;
  else if(strcmp(str, DECIMAL_SIZE_ARG_W) == 0) args->decimalSize = true;
  else if(strcmp(str, NUMERIC_UID_GID_ARG_W) == 0) args-> numericUidGid = true;
  else if(strcmp(str, NO_DIRS_TOP_ARG_W) == 0) args->noDirsTop = true;
  else if(strcmp(str, REVERSE_ARG_W) == 0) args->reverse = true;
}

Args* parseArgs(int argc, char** argv)
{
  Args *args = (Args*)calloc(1, sizeof(Args));
  args->dirPath=".";
  
  if(argc == 1) return args;

  for(int i = 1; i < argc; i++)
  {
    if(argv[i][0] == '-'){
      if(argv[i][1] == '-') getWordArg(args, argv[i]);
      else getCharArgs(args, argv[i]);
    }
    else{
      args->dirPath = argv[i];
    }
  }

  return args; 
}
