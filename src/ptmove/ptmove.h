#ifndef PTMOVE_H
#define PTMOVE_H

#include "types.h"

Args *parseArgs(int argc, char **argv);
void freeArgs(Args *args);
MoveData *getMoveData(Args *args);
void freeMoveData(MoveData *data);

#endif
