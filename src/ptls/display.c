#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include "ptls.h"

void printPermissions(FileInstance *file, Args *args)
{
  short noGroupDecrease = 0;

  if(args->noGroup)
    noGroupDecrease = 4;

  char permStr[12] = "";
  __mode_t mode = file->stats->st_mode;

  permStr[0] = (mode & S_IRUSR) ?  'r' : '-';
  permStr[1] = (mode & S_IWUSR) ?  'w' : '-';
  permStr[2] = (mode & S_IXUSR) ?  'x' : '-';
  if(!args->noGroup)
  {
    permStr[3] = ' ';
    permStr[4] = (mode & S_IRGRP) ?  'r' : '-';
    permStr[5] = (mode & S_IWGRP) ?  'w' : '-';
    permStr[6] = (mode & S_IXGRP) ?  'x' : '-';
  }
  permStr[7-noGroupDecrease] = ' ';
  permStr[8-noGroupDecrease] = (mode & S_IROTH) ?  'r' : '-';
  permStr[9-noGroupDecrease] = (mode & S_IWOTH) ?  'w' : '-';
  permStr[10-noGroupDecrease] = (mode & S_IXOTH) ?  'x' : '-';
  
  printf("%s", &permStr[0]); 
}

void printUidGid(FileInstance *file, Args *args)
{
  struct passwd *user = getpwuid(file->stats->st_uid);
  printf(" %s", user->pw_name);

  if(!args->noGroup)
  {
    struct passwd *group = getpwuid(file->stats->st_gid);
    printf(" %s", group->pw_name);
  }
}

void displayBlock(FileInstance **files, int count, Args* args, PtShConfig *config, int longestName, int* actualColumn, int* actualChar)
{
  if(longestName == 0) return;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  int columns = w.ws_col/(longestName+1);

  for(int i = 0; i < count; i++)
  {
    char* prefix = getPrefix(config, files[i]->stats);

    printf("%s%s\x1b[0m", getPrefixEscapeCodes(config, files[i]->stats), prefix);
    printf("%s%s\x1b[0m", getNameEscapeCodes(config, files[i]->stats), files[i]->name);
    (*actualChar)+=strlen(files[i]->name) + strlen(prefix);
    int spaces = longestName+1 - ((*actualChar)%(longestName+1));
    for(int x = 0; x < spaces; x++)
    {
      printf(" ");
      (*actualChar)++;
    }
    (*actualColumn)++;
    if((*actualColumn) >= columns)
    {
      printf("\n");
      (*actualChar) = 0;
      (*actualColumn) = 0;
    }
  }
}

void displayList(FileInstance **files, int count, Args* args, PtShConfig *config, int longestName)
{
  for(int i = 0; i < count; i++)
  {
    char* prefix = getPrefix(config, files[i]->stats);
    char* prefixEC = getPrefixEscapeCodes(config, files[i]->stats);
    char* nameEC = getNameEscapeCodes(config, files[i]->stats);

    printf("%s%s\x1b[0m", prefixEC, prefix);
    printf("%s%s\x1b[0m", nameEC, files[i]->name);

    int spaces = longestName+1 - (strlen(prefix)+strlen(files[i]->name));
    for(int x = 0; x < spaces; x++) printf(" ");

    printPermissions(files[i], args);
    printUidGid(files[i], args);
    printf("\n");
  }
}

void display(FileInstance **files, int count, Args* args, PtShConfig *config,  int longestName, int* actualColumn, int* actualChar)
{
  if(args->l) displayList(files, count, args, config, longestName);
  else displayBlock(files, count, args, config, longestName, actualColumn, actualChar);
}


