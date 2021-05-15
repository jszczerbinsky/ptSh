#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "ptls.h"

Files *getFiles(Args *args)
{
  DIR *d;
  struct dirent *dir;
  Files *files = (Files*)malloc(sizeof(Files));
  files->count = 0;

  d = opendir(args->dirPath);

  if(d)
  {
    long loc = telldir(d);
    int filesCount = 0;
    while (readdir(d) != NULL) filesCount++;
    seekdir(d, loc);

    files->instances = (FileInstance**)calloc(filesCount, sizeof(FileInstance*));

    int i = 0;
    while ((dir = readdir(d)) != NULL)
    {
      files->instances[i] = (FileInstance*)malloc(sizeof(FileInstance));
      files->instances[i]->name = (char*)malloc(strlen(dir->d_name) +1);
      files->instances[i]->stats = (struct stat*)malloc(sizeof(struct stat));
      
      strcpy(files->instances[i]->name, dir->d_name);
      lstat(files->instances[i]->name, files->instances[i]->stats); 
      i++;
    }

    files->count = filesCount;

    closedir(d); 
    free(dir);
  }

  return files;
}
