#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>

#include "ptls.h"

void setDateTime(Fields *fields, File *file, Args *args)
{
  time_t *time = &(file->stats->st_ctime);

  fields->date = calloc(13, sizeof(char));
  strftime(fields->date, 12, "%Y-%m-%d", localtime(time));

  if(!args->fullTime && !args->time) return;

  fields->time= calloc(13, sizeof(char));

  if(args->fullTime)
    strftime(fields->time, 12, "%H:%M:%S", localtime(time));
  else if(args->time)
    strftime(fields->time, 12, "%H:%M", localtime(time));

}

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
  if(args->numericUidGid)
  {
    int digits = 1;
    if(file->stats->st_uid > 0)
      digits = log10(file->stats->st_uid)+1;

    fields->uid = calloc(digits+1, sizeof(char));
    sprintf(fields->uid, "%d", file->stats->st_uid);
    if(digits > cSize->uid) cSize->uid = digits;
  
  }else
  {
    struct passwd *user = getpwuid(file->stats->st_uid);

    fields->uid = calloc(strlen(user->pw_name) +1,  sizeof(char));
    strcpy(fields->uid, user->pw_name);
    if(strlen(fields->uid) > cSize->uid) cSize->uid = strlen(fields->uid); 
  }
  if(!args->noGroup)
  {
    if(args->numericUidGid)
    {
      int digits = 1;
      if(file->stats->st_gid > 0)
        digits = log10(file->stats->st_gid)+1;

      fields->gid = calloc(digits+1, sizeof(char));
      sprintf(fields->gid, "%d", file->stats->st_gid);
    if(digits > cSize->gid) cSize->gid = digits;

    }else
    {
      struct group *group = getgrgid(file->stats->st_gid);
      fields->gid = calloc(strlen(group->gr_name) +1, sizeof(char));
      strcpy(fields->gid, group->gr_name);
      if(strlen(fields->gid) > cSize->gid) cSize->gid = strlen(fields->gid); 
    }
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
  setDateTime(fields, file, args);
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
