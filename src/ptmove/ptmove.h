#ifndef PTMOVE_H
#define PTMOVE_H

#include "types.h"

Args *parseArgs(int argc, char **argv);
MoveData *getMoveData(Args *args);
void freeMoveData(MoveData *data);

#endif
