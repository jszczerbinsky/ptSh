#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ptsh.h>

#ifndef PTLS_H
#define PTLS_H

typedef struct Args_{

  //Path
  char* dirPath;

  //Files
  bool all;
  bool almostAll;
  bool ignoreBackups;

  //List
  bool l;

  //Fields
  bool author;
  bool inode;
  bool noGroup;

  //Fields options
  bool fullTime;
  bool fullSize;
  bool decimalSize;
  bool numericUidGid;

  //Group
  bool noDirsTop;

  //Sort
  bool reverse;
  bool sExtension;
  bool sTime;
}Args;

typedef struct FileInstance_{
  char* name;
  struct stat* stats;
}FileInstance;

typedef struct Files_{
  FileInstance** instances;
  int count;
}Files;

Args *parseArgs(int argc, char** argv);
Files *getFiles();
void sort(FileInstance **instances, Args* args, int low, int high);
void display(FileInstance **files, int count, Args* args, PtShConfig *config,  int longestName, int* actualColumn, int* actualChar);

#endif
