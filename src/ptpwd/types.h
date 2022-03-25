#ifndef PTPWD_TYPES
#define PTPWD_TYPES

#include <stdbool.h>

typedef struct Args_{
  bool help;
  bool logical;
}Args;

typedef struct PathDirs_{
  char **names;
  int count;
}PathDirs;

#endif
