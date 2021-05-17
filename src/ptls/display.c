#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include "ptls.h"
#include "argDefs.h"

void printHelpLine(char arg, const char* argw, const char* desc, bool sort)
{
  const int argwColSize = 20;
  printf("     ");

  if(arg == ' ')
    printf("   ");
  else
    printf("-%c ", arg);

  printf(" ");

  printf("%s  ", argw);
  for(int i = 0; i < argwColSize - strlen(argw); i++) printf(" ");
  printf("%s\n", desc);
}

void displayHelp()
{
  printf("Syntax: ptls [ARGS] [PATH]\n");
  printf("\nArguments:\n");
  printHelpLine(ALL_ARG, ALL_ARG_W, "Displays all files in directory", false);
  printHelpLine(ALMOST_ALL_ARG, ALMOST_ALL_ARG_W, "Displays all files in directory, except of . and ..", false);
  printHelpLine(IGNORE_BACKUPS_ARG, IGNORE_BACKUPS_ARG_W, "Ignores files, that ends with ~", false);
  printHelpLine(L_ARG, "", "Displays more informations about a file", false);
  printHelpLine(REVERSE_ARG, REVERSE_ARG_W, "Reverses sorting order", false);
  printf("\nSort arguments:\n");
  printHelpLine(S_X_ARG, "", "Sort by extension instead of name", true);
  printHelpLine(S_TIME_ARG, "", "Sort by last modification time instead of name", true);
  printf("\nArguments only used with -l:\n");
  printHelpLine(INODE_ARG, INODE_ARG_W, "Shows file inode", false);
  printHelpLine(NO_GROUP_ARG, NO_GROUP_ARG_W, "Ignores information about group", false);
  printHelpLine(TIME_ARG, "", "Shows last modifiaction time as HH:MM", false);
  printHelpLine(FULL_TIME_ARG, FULL_TIME_ARG_W, "Shows last modification time as HH:MM:SS", false);
  printHelpLine(FULL_SIZE_ARG, FULL_SIZE_ARG_W, "Shows file size in bytes", false);
  printHelpLine(DECIMAL_SIZE_ARG, DECIMAL_SIZE_ARG_W, "Shows size in decimal units instead of binary", false);
  printHelpLine(NUMERIC_UID_GID_ARG, NUMERIC_UID_GID_ARG_W, "Displays UID and GID numbers instead of names", false);
  printHelpLine(' ', NO_DIRS_TOP_ARG_W, "Don't group directories on top", false);

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


