#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
  bool sName;
  bool sExtension;
  bool sTime;
}Args;

typedef struct Files_{
  char** names;
  int count;
}Files;

Args *parseArgs(int argc, char** argv);
Files *getFiles();
