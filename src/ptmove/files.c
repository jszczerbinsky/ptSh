#include "ptmove.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>

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

void copyFile(FilePaths *paths, int progressBarSize, unsigned long *actualByte, unsigned long* totalBytes)
{

  struct stat stats;
  stat(paths->sourcePath, &stats);

  if(S_ISREG(stats.st_mode))
  {

    FILE *source = fopen(paths->sourcePath, "rb");
    FILE *dest = fopen(paths->destPath, "wb");

    const unsigned int buffSize = 100000;
    unsigned char buff[buffSize];

    size_t readCount = 0;

    do
    {
      readCount = fread(buff, sizeof(unsigned char), buffSize, source);
      fwrite(buff, sizeof(unsigned char), readCount, dest); 

      (*actualByte) += readCount * sizeof(unsigned char);
      if(*totalBytes)
        setProgressBar(progressBarSize, (100*(*actualByte))/(*totalBytes));
      else
        setProgressBar(progressBarSize, 100);

    } while(readCount == buffSize);

    fclose(source);
    fclose(dest);
  }

  chmod(paths->destPath, stats.st_mode);
}

void copyFiles(Args *args, MoveData *mData)
{
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  setProgressBar(w.ws_col, 0);

  createSubdirs(args, mData);
  unsigned long actualByte = 0;

  for(int i = 0; i < mData->fileCount; i++)
    if(!mData->files[i]->ignore)
      copyFile(mData->files[i], w.ws_col, &actualByte, &mData->totalBytes);

  setProgressBar(w.ws_col, 100);
  printf("\n");
}
