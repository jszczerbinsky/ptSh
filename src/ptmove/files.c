#include "ptmove.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void createSubdirs(Args *args, MoveData *mData)
{
  for(int i = 0; i < mData->subdirCount; i++)
  {
    char *path = calloc(strlen(args->destPath)+strlen(mData->subdirs[i]->name) +2, sizeof(char)); 
    strcpy(path, args->destPath);
    strcat(path, "/");
    strcat(path, mData->subdirs[i]->name);

    mkdir(path, mData->subdirs[i]->mode);

    free(path);
  } 
}

void copyFile(FilePaths *paths, unsigned long *actualByte, unsigned long* totalBytes)
{
  FILE *source = fopen(paths->sourcePath, "rb");
  FILE *dest = fopen(paths->destPath, "wb");

  const unsigned int buffSize = 1000;
  unsigned char buff[buffSize];

  size_t readCount = 0;

  do
  {
    readCount = fread(buff, sizeof(unsigned char), buffSize, source);
    fwrite(buff, sizeof(unsigned char), readCount, dest); 

    (*actualByte) += readCount * sizeof(unsigned char);
    setProgressBar(50, (100*(*actualByte))/(*totalBytes));

  } while(readCount == buffSize);

  fclose(source);
  fclose(dest);

  struct stat stats;

  stat(paths->sourcePath, &stats);
  chmod(paths->destPath, stats.st_mode);
}

void copyFiles(Args *args, MoveData *mData)
{
  setProgressBar(50, 0);

  createSubdirs(args, mData);
  unsigned long actualByte = 0;

  for(int i = 0; i < mData->fileCount; i++)
    copyFile(mData->files[i], &actualByte, &mData->totalBytes);

  setProgressBar(50, 100);
  printf("\n");
}
