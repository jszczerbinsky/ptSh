#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "ptls.h"

DirContent *getFiles(Args *args)
{
  DIR *d;
  struct dirent *dir;
  DirContent *content= (DirContent*)malloc(sizeof(DirContent));
  content->fileCount = 0;

  d = opendir(args->dirPath);

  if(d)
  {
    long loc = telldir(d);
    int filesCount = 0;
    while (readdir(d) != NULL) filesCount++;
    seekdir(d, loc);

    content->files = (File**)calloc(filesCount, sizeof(File*));

    int i = 0;
    while ((dir = readdir(d)) != NULL)
    {
      content->files[i] = (File*)malloc(sizeof(File));
      content->files[i]->name = (char*)malloc(strlen(dir->d_name) +1);
      content->files[i]->stats = (struct stat*)malloc(sizeof(struct stat));
      
      strcpy(content->files[i]->name, dir->d_name);
      lstat(content->files[i]->name, content->files[i]->stats); 
      i++;
    }

    content->fileCount = filesCount;

    closedir(d); 
    free(dir);
  }

  return content;
}

void freeContent(DirContent *dirContent)
{
  for(int i = 0; i < dirContent->fileCount; i++)
  {
    free(dirContent->files[i]->name);
    free(dirContent->files[i]->stats);
    free(dirContent->files[i]);
  }
  free(dirContent); 
}
