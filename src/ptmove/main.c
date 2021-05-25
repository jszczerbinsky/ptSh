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
  for(int i = 0; i < mData->fileCount; i++)
  {
    printf("%s ->", mData->files[i]->sourcePath);
    printf(" %s\n", mData->files[i]->destPath);
  }

  freeMoveData(mData);

  freeArgs(args);
  
  return 0;
}
