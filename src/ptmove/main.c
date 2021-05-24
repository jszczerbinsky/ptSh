#include <stdlib.h>
#include <unistd.h>

#include "ptmove.h"

int main(int argc, char **argv)
{
  Args *args = parseArgs(argc, argv);

  free(args);
}
