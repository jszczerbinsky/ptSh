#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <ptsh.h>

#include "ptls.h"

bool fileVisible(FileInstance *file, Args *args)
{
    if((strcmp(file->name, ".") == 0 || strcmp(file->name, "..") == 0)  && !args->all) return false;
    if(file->name[0] == '.' && !(args->all || args->almostAll)) return false;
    if(file->name[strlen(file->name)-1] == '~' && args->ignoreBackups) return false;
    return true;
}

void displayBlock(FileInstance **files, int count, Args* args, int longestName, int* actualColumn, int* actualChar)
{
  if(longestName == 0) return;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  int columns = w.ws_col/(longestName+1);

  for(int i = 0; i < count; i++)
  {
    printf("%s", files[i]->name);
    (*actualChar)+=strlen(files[i]->name);
    int spaces = longestName+1 - ((*actualChar)%(longestName+1));
    for(int x = 0; x < spaces; x++)
    {
      printf(" ");
      (*actualChar)++;
    }
    (*actualColumn)++;
    if((*actualColumn) >= columns)
    {
      printf("\n");
      (*actualChar) = 0;
      (*actualColumn) = 0;
    }
  }
}

void displayList(FileInstance **files, int count, Args* args, int longestName)
{

}

void display(FileInstance **files, int count, Args* args, int longestName, int* actualColumn, int* actualChar)
{
  if(args->l) displayList(files, count, args, longestName);
  else displayBlock(files, count, args, longestName, actualColumn, actualChar);
}

int main(int argc, char** argv)
{
  Args *args = parseArgs(argc, argv);

  Files *allFiles = getFiles();

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

  PtShConfig *config = readConfig();

  display(dirs, dirCount, args, longestName, &actualColumn, &actualChar);
  display(files, fileCount, args, longestName, &actualColumn, &actualChar);

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
