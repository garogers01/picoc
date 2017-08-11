/* msvc-specific includes and defines go in this file */
#ifndef PLATFORM_H
#define PLATFORM_H

/* Sorry, but I can't test this.*/

#define ADD_UNISTD		  // Script files include unistd.h automatically.
#define USE_STDIO	     // For interpret I/O
#define INCLUDE_LICENSE  // For -c option
#define HAVE_SETJUMP


#endif /* PLATFORM_H */
