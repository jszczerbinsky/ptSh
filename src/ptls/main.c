#include <sys/ioctl.h>
#include <string.h>

#include "ptls.h"

int main(int argc, char** argv)
{
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w); 

  Args *args = parseArgs(argc, argv);

  Files *files = getFiles();

  struct stat **stats = (struct stat**)malloc(files->count * sizeof(struct stat*));

  int dirCount = 0;

  for(int i = 0; i < files->count; i++)
  {
    stats[i] = (struct stat*)malloc(sizeof(struct stat));
    stat(files->names[i], stats[i]);
    if(!args->noDirsTop && S_ISDIR(stats[i]->st_mode)) dirCount++;
  }

  int fileCount = files->count - dirCount;

  FileInstance **dirInstances = (FileInstance**)malloc(dirCount * sizeof(FileInstance*));
  FileInstance **fileInstances = (FileInstance**)malloc(fileCount * sizeof(FileInstance*));

  int actualDir = 0;
  
  for(int i = 0; i < files->count; i++)
  {
    if(!args->noDirsTop && S_ISDIR(stats[i]->st_mode))    
    {
      dirInstances[actualDir] = (FileInstance*)malloc(sizeof(FileInstance));
      dirInstances[actualDir]->name = malloc(strlen(files->names[i]) +1);
      dirInstances[actualDir]->stats = (struct stat*)malloc(sizeof(struct stat));
      strcpy(dirInstances[actualDir]->name, files->names[i]);
      memcpy(dirInstances[actualDir]->stats, stats[i], sizeof(struct stat));
      actualDir++;
    }
    else
    {
      fileInstances[i-actualDir] = (FileInstance*)malloc(sizeof(FileInstance));
      fileInstances[i-actualDir]->name = (char*)malloc(strlen(files->names[i]) +1);
      fileInstances[i-actualDir]->stats = (struct stat*)malloc(sizeof(struct stat));
      strcpy(fileInstances[i-actualDir]->name, files->names[i]);
      memcpy(fileInstances[i-actualDir]->stats, stats[i], sizeof(struct stat));
      
    }
  }
  free(stats);
  free(files);

  sort(dirInstances, args, 0, dirCount-1);

  for(int i = 0; i < dirCount; i++)
    printf("%s\n", dirInstances[i]->name);

  sort(fileInstances, args, 0, fileCount-1);

  for(int i = 0; i < fileCount; i++)
    printf("%s\n", fileInstances[i]->name);


  free(dirInstances);
  free(fileInstances);

  free(args);

  return 0;
}
