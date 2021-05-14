#include <sys/ioctl.h>
#include <sys/stat.h>

#include "ptls.h"

int main(int argc, char** argv)
{
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w); 

  Args *args = parseArgs(argc, argv);

  Files *files = getFiles();

  struct stat stats[files->count];
  for(int i = 0; i < files->count; i++)
    stat(files->names[i], &stats[i]);

  free(files);
  free(args);

  return 0;
}
