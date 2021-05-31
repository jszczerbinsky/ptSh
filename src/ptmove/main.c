#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "ptmove.h"

int main(int argc, char **argv)
{
  Args *args = parseArgs(argc, argv);

  if(args->sourcePath == NULL || args->destPath == NULL) return 1;

  MoveData *mData = getMoveData(args); 
  if(mData == NULL)
  {
    printf("NULL");
    return 1;
  }

  printf("%d bytes\n", mData->totalBytes);
  printf("%d\n", mData->fileCount);

/*  setProgressBar(50,0);
  for(int i = 0; i < mData->totalBytes; i+= mData->totalBytes/10)
  {
    int count = (100*i)/mData->totalBytes;
    setProgressBar(50,count);
    sleep(1);
  }
  setProgressBar(50, 100);
  printf("\n");
*/
//  for(int i = 0; i < mData->subdirCount; i++)
//    printf("%s\n", mData->subdirs[i]->name);

  copyFiles(args, mData);

  freeMoveData(mData);

  freeArgs(args);
  
  return 0;
}
