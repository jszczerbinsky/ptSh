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
  printf("%s", fPaths->sourcePath);

  unsigned int actualChar = strlen(fcv->prefix) + strlen(fPaths->sourcePath);

  for(int i = 0; i < cols - actualChar; i++)
    printf(" ");

  printf("\x1b[0m\n");

  free(fcv);
}
void displaySelector(PtShConfig *config, unsigned int page, unsigned int lastPage, unsigned int rows, unsigned int cols, unsigned int selectedFile, MoveData *mData)
{
  printf("\e[2J\e[H");
  printf("page (%d/%d)\n", page+1, lastPage+1);
  int firstFile = page * (rows - 2);

  for(int i = 0; i < rows - 2; i++)
  {
    if(i+firstFile >= mData->fileCount)
      printf("\n");
    else
      displayFile(config, mData->files[i+firstFile], cols, i == selectedFile);
  }
}
void selectFiles(PtShConfig *config, MoveData *mData)
{
  /*
  
  char c;
  do
    c = getchar();
  while(c != 'y' && c != 'n');

  if(c == 'n') return;
*/
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  unsigned int lastPage = mData->fileCount/(w.ws_row-2);

  unsigned int page = 0;
  unsigned int selectedFile = 0;

  char c;

  do
  {
    displaySelector(config, page, lastPage, w.ws_row, w.ws_col, selectedFile, mData);
    c = getchar();

    switch(c)
    {
      case 'j':
        selectedFile++;
        selectedFile %=w.ws_row-2;
        break;
      case 'k':
        if(selectedFile == 0)
          selectedFile = w.ws_row-3;
        else
          selectedFile--;
        break;
      case 'l':
        page++;
        page %= lastPage+1;
        break;
      case 'h':
        if(page == 0)
          page = lastPage;
        else
          page--;
    }
  }while(c != 'q');
}
