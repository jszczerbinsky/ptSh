#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include "ptls.h"
#include "argDefs.h"

void displayHelp()
{
  printf("Syntax: ptls [ARGS] [PATH]\n");
  printf("\nArguments:\n");
  printHelpLine(ALL_ARG, ALL_ARG_W, ALL_ARG_DESC);
  printHelpLine(ALMOST_ALL_ARG, ALMOST_ALL_ARG_W, ALMOST_ALL_ARG_DESC);
  printHelpLine(IGNORE_BACKUPS_ARG, IGNORE_BACKUPS_ARG_W, IGNORE_BACKUPS_ARG_DESC);
  printHelpLine(L_ARG, L_ARG_W, L_ARG_DESC);
  printf("\nSort arguments:\n");
  printHelpLine(S_X_ARG, S_X_ARG_W, S_X_ARG_DESC);
  printHelpLine(S_TIME_ARG, S_TIME_ARG_W, S_TIME_ARG_DESC);
  printHelpLine(REVERSE_ARG, REVERSE_ARG_W, REVERSE_ARG_DESC);
  printf("\nArguments only used with -l:\n");
  printHelpLine(INODE_ARG, INODE_ARG_W, INODE_ARG_DESC);
  printHelpLine(NO_GROUP_ARG, NO_GROUP_ARG_W, NO_GROUP_ARG_DESC);
  printHelpLine(TIME_ARG, TIME_ARG_W, TIME_ARG_DESC);
  printHelpLine(FULL_TIME_ARG, FULL_TIME_ARG_W, FULL_TIME_ARG_DESC);
  printHelpLine(FULL_SIZE_ARG, FULL_SIZE_ARG_W, FULL_SIZE_ARG_DESC);
  printHelpLine(DECIMAL_SIZE_ARG, DECIMAL_SIZE_ARG_W, DECIMAL_SIZE_ARG_DESC);
  printHelpLine(NUMERIC_UID_GID_ARG, NUMERIC_UID_GID_ARG_W, NUMERIC_UID_GID_ARG_DESC);
  printHelpLine(NO_DIRS_TOP_ARG, NO_DIRS_TOP_ARG_W, NO_DIRS_TOP_ARG_DESC);

}

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
			if(i != count-1)
				printf("\n");
      actualChar = 0;
      actualColumn = 0;
    }
  }
  printf("\n");
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


