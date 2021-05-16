#include <string.h>
#include <pwd.h>

#include "ptls.h"

void setPermissions(Fields *fields, File *file, Args *args)
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
  
  fields->permissions = permStr;
}

void setUidGid(Fields *fields, File *file, Args *args, ColumnSizes *cSize)
{
  struct passwd *user = getpwuid(file->stats->st_uid);
  fields->uid = calloc(strlen(user->pw_name) +1,  sizeof(char));
  strcpy(fields->uid, user->pw_name);
  if(strlen(fields->uid) > cSize->uid) cSize->uid = strlen(fields->uid); 

  if(!args->noGroup)
  {
    struct passwd *group = getpwuid(file->stats->st_gid);
    fields->gid = calloc(strlen(group->pw_name) +1, sizeof(char));
    strcpy(fields->gid, group->pw_name);
  }
}

void fillFields(Fields *fields, File *file, PtShConfig *config, Args *args, ColumnSizes *cSize)
{
  char* prefixEC = getPrefixEscapeCodes(config, file->stats);
  char* prefix = getPrefix(config, file->stats);
  char* nameEC = getNameEscapeCodes(config, file->stats);
  
  int nameLength = strlen(prefix) + strlen(file->name);
  fields->nameLength = nameLength;
  if(nameLength > cSize->name) cSize->name = nameLength;

  nameLength += strlen(prefixEC) + strlen(nameEC) + strlen("\x1b[0m")*2;

  fields->name = calloc(nameLength+1, sizeof(char));
  strcpy(fields->name, prefixEC);
  strcat(fields->name, prefix);
  strcat(fields->name, "\x1b[0m");
  strcat(fields->name, nameEC);
  strcat(fields->name, file->name);
  strcat(fields->name, "\x1b[0m");

  if(!args->l) return;
  setPermissions(fields, file, args);
  setUidGid(fields, file, args, cSize);
}

void freeFields(Fields *fields)
{
  free(fields->name);
  free(fields->permissions);
  free(fields->date);
  free(fields->time);
  free(fields->size);
  free(fields);
}
