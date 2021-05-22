#include <linux/limits.h>
#include <ptsh.h>
#include <stdio.h>
#include <string.h>

#include "ptpwd.h"

void displayHelp()
{

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

void display(char *path, PtShConfig *config)
{
  PathDirs *dirs = getDirs(path);

  for(int i = 0; i < dirs->count; i++)
    printf("%s\n", dirs->names[i]);

  freeDirs(dirs);
}

int main(int argc, char **argv)
{
  Args *args = parseArgs(argc, argv);

  if(args->help) displayHelp();

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
