#include <sys/ioctl.h>
#include <sys/stat.h>
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

  char** dirnames = calloc(dirCount, sizeof(char*) + 1);
  struct stat **dirStats = (struct stat**)malloc(dirCount * sizeof(struct stat*));

  char** filenames = calloc(fileCount, sizeof(char*) + 1);
  struct stat **fileStats = (struct stat**)malloc(fileCount * sizeof(struct stat*));

  int actualDir = 0;
  
  for(int i = 0; i < files->count; i++)
  {
    if(!args->noDirsTop && S_ISDIR(stats[i]->st_mode))    
    {
      dirnames[actualDir] = (char*)malloc(strlen(files->names[i])+1);
      strcpy(dirnames[actualDir], files->names[i]);
      dirStats[actualDir] = (struct stat*)malloc(sizeof(struct stat));
      memcpy(dirStats[actualDir], stats[i], sizeof(struct stat));
      actualDir++;
    }
    else
    {
      filenames[i-actualDir] = (char*)malloc(strlen(files->names[i]) +1);
      strcpy(filenames[i-actualDir], files->names[i]);
      fileStats[i-actualDir] = (struct stat*)malloc(sizeof(struct stat));
      memcpy(fileStats[i-actualDir], stats[i-actualDir], sizeof(struct stat));
      
    }
  }
  free(stats);
  free(files);

  for(int i = 0; i < dirCount; i++)
    printf("%s\n", dirnames[i]);

  for(int i = 0; i < fileCount; i++)
    printf("%s\n", filenames[i]);

  free(dirnames);
  free(dirStats);
  free(filenames);
  free(fileStats);

  free(args);

  return 0;
}
