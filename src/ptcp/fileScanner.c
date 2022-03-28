#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common/limits.h"

#include "ptcp.h"

void removeSlash(char ** val)
{
   if((*val)[strlen(*val)-1] == '/')
    (*val)[strlen(*val)-1] = '\0';
}

void addFile(Args *args, PtShConfig *config, MoveData *data, char *source, char *dest)
{
  if(data->files == NULL)
    data->files = calloc(1, sizeof(FilePaths*));
  else
    data->files = realloc(data->files, (data->fileCount+1) * sizeof(FilePaths*));

  FilePaths **filePtr = &data->files[data->fileCount];

  (*filePtr) = calloc(1, sizeof(FilePaths));

  (*filePtr)->sourcePath = source;
  (*filePtr)->destPath = dest;
  
  char path[PATH_MAX+1];
  realpath(source, path);

  struct stat stats;
  stat(path, &stats);

  data->totalBytes += stats.st_size;

  data->fileCount++;
}

void addSubdir(Args *args, MoveData *data, char *source, char *dest)
{
  if(data->subdirs == NULL)
    data->subdirs = malloc(sizeof(Subdir*));
  else
    data->subdirs = realloc(data->subdirs, (data->subdirCount+1)*sizeof(Subdir*));

  struct stat *sourceStats = malloc(sizeof(struct stat));
  
  char path[PATH_MAX+1];

  realpath(source, path);
 
  stat(path, sourceStats);

  int actualDir = data->subdirCount;

  data->subdirs[actualDir] = malloc(sizeof(Subdir));

  data->subdirs[actualDir]->name = calloc(strlen(dest)+1, sizeof(char));
  strcpy(data->subdirs[actualDir]->name, dest);

  data->subdirs[actualDir]->mode = sourceStats->st_mode;

  data->subdirCount++;
  free(sourceStats);
}

void scanSubdir(Args *args, PtShConfig *config, DIR *sourceDir, MoveData *data, char* subPath, int ignoreDestSubPath)
{
  struct dirent *d;

  addSubdir(args, data, subPath, subPath+strlen(subPath)-ignoreDestSubPath);

  while((d = readdir(sourceDir)) != NULL)
  {
    if(d->d_type == DT_DIR)
    {
      if(strcmp(&d->d_name[0], ".") == 0 || strcmp(&d->d_name[0], "..") == 0) continue;

      char* newSubPath = calloc(strlen(subPath) + 2 + strlen(&d->d_name[0]), sizeof(char));
      strcpy(newSubPath, subPath);
      strcat(newSubPath, &d->d_name[0]);
      strcat(newSubPath, "/");

      scanSubdir(args, config, opendir(newSubPath), data, newSubPath, ignoreDestSubPath+strlen(&d->d_name[0]-1));

      free(newSubPath);
      continue;
    } 
    char *source = calloc(strlen(subPath) + strlen(&d->d_name[0]) +1, sizeof(char));
    strcpy(source, subPath);
    strcat(source, &d->d_name[0]);

    char *ptr = subPath +strlen(subPath)- ignoreDestSubPath;

    char *dest = calloc(strlen(args->destPath) +strlen(ptr) + strlen(&d->d_name[0]) +2, sizeof(char));
    strcpy(dest, args->destPath);
    strcat(dest, "/");
    strcat(dest, ptr);
    strcat(dest, &d->d_name[0]);

    addFile(args, config, data, source, dest);
  }
}

MoveData *copyToDir(Args *args, PtShConfig *config)
{
  MoveData *data = calloc(1, sizeof(MoveData));
  
  removeSlash(&args->destPath);
  for(int i = 0; i < args->sourcePathCount; i++)
    removeSlash(&args->sourcePath[i]);
   
  for(int x = 0; x < args->sourcePathCount; x++)
  {
    char *sourcePath = args->sourcePath[x];

    DIR *sourceDir = opendir(sourcePath);
    if(sourceDir)
    {
      if(args->recursive)
      {
        char *subPath = calloc(strlen(sourcePath) +2, sizeof(char));
        strcpy(subPath, sourcePath);
        strcat(subPath, "/");

        int ignoreDestSubPath = 1;
        char *ptr = subPath+strlen(subPath)-2;
        while(*ptr != '/' && ptr != subPath) 
        {
          ptr--;
          ignoreDestSubPath++;
        }
        if(ptr != subPath) ptr++;
        else ignoreDestSubPath++;

        scanSubdir(args, config, sourceDir, data, subPath, ignoreDestSubPath); 

        free(subPath);

      }
      closedir(sourceDir);
      continue;
    }

    struct stat *stats = calloc(1,sizeof(struct stat));

    if(stat(sourcePath, stats) == 0)
    {
      char *ptr = sourcePath+strlen(sourcePath)-1;

      while(*ptr != '/' && ptr != sourcePath)
      {
        ptr--;
      }
      ptr++;

      char *dest = calloc(strlen(ptr)+2+strlen(args->destPath), sizeof(char));
      strcpy(dest, args->destPath);
      strcat(dest, "/");
      strcat(dest, ptr);

      char *source = calloc(strlen(sourcePath)+1, sizeof(char));
      strcpy(source, sourcePath);

      addFile(args, config, data, source, dest);
    }

    free(stats);
  }

  return data;
}

MoveData *copyToFile(const PtShConfig *config, Args *args)
{
  struct stat *stats = calloc(1, sizeof(struct stat));

  if(stat(args->sourcePath[0], stats) != 0)
  {
    printMessage(config, "Source file doesn't exist", true);
    free(stats);
    return NULL;
  }

  MoveData *data = calloc(1, sizeof(MoveData));
  data->fileCount = 1;
  data->totalBytes = stats->st_size;

  data->files = calloc(1, sizeof(FilePaths*));
  data->files[0] = calloc(1, sizeof(FilePaths));
  
  data->files[0]->sourcePath = calloc(strlen(args->sourcePath[0]) +1, sizeof(char));
  strcpy(data->files[0]->sourcePath, args->sourcePath[0]);
  data->files[0]->destPath = calloc(strlen(args->destPath) +1, sizeof(char));
  strcpy(data->files[0]->destPath, args->destPath);

  free(stats);

  return data;
}

MoveData *getMoveData(Args *args, PtShConfig *config)
{
  DIR *dir = opendir(args->destPath);

  if(dir)
  {
    closedir(dir);

    DIR *srcdir = opendir(args->sourcePath[0]);
    if(srcdir && !args->recursive)
    {
      closedir(srcdir);
      printMessage(config, "To copy a directory, You must use 'recursive' flag", true);
      return NULL;
    }

    return copyToDir(args, config);
  }else if(args->sourcePathCount == 1)
  {
    DIR *srcdir = opendir(args->sourcePath[0]);
    if(srcdir)
    {
      closedir(srcdir);
      printMessage(config, "Destination directory doesn't exist", true);
      return NULL;
    }
    return copyToFile(config, args);
  }

  printMessage(config, "Destination directory doesn't exist", true);
  return NULL;
}

void freeMoveData(MoveData *data)
{
  for(int i = 0; i < data->subdirCount; i++)
  {
    free(data->subdirs[i]->name);
    free(data->subdirs[i]);
  }
  for(int i = 0; i < data->fileCount; i++)
  {
    free(data->files[i]->sourcePath);
    free(data->files[i]->destPath);
    free(data->files[i]);
  }
  free(data->subdirs);
  free(data->files);
  free(data);
}
