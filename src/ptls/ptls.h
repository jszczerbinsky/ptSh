#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ptsh.h>
#include "types.h"

#ifndef PTLS_H
#define PTLS_H

//argParser.c
Args *parseArgs(int argc, char** argv);

//files.c
DirContent *getFiles();
void freeContent(DirContent *dirContent);

//fields.c
void freeFields(Fields *fields);
void fillFields(Fields *printData, File *file, PtShConfig *config, Args *args, ColumnSizes *sizes);

//sort.c
void sort(File **file, Args* args, int low, int high);

//display.c
void display(Fields **files, int count, Args* args, PtShConfig *config, ColumnSizes *sizes);
void displayHelp();

#endif
