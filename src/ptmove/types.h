#ifndef PTMOVE_TYPES_H
#define PTMOVE_TYPES_H

#include <stdbool.h>

typedef struct Args_{
  bool copy;
  bool recursive;
  bool interactive;
  bool update;
  char *sourcePath;
  char *destPath;
} Args;

typedef struct FilePaths_{
  char *sourcePath;
  char *destPath;
} FilePaths;

typedef struct MoveData_{
  FilePaths **files;
  int fileCount;
  int totalBytes;
} MoveData;

#endif
