#ifndef PTMOVE_H
#define PTMOVE_H

#include "types.h"

//argParser.c
Args *parseArgs(int argc, char **argv);
void freeArgs(Args *args);

//fileScanner.c
MoveData *getMoveData(Args *args);
void freeMoveData(MoveData *data);

//display.c
void setProgressBar(int size, int percentage);

//files.c
void copyFiles(Args *args, MoveData *mData);

#endif
