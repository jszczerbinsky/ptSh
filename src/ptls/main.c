#include <string.h>

#include "ptls.h"

void groupDirs(DirContent *dirContent, int dirCount)
{
  int currentPos = 0;
  while(currentPos < dirCount)
  {
    if(!S_ISDIR(dirContent->files[currentPos]->stats->st_mode))
    {
      for(int i = currentPos+1; i < dirContent->fileCount; i++)
      {
        if(S_ISDIR(dirContent->files[i]->stats->st_mode))
        {
          File *tmp = dirContent->files[currentPos];
          dirContent->files[currentPos] = dirContent->files[i];
          dirContent->files[i] = tmp;
        }
      } 
    }
    currentPos++;
  }
}

bool fileVisible(File *file, Args *args)
{
    if((strcmp(file->name, ".") == 0 || strcmp(file->name, "..") == 0)  && !args->all) return false;
    if(file->name[0] == '.' && !(args->all || args->almostAll)) return false;
    if(file->name[strlen(file->name)-1] == '~' && args->ignoreBackups) return false;
    return true;
}

int main(int argc, char** argv)
{
  Args *args = parseArgs(argc, argv);

  DirContent *dirContent = getFiles(args);

  int dirCount = 0;
  int printSize = 0;

  for(int i = 0; i < dirContent->fileCount; i++)
  {
    if(fileVisible(dirContent->files[i], args))
    {
      printSize++;
      if(S_ISDIR(dirContent->files[i]->stats->st_mode)) dirCount++;
    }
  }

  PtShConfig *config = readConfig();

  if(dirContent->fileCount >1)
  {
    if(args->noDirsTop)
      sort(dirContent->files, args, 0, dirContent->fileCount-1);
    else
    {
      groupDirs(dirContent, dirCount);
      sort(dirContent->files, args, 0, dirCount-1);
      sort(dirContent->files, args, dirCount, dirContent->fileCount-1);
    }
  }

  ColumnSizes *columnSizes= calloc(1, sizeof(ColumnSizes));
  Fields **fields = malloc(printSize * sizeof(Fields*));

  int x = 0;
  for(int i = 0; i < printSize; i++)
  {
    while(!fileVisible(dirContent->files[x], args)) x++;
    fields[i] = calloc(1,sizeof(Fields));
    fillFields(fields[i], dirContent->files[x], config, args, columnSizes); 
    x++;
  }

  display(fields, printSize,args, config, columnSizes); 

  closeConfig(config);

  printf("\n");

  free(columnSizes);
  for(int i = 0; i < printSize; i++)
    freeFields(fields[i]);
  free(fields);
  freeContent(dirContent);
  free(args);

  return 0;
}
