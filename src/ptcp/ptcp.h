#ifndef PTCP_H
#define PTCP_H

#include "../common/configParser.h"
#include "../common/help.h"
#include "types.h"

//argParser.c
Args *parseArgs(int argc, char **argv);
void freeArgs(Args *args);

//fileScanner.c
MoveData *getMoveData(Args *args, PtShConfig *config);
void freeMoveData(MoveData *data);

//display.c
void setProgressBar(int size, int percentage);
void displayHelp();

//files.c
void copyFiles(Args *args, MoveData *mData);

//fileSelector.c
void selectFiles(PtShConfig *config, MoveData *mData);

#endif
