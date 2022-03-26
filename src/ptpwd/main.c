#include <linux/limits.h>
#include "../common/configParser.h"
#include "../common/display.h"
#include <stdio.h>
#include <string.h>

#include "argDefs.h"
#include "ptpwd.h"

void displayHelp()
{
  printf("Syntax: ptpwd [ARGS]\n");
  printf("\nArguments:\n");
  printHelpLine(LOGICAL_PATH_ARG, LOGICAL_PATH_ARG_W, LOGICAL_PATH_ARG_DESC);
}

PathDirs *getDirs(char *path)
{
  PathDirs *dirs = malloc(sizeof(PathDirs));

  for(int i = 0; i < strlen(path); i++)
    if(path[i] == '/') dirs->count++;

  dirs->names = calloc(dirs->count, sizeof(char*));

  int actualChar = 0;
  for(int i = 0; i < dirs->count; i++)
  {
    int length = 0;
    for(int j = actualChar+1; j < strlen(path); j++)
        if(path[j] == '/')
          break;
        else length++;

    dirs->names[i] = calloc(length+1, sizeof(char));
    strncpy(dirs->names[i], (path+actualChar+1), length);
    actualChar += length+1;
  }

  return dirs;
}

void freeDirs(PathDirs *dirs)
{
  for(int i = 0; i < dirs->count; i++)
  {
    free(dirs->names[i]);
  }
  free(dirs->names);
  free(dirs);
}

void displayDir(PtShConfig *config, char *name, FileConfigValues *fcv)
{
  if(getValueInt(config->pwdShowDirPrefix))
    printf("%s%s\x1b[0m%s", fcv->prefixEscapeCodes, fcv->prefix, fcv->nameEscapeCodes);

  printf("%s\x1b[0m", name);

  if(getValueInt(config->pwdNextline))
    printf("\n");

}

void display(char *path, PtShConfig *config)
{
  PathDirs *dirs = getDirs(path);
  FileConfigValues *fcv = getFileConfigValues(config, FT_Directory);

  displayDir(config, "/", fcv);

  for(int i = 0; i < dirs->count; i++)
  {
    if(getValueInt(config->pwdNextline))
    {
      int margin = getValueInt(config->pwdNextlineMargin);
      for(int x = 0; x < margin*i; x++)
        printf(" ");
    }
    printf("%s%s\x1b[0m", getValueStr(config->pwdDirSeparatorEscapeCodes), getValueStr(config->pwdDirSeparator));
    displayDir(config, dirs->names[i], fcv);
  }

  if(!getValueInt(config->pwdNextline)) printf("\n");

  free(fcv);
  freeDirs(dirs);
}

int main(int argc, char **argv)
{
  Args *args = parseArgs(argc, argv);

  if(args->help)
  {
    displayHelp();
    return 0;
  }

  char *pwd = getenv("PWD");
  
  if(!args->logical)
  {
    char *tmp = calloc(PATH_MAX, sizeof(char));
    realpath(pwd, tmp);
    pwd = tmp;
  }

  PtShConfig *config = readConfig();

  display(pwd, config);

  closeConfig(config);
  if(!args->logical) free(pwd);
  free(args);

  return 0;
}
