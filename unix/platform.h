/* unix-specific includes and defines go in this file */
#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <unistd.h>

#define USE_READLINE
#define USE_STDIO	     // For interpret I/O
#define INCLUDE_LICENSE  // For -c option
#define HAVE_SETJUMP

#endif /* PLATFORM_H */
