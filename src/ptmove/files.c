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

void scanSubdir(Args *args, DIR *source, MoveData *data, char* subPath)
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

      scanSubdir(args, opendir(newSubPath), data, newSubPath);

      free(newSubPath);
      continue;
    } 

    if(data->files == NULL)
      data->files = calloc(1, sizeof(FilePaths*));
    else
      data->files = realloc(data->files, (data->fileCount+1) * sizeof(FilePaths*));

    FilePaths **filePtr = &data->files[data->fileCount];

    (*filePtr) = calloc(1, sizeof(FilePaths));

    (*filePtr)->sourcePath = calloc(strlen(subPath) + strlen(&d->d_name[0]) +1, sizeof(char));
    strcpy((*filePtr)->sourcePath, subPath);
    strcat((*filePtr)->sourcePath, &d->d_name[0]);

    (*filePtr)->destPath = calloc(strlen(args->destPath) +strlen(subPath) + strlen(&d->d_name[0]) +2, sizeof(char));
    strcpy((*filePtr)->destPath, args->destPath);
    strcat((*filePtr)->destPath, "/");
    strcat((*filePtr)->destPath, subPath);
    strcat((*filePtr)->destPath, &d->d_name[0]);

    data->fileCount++;
  }
}

MoveData *copyToDir(Args *args)
{
  MoveData *data = calloc(1, sizeof(MoveData));
  
  removeSlash(&args->destPath);
  removeSlash(&args->sourcePath);
   
  DIR *sourceDir = opendir(args->sourcePath);
  if(sourceDir)
  {
    char *subPath = calloc(strlen(args->sourcePath) +2, sizeof(char));
    strcpy(subPath, args->sourcePath);
    strcat(subPath, "/");

    scanSubdir(args, sourceDir, data, subPath); 

    free(subPath);

    closedir(sourceDir);
    return data;
  }

  struct stat *stats = calloc(1,sizeof(struct stat));

  if(stat(args->sourcePath, stats) == 0)
  {
    
    data->fileCount = 1;
    data->totalBytes = stats->st_size;

    data->files = calloc(1, sizeof(FilePaths*));
    data->files[0] = calloc(1, sizeof(FilePaths));
    
    data->files[0]->sourcePath = calloc(strlen(args->sourcePath) +1, sizeof(char));
    strcpy(data->files[0]->sourcePath, args->sourcePath);

    int nameLength = 0;
    char* ptr = args->sourcePath+strlen(args->sourcePath)-1;

    while(*ptr != '/' && nameLength < strlen(args->sourcePath))
    {
      nameLength++;
      ptr--;
    }
    ptr++;

    
    data->files[0]->destPath = calloc(strlen(args->destPath) + nameLength + 2, sizeof(char));
    strcpy(data->files[0]->destPath, args->destPath);
    strcat(data->files[0]->destPath, "/");
    strcat(data->files[0]->destPath, ptr);
    
    free(stats);
    return data;
  }

  free(stats);
  free(data);
  return NULL;
}

MoveData *copyToFile(Args *args)
{
  struct stat *stats = calloc(1, sizeof(struct stat));

  if(stat(args->sourcePath, stats) != 0)
  {
    free(stats);
    return NULL;
  }

  MoveData *data = calloc(1, sizeof(MoveData));
  data->fileCount = 1;
  data->totalBytes = stats->st_size;

  data->files = calloc(1, sizeof(FilePaths*));
  data->files[0] = calloc(1, sizeof(FilePaths));
  
  data->files[0]->sourcePath = calloc(strlen(args->sourcePath) +1, sizeof(char));
  strcpy(data->files[0]->sourcePath, args->sourcePath);
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
