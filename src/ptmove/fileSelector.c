#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>

#include "ptmove.h"

void displayFile(PtShConfig *config, FilePaths *fPaths, unsigned int cols, bool selected)
{
  char *selectedPrefix = "";
  if(selected) selectedPrefix = "\x1b[7m";

  struct stat stats;
  char path[PATH_MAX+1];
  realpath(fPaths->sourcePath, path);

  FileType type = FT_File;
  if(S_ISLNK(stats.st_mode))
    type = FT_Link;

  FileConfigValues *fcv = getFileConfigValues(config, type);
  printf("%c[2K", 27); 
  printf("%s%s%s\x1b[0m%s%s", selectedPrefix, fcv->prefixEscapeCodes, fcv->prefix, selectedPrefix, fcv->nameEscapeCodes);
  printf("%s\x1b[0m%s", fPaths->sourcePath, selectedPrefix);

  unsigned int actualChar = strlen(fcv->prefix) + strlen(fPaths->sourcePath);

  char *fileStatus= getValueStr(config->successPrefix);
  char *fileStatusEC = getValueStr(config->successPrefixEscapeCodes);
  if(fPaths->ignore) 
  {
    fileStatus = getValueStr(config->errorPrefix);
    fileStatusEC = getValueStr(config->errorPrefixEscapeCodes);
  }

  for(int i = 0; i < cols - actualChar - strlen(fileStatus); i++)
    printf(" ");

  printf("%s%s", fileStatusEC, fileStatus);

  printf("\x1b[0m\n");
  
  fflush(stdout);

  free(fcv);
}
void displaySelector(PtShConfig *config, unsigned int page, unsigned int lastPage, unsigned int pageFileCount, unsigned int cols, unsigned int selectedFile, MoveData *mData)
{
  printf("\n\x1b[45m");
  printf("page (%d/%d) | h,l - jump between pages | j,k - jump between files | x - remove file | ENTER - add file\n", page+1, lastPage+1);
  printf("\x1b[0m");
  int firstFile = page * pageFileCount;

  for(int i = 0; i < pageFileCount; i++)
  {
    if(i+firstFile >= mData->fileCount)
      printf("\n");
    else
      displayFile(config, mData->files[i+firstFile], cols, i == selectedFile);
  }
}

void nextPage(unsigned int *actualPage, unsigned int *lastPage)
{
  (*actualPage)++;
  if((*actualPage) > (*lastPage)) (*actualPage) = (*lastPage);
}

void selectFiles(PtShConfig *config, MoveData *mData)
{
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  unsigned int pageFileCount = w.ws_row -3;
  unsigned int lastPage = mData->fileCount/pageFileCount;

  unsigned int page = 0;
  unsigned int selectedFile = 0;

  char c;

  do
  {
    int fileIndex = selectedFile+page * pageFileCount;
    if(fileIndex >= mData->fileCount)
    {
      selectedFile = mData->fileCount%pageFileCount-1;
      continue;
    }

    displaySelector(config, page, lastPage, pageFileCount, w.ws_col, selectedFile, mData);
    c = getchar();

    switch(c)
    {
      case '0':
        selectedFile = 0;
        break;
      case 'j':
        if(selectedFile != pageFileCount-1)
          selectedFile++;
        break;
      case 'k':
        if(selectedFile != 0)
          selectedFile--;
        break;
      case 'l':
        nextPage(&page, &lastPage);
        break;
      case 'h':
        if(page != 0)
          page--;
        break;
      case '\n':
        mData->files[fileIndex]->ignore = false;
        selectedFile++;
        if(selectedFile >= pageFileCount)
        {
          nextPage(&page, &lastPage);
          selectedFile = 0;
        }
        break;
      case 'x':
        mData->files[fileIndex]->ignore = true;
        selectedFile++;
        if(selectedFile >= pageFileCount)
        {
          nextPage(&page, &lastPage);
          selectedFile = 0;
        }
        break;
    }
  }while(c != 'q');

  for(int i = 0; i < mData->fileCount; i++)
  {
    if(!mData->files[i]->ignore) continue;

    struct stat stats;  
    stat(mData->files[i]->sourcePath, &stats);

    mData->totalBytes -= stats.st_size;
  }
}
