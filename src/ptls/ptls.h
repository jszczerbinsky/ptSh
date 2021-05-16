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

typedef struct ColumnLengths_{
  int name;
  int uid;
  int gid;
  int inode;
  int size;
  int hardLinks;
}ColumnLengths;

typedef struct PrintFileData_{
  int nameLength;
  char *name;
  char *permissions;
  char *uid;
  char *gid;
  char *auhor;
  char *date;
  char *time;
  int inode;
  int hardlinks;
  char *size;
}PrintFileData;

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
void display(PrintFileData **files, int count, Args* args, PtShConfig *config, ColumnLengths *lengths);
void setPrintData(PrintFileData *printData, FileInstance *instance, PtShConfig *config, Args *args, ColumnLengths *lengths);

#endif
