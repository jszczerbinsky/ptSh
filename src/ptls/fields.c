#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>

#include "ptls.h"

int getIntDigits(int i)
{
  if(i == 0) return 1;
  else return log10(i)+1;
}

void setSize(Fields *fields, File *file, Args *args, ColumnSizes *cSize)
{
  if(S_ISDIR(file->stats->st_mode))
  {
    fields->size = calloc(2, sizeof(char));
    strcpy(fields->size, "-");
    return;
  }

  char buff[25];
  int length = printSize(buff, 25, file->stats->st_size, args->decimalSize);

  fields->size = strdup(buff);

  if(length > cSize->size) cSize->size = length;
}

void setHardlinks(Fields *fields, File *file, Args *args, ColumnSizes *cSize)
{
  int digits = getIntDigits(file->stats->st_nlink); 

  fields->hardlinks = calloc(digits+1, sizeof(char));
  sprintf(fields->hardlinks, "%d", file->stats->st_nlink);
  if(digits > cSize->hardlinks) cSize->hardlinks = digits;
}

void setInode(Fields *fields, File *file, Args *args, ColumnSizes *cSize)
{
  int digits = getIntDigits(file->stats->st_ino); 

  fields->inode = calloc(digits+1, sizeof(char));
  sprintf(fields->inode, "%d", file->stats->st_ino);
  if(digits > cSize->inode) cSize->inode = digits;
}

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
  mode_t mode = file->stats->st_mode;

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
    int digits = getIntDigits(file->stats->st_uid);

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
      int digits = getIntDigits(file->stats->st_gid);

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
  FileType fType = FT_File;
  
  if(S_ISDIR(file->stats->st_mode)) fType = FT_Directory;
  else if(S_ISLNK(file->stats->st_mode)) fType = FT_Link;

  FileConfigValues *fcv = getFileConfigValues(config, fType);
  
  int nameLength = strlen(fcv->prefix) + strlen(file->name);
  fields->nameLength = nameLength;
  if(nameLength > cSize->name) cSize->name = nameLength;

  nameLength += strlen(fcv->prefixEscapeCodes) + strlen(fcv->nameEscapeCodes) + strlen("\x1b[0m")*2;

  fields->name = calloc(nameLength+1, sizeof(char));
  strcpy(fields->name, fcv->prefixEscapeCodes);
  strcat(fields->name, fcv->prefix);
  strcat(fields->name, "\x1b[0m");
  strcat(fields->name, fcv->nameEscapeCodes);
  strcat(fields->name, file->name);
  strcat(fields->name, "\x1b[0m");

  free(fcv);

  if(!args->l) return;
  setPermissions(fields, file, args);
  setUidGid(fields, file, args, cSize);
  setDateTime(fields, file, args);
  setHardlinks(fields, file, args, cSize);
  setSize(fields, file, args, cSize);

  if(args->inode)
    setInode(fields, file, args, cSize);
}

void freeFields(Fields *fields)
{
  free(fields->name);
  free(fields->permissions);
  free(fields->date);
  free(fields->time);
  free(fields->size);
  free(fields->inode);
  free(fields->hardlinks);
  free(fields);
}
