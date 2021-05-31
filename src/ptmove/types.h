#ifndef PTMOVE_TYPES_H
#define PTMOVE_TYPES_H

#include <stdbool.h>
#include <sys/stat.h>

typedef struct Args_{
  bool copy;
  bool recursive;
  bool interactive;
  bool update;
  char **sourcePath;
  int sourcePathCount;
  char *destPath;
} Args;

typedef struct Subdir_{
  char *name;
  mode_t mode;
} Subdir;

typedef struct FilePaths_{
  char *sourcePath;
  char *destPath;
} FilePaths;

typedef struct MoveData_{
  FilePaths **files;
  int fileCount;
  Subdir **subdirs;
  int subdirCount;
  unsigned long totalBytes;
} MoveData;

#endif
