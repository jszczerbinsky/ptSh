#include <dirent.h>
#include <string.h>
#include "ptls.h"

Files *getFiles()
{
  DIR *d;
  struct dirent *dir;
  Files *files = (Files*)malloc(sizeof(Files));

  d = opendir(".");

  if(d)
  {
    long loc = telldir(d);
    int filesCount = 0;
    while (readdir(d) != NULL) filesCount++;
    seekdir(d, loc);

    char **names = calloc(filesCount, sizeof(char*));

    int x = 0;
    while ((dir = readdir(d)) != NULL)
    {
      char* name = (char*)malloc(strlen(dir->d_name) +1);
      strcpy(dir->d_name, name);
      names[x] = name;
      x++;
    }

    files->names = names;
    files->count = filesCount;

    closedir(d); 
  }
  free(dir);

  return files;
}
