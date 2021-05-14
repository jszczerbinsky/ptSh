#include <sys/ioctl.h>
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
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w); 

  Args *args = parseArgs(argc, argv);

  Files *allFiles = getFiles();

  int dirCount = 0;
  int fileCount = 0;

  for(int i = 0; i < allFiles->count; i++)
  {
    if(!fileVisible(allFiles->instances[i], args)) continue;
    if(S_ISDIR(allFiles->instances[i]->stats->st_mode)) dirCount++;
    else fileCount++;

  }

  FileInstance **dirs= (FileInstance**)malloc(dirCount * sizeof(FileInstance*));
  FileInstance **files= (FileInstance**)malloc(fileCount * sizeof(FileInstance*));

  int actualDir = 0;
  int actualFile = 0;
  
  for(int i = 0; i < allFiles->count; i++)
  {
    if(!fileVisible(allFiles->instances[i], args)) continue;

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

  sort(dirs, args, 0, dirCount-1);

  for(int i = 0; i < dirCount; i++)
    printf("%s\n", dirs[i]->name);

  sort(files, args, 0, fileCount-1);

  for(int i = 0; i < fileCount; i++)
    printf("%s\n", files[i]->name);


  free(dirs);
  free(files);

  free(args);

  return 0;
}
