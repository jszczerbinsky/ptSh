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

void displayList(Fields **fields, int count, ColumnSizes *cSize, Args* args, PtShConfig *config)
{
  for(int i = 0; i < count; i++)
  {
    printf("%s", fields[i]->name);

    int spaces = cSize->name+1 - fields[i]->nameLength;
    for(int x = 0; x < spaces; x++) printf(" ");

    printf("%s %s", fields[i]->permissions, fields[i]->uid);

    spaces = cSize->uid+1 - strlen(fields[i]->uid);
    for(int x = 0; x < spaces; x++) printf(" ");

    if(!args->noGroup)
      printf("%s", fields[i]->gid);

    printf("\n");
  }
}

void display(Fields **fields, int count, Args* args, PtShConfig *config, ColumnSizes *cSize)
{
  if(args->l) displayList(fields, count, cSize, args, config);
  else displayBlock(fields, count, cSize, args, config); 
}


