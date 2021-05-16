#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include "ptls.h"

void setPermissions(PrintFileData *printData, FileInstance *file, Args *args)
{
  short noGroupDecrease = 0;

  if(args->noGroup)
    noGroupDecrease = 4;

  char *permStr = calloc(12-noGroupDecrease, sizeof(char));
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
  
  printData->permissions = permStr;
}

void setUidGid(PrintFileData *printData, FileInstance *file, Args *args, ColumnLengths *lengths)
{
  struct passwd *user = getpwuid(file->stats->st_uid);
  printData->uid = calloc(strlen(user->pw_name) +1,  sizeof(char));
  strcpy(printData->uid, user->pw_name);
  if(strlen(printData->uid) > lengths->uid) lengths->uid = strlen(printData->uid); 

  if(!args->noGroup)
  {
    struct passwd *group = getpwuid(file->stats->st_gid);
    printData->gid = calloc(strlen(group->pw_name) +1, sizeof(char));
    strcpy(printData->gid, group->pw_name);
  }
}

void setPrintData(PrintFileData *printData, FileInstance *instance, PtShConfig *config, Args *args, ColumnLengths *lengths)
{
  char* prefixEC = getPrefixEscapeCodes(config, instance->stats);
  char* prefix = getPrefix(config, instance->stats);
  char* nameEC = getNameEscapeCodes(config, instance->stats);
  
  int nameLength = strlen(prefix) + strlen(instance->name);
  printData->nameLength = nameLength;
  if(nameLength > lengths->name) lengths->name = nameLength;

  nameLength += strlen(prefixEC) + strlen(nameEC) + strlen("\x1b[0m")*2;

  printData->name = calloc(nameLength+1, sizeof(char));
  strcpy(printData->name, prefixEC);
  strcat(printData->name, prefix);
  strcat(printData->name, "\x1b[0m");
  strcat(printData->name, nameEC);
  strcat(printData->name, instance->name);
  strcat(printData->name, "\x1b[0m");

  if(!args->l) return;
  setPermissions(printData, instance, args);
  setUidGid(printData, instance, args, lengths);
}

void displayBlock(PrintFileData **printData, int count, ColumnLengths *lengths, Args* args, PtShConfig *config)
{
  int longestName = lengths->name;
  int actualChar = 0;
  int actualColumn = 0;

  if(longestName == 0) return;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  int columns = w.ws_col/(longestName+1);

  for(int i = 0; i < count; i++)
  {
    printf("%s", printData[i]->name);
    actualChar+=printData[i]->nameLength;
    int spaces = longestName+1 - (actualChar%(longestName+1));
    for(int x = 0; x < spaces; x++)
    {
      printf(" ");
      actualChar++;
    }
    actualColumn++;
    if(actualColumn >= columns)
    {
      printf("\n");
      actualChar = 0;
      actualColumn = 0;
    }
  }
}

void displayList(PrintFileData **files, int count, ColumnLengths *lengths, Args* args, PtShConfig *config)
{
  for(int i = 0; i < count; i++)
  {
    printf("%s", files[i]->name);

    int spaces = lengths->name+1 - files[i]->nameLength;
    for(int x = 0; x < spaces; x++) printf(" ");

    printf("%s %s", files[i]->permissions, files[i]->uid);

    spaces = lengths->uid+1 - strlen(files[i]->uid);
    for(int x = 0; x < spaces; x++) printf(" ");

    if(!args->noGroup)
      printf("%s", files[i]->gid);

    printf("\n");
  }
}

void display(PrintFileData **files, int count, Args* args, PtShConfig *config, ColumnLengths *lengths)
{
  if(args->l) displayList(files, count, lengths, args, config);
  else displayBlock(files, count, lengths, args, config); 
}


