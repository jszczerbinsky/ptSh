#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include "ptls.h"

void displayBlock(Fields **fields, int count, ColumnSizes *cSize, Args* args, PtShConfig *config)
{
  int longestName = cSize->name;
  int actualChar = 0;
  int actualColumn = 0;

  if(longestName == 0) return;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  int columns = w.ws_col/(longestName+1);

  for(int i = 0; i < count; i++)
  {
    printf("%s", fields[i]->name);
    actualChar+=fields[i]->nameLength;
    int spaces = longestName+1 - (actualChar%(longestName+1));
    for(int x = 0; x < spaces; x++)
    {
      printf(" ");
      actualChar++;
    }
    actualColumn++;
    if(actualColumn >= columns)
    {
      printf("\n");
      actualChar = 0;
      actualColumn = 0;
    }
  }
}

void printField(char* str, int length, int columnSize)
{
    printf("%s ", str);

    int spaces = columnSize - length;
    for(int x = 0; x < spaces; x++) printf(" ");
}

void displayList(Fields **fields, int count, ColumnSizes *cSize, Args* args, PtShConfig *config)
{
  for(int i = 0; i < count; i++)
  {
    printField(fields[i]->name, fields[i]->nameLength, cSize->name);

    if(args->inode)
      printField(fields[i]->inode, strlen(fields[i]->inode), cSize->inode);

    printField(fields[i]->permissions, 0, 0);
    printField(fields[i]->uid, strlen(fields[i]->uid), cSize->uid);

    if(!args->noGroup)
      printField(fields[i]->gid, strlen(fields[i]->gid), cSize->gid);

    printField(fields[i]->date, 0, 0);

    if(args->fullTime || args->time)
      printField(fields[i]->time, 0,0);

    printField(fields[i]->hardlinks, strlen(fields[i]->hardlinks), cSize->hardlinks);

    printField(fields[i]->size, strlen(fields[i]->size), cSize->size);

    printf("\n");
  }
}

void display(Fields **fields, int count, Args* args, PtShConfig *config, ColumnSizes *cSize)
{
  if(args->l) displayList(fields, count, cSize, args, config);
  else displayBlock(fields, count, cSize, args, config); 
}


