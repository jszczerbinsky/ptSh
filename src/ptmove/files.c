#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "ptmove.h"

MoveData *copyToDir(Args *args, DIR *dest)
{

    closedir(dest); 
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
  
  data->files[0]->sourcePath = args->sourcePath;
  data->files[0]->destPath = args->destPath;

  free(stats);
  return data;
}

MoveData *getMoveData(Args *args)
{
  DIR *dir = opendir(args->destPath);

  if(dir)
  {
    return copyToDir(args, dir);
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
    free(data->files[i]);
  free(data->files);
  free(data);
}
