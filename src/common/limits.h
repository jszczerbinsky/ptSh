#if  defined(__linux__)
  #include <linux/limits.h>
#elif defined(__NetBSD__)
  #include <limits.h>
#elif defined(_APPLE__)
  #include <sys/syslimits.h>
#else
  #include <limits.h>
#endif
