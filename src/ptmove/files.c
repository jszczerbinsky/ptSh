#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "ptmove.h"

void removeSlash(char ** val)
{
   if((*val)[strlen(*val)-1] == '/')
    (*val)[strlen(*val)-1] = '\0';
}

void addFile(Args *args, MoveData *data, char *source, char *dest)
{
  if(data->files == NULL)
    data->files = calloc(1, sizeof(FilePaths*));
  else
    data->files = realloc(data->files, (data->fileCount+1) * sizeof(FilePaths*));

  FilePaths **filePtr = &data->files[data->fileCount];

  (*filePtr) = calloc(1, sizeof(FilePaths));

  (*filePtr)->sourcePath = source;
  (*filePtr)->destPath = dest;

  data->fileCount++;

}

void scanSubdir(Args *args, DIR *source, MoveData *data, char* subPath, int ignoreDestSubPath)
{
  struct dirent *d;

  while((d = readdir(source)) != NULL)
  {
    if(d->d_type == DT_DIR)
    {
      if(strcmp(&d->d_name[0], ".") == 0 || strcmp(&d->d_name[0], "..") == 0) continue;

      char* newSubPath = calloc(strlen(subPath) + 2 + strlen(&d->d_name[0]), sizeof(char));
      strcpy(newSubPath, subPath);
      strcat(newSubPath, &d->d_name[0]);
      strcat(newSubPath, "/");

      scanSubdir(args, opendir(newSubPath), data, newSubPath, ignoreDestSubPath+strlen(&d->d_name[0]-1));

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

    addFile(args, data, source, dest);
  }
}

MoveData *copyToDir(Args *args)
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
      char *subPath = calloc(strlen(sourcePath) +2, sizeof(char));
      strcpy(subPath, sourcePath);
      strcat(subPath, "/");

      int ignoreDestSubPath = 1;
      char *ptr = subPath+strlen(subPath)-2;
      while(*ptr != '/' && ptr != subPath-1) 
      {
        ptr--;
        ignoreDestSubPath++;
      }
      ptr++;

      scanSubdir(args, sourceDir, data, subPath, ignoreDestSubPath); 

      free(subPath);

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

      addFile(args, data, source, dest);
    }

    free(stats);
  }

  return data;
}

MoveData *copyToFile(Args *args)
{
  struct stat *stats = calloc(1, sizeof(struct stat));

  if(stat(args->sourcePath[0], stats) != 0)
  {
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

MoveData *getMoveData(Args *args)
{
  DIR *dir = opendir(args->destPath);

  if(dir)
  {
    closedir(dir);
    return copyToDir(args);
  }else
  {
    FILE *file = fopen(args->destPath, "w");
    if(file != NULL)
    {
      fclose(file);
      return copyToFile(args);
    } 
  }
  return NULL;
}

void freeMoveData(MoveData *data)
{
  for(int i = 0; i < data->fileCount; i++)
  {
    free(data->files[i]->sourcePath);
    free(data->files[i]->destPath);
    free(data->files[i]);
  }
  free(data->files);
  free(data);
}
