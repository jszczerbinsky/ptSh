#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef struct Args_{

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

typedef struct Files_{
  char** names;
  int count;
}Files;

typedef struct FileInstance_{
  char* name;
  struct stat* stats;
}FileInstance;

Args *parseArgs(int argc, char** argv);
Files *getFiles();
void sort(FileInstance **instances, Args* args, int low, int high);
