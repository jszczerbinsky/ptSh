#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

#include "ptcp.h"

int main(int argc, char **argv)
{
  Args *args = parseArgs(argc, argv);

  if(args->help)
  {
    displayHelp();
    return 0;
  }

  PtShConfig *config = readConfig();

  if(args->sourcePath == NULL || args->destPath == NULL)
  {
    printMessage(config, "You must specify source and destination path", true);
    closeConfig(config);
    return 1;
  }
  
  struct termios oldt, newt;
  
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  MoveData *mData = getMoveData(args, config); 
  if(mData == NULL)
  {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    closeConfig(config);
    return 1;
  }

  if(args->interactive) selectFiles(config, mData);

  char buff[25];
  printSize(buff, 25, mData->totalBytes, args->decimalSize);

  printf("Copying %d files (%s)", mData->fileCount, buff);

  copyFiles(config, args, mData);
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  closeConfig(config);
  freeMoveData(mData);
  freeArgs(args);

  return 0;
}
