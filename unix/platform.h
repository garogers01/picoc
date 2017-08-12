/* unix-specific includes and defines go in this file */
#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <unistd.h>

#define USE_STDIO	     // For interpreter I/O

#define USE_READLINE
#define INCLUDE_LICENSE  // For -c option

#endif /* PLATFORM_H */
