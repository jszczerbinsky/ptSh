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

  Files *allFiles = getFiles(args);

  int dirCount = 0;
  int fileCount = 0;

  for(int i = 0; i < allFiles->count; i++)
  {
    if(!fileVisible(allFiles->instances[i], args)) continue;
    if(!args->noDirsTop && S_ISDIR(allFiles->instances[i]->stats->st_mode)) dirCount++;
    else fileCount++;

  }

  FileInstance **dirs;
  FileInstance **files;
  if(dirCount > 0)dirs = (FileInstance**)malloc(dirCount * sizeof(FileInstance*));
  if(fileCount >0)files= (FileInstance**)malloc(fileCount * sizeof(FileInstance*));

  PtShConfig *config = readConfig();
  int actualDir = 0;
  int actualFile = 0;

  int longestName = 0;
  
  for(int i = 0; i < allFiles->count; i++)
  {
    if(!fileVisible(allFiles->instances[i], args)){
      free(allFiles->instances[i]->stats);
      free(allFiles->instances[i]->name);
      free(allFiles->instances[i]);
      continue;
    }

    int nameLength = strlen(allFiles->instances[i]->name);
    nameLength += strlen(getPrefix(config, allFiles->instances[i]->stats));

    if(nameLength > longestName) longestName = nameLength;

    if(!args->noDirsTop && S_ISDIR(allFiles->instances[i]->stats->st_mode))    
    {
      dirs[actualDir] = allFiles->instances[i];
      actualDir++;
    }
    else
    {
      files[actualFile] = allFiles->instances[i];
      actualFile++;
    }
  }
  free(allFiles);

  if(dirCount > 1) sort(dirs, args, 0, dirCount-1);
  if(fileCount >1) sort(files, args, 0, fileCount-1);

  int actualColumn = 0;
  int actualChar = 0;

  display(dirs, dirCount, args, config, longestName, &actualColumn, &actualChar);
  display(files, fileCount, args, config, longestName, &actualColumn, &actualChar);

  closeConfig(config);

  printf("\n");

  for(int i = 0; i < dirCount; i++)
  {
    free(dirs[i]->name);
    free(dirs[i]->stats);
  }
  for(int i = 0; i < fileCount; i++)
  {
    free(files[i]->name);
    free(files[i]->stats);
  }
  if(dirCount >0)free(dirs);
  if(fileCount>0)free(files);

  free(args);

  return 0;
}
