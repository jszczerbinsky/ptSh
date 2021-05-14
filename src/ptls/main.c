#include <sys/ioctl.h>

#include "ptls.h"

int main(int argc, char** argv)
{
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w); 

  Args *args = parseArgs(argc, argv);

  free(args);
}
