#include <string.h>

#include "ptls.h"

bool fileVisible(FileInstance *file, Args *args)
{
    if((strcmp(file->name, ".") == 0 || strcmp(file->name, "..") == 0)  && !args->all) return false;
    if(file->name[0] == '.' && !(args->all || args->almostAll)) return false;
    if(file->name[strlen(file->name)-1] == '~' && args->ignoreBackups) return false;
    return true;
}

int main(int argc, char** argv)
{
  Args *args = parseArgs(argc, argv);

  Files *files = getFiles(args);

  int dirCount = 0;
  int printSize = 0;

  for(int i = 0; i < files->count; i++)
  {
    if(fileVisible(files->instances[i], args))
    {
      printSize++;
      if(S_ISDIR(files->instances[i]->stats->st_mode)) dirCount++;
    }
  }

  PtShConfig *config = readConfig();

  if(files->count >1) sort(files->instances, args, 0, files->count-1);

  ColumnLengths *lengths = calloc(1, sizeof(ColumnLengths));
  PrintFileData **printData = malloc(printSize * sizeof(PrintFileData*));

  int x = 0;
  for(int i = 0; i < printSize; i++)
  {
    while(!fileVisible(files->instances[x], args)) x++;
    printData[i] = calloc(1,sizeof(PrintFileData));
    setPrintData(printData[i], files->instances[x], config, args, lengths); 
    x++;
  }

  display(printData, printSize,args, config, lengths); 

  closeConfig(config);

  printf("\n");

  free(lengths);
  for(int i = 0; i < printSize; i++)
  {
    free(printData[i]->name);
    free(printData[i]->permissions);
    free(printData[i]->date);
    free(printData[i]->time);
    free(printData[i]->size);
    free(printData[i]);
  }
  free(printData);
  for(int i = 0; i < files->count; i++)
  {
    free(files->instances[i]->name);
    free(files->instances[i]->stats);
    free(files->instances[i]);
  }
  free(files); 
  free(args);

  return 0;
}
