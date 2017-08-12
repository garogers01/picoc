/* Dynamically create intXX_t and uintXX_t types */

#include <limits.h>
#include <stdint.h>
#include <ctype.h>

#include "../interpreter.h"
#include "../picoc.h"

/*
 * Using limits.h here is buggy. A user could include stdint.h then redefine
 * a name in limits.h that stdint.h is using, and get the wrong value. But
 * doing it right would require having two copies of everything. A minor
 * issue in a compiler, but not so much in an interpreter.
 */
static const char StaticDefs[] = "\
#include <limits.h>\n\
typedef long intmax_t;\n\
#define INTMAX_MIN LONG_MIN\n\
#define INTMAX_MAX LONG_MAX\n\
#define INTMAX_C(c) c ## L\n\
typedef unsigned long uintmax_t;\n\
#define UINTMAX_MAX LONG_MAX\n\
#define UINTMAX_C(c) c ## UL\n\
";

static const char format[] = "\
typedef signed %s int%s%d_t;\n\
#define INT%s%d_MIN %s_MIN\n\
#define INT%s%d_MAX %s_MAX\n\
typedef unsigned %s uint%s%d_t;\n\
#define UINT%s%d_MAX U%s_MAX\n\
";

static const char fn[] = "stdint.h";
#define LEN 2400

static void make_types(char *buffer, const char *type, int length, bool exact) {
     int i;
     char upper[10];	// Long Long some day?

     if (strcmp(type, "short") == 0) {
          strcpy(upper, "SHRT");
     } else {
          for (i = 0; type[i]; i += 1)
               upper[i] = toupper(type[i]);
          upper[i] = '\0';
     }

     if (exact) {
          snprintf(buffer + strlen(buffer), LEN - strlen(buffer), format,
                   type, "", length, "", length, upper, "", length, upper,
                   type, "", length, "", length, upper);
     }
     snprintf(buffer + strlen(buffer), LEN - strlen(buffer), format,
              type, "_fast", length, "_FAST", length, upper,
              "_FAST", length, upper,
               type, "_fast", length, "_FAST", length, upper);
     snprintf(buffer + strlen(buffer), LEN - strlen(buffer), format,
              type, "_least", length, "_LEAST", length, upper,
              "_LEAST", length, upper,
               type, "_least", length, "_LEAST", length, upper);
}

void StdintSetupFunc(Picoc *pc)
{
     char *typptr ;
     char buffer[LEN];

     strcpy(buffer, StaticDefs);

     /* The easy cases */
     make_types(buffer, "char", 8, CHAR_BIT == 8);
     make_types(buffer, "short", 16, sizeof(short) * CHAR_BIT == 16);

     if (sizeof(short) * CHAR_BIT >= 32 && sizeof(int) * CHAR_BIT >= 64) {
          make_types(buffer, "short", 32, sizeof(short) * CHAR_BIT == 32);
          make_types(buffer, "int", 64, sizeof(int) * CHAR_BIT == 64);
     } else {
          if (sizeof(int) * CHAR_BIT >= 32) {
               make_types(buffer, "int", 32, sizeof(int) * CHAR_BIT == 32);
          } else if (sizeof(long) * CHAR_BIT >= 32) {
               make_types(buffer, "long", 32, false);
          }
          if (sizeof(long) * CHAR_BIT >= 64) {
               make_types(buffer, "long", 64, sizeof(int) * CHAR_BIT == 64);
          }
     }

     typptr = sizeof(short) >= sizeof(void *) ? "short"
           : sizeof(int) >= sizeof(void *) ? "int" : "long";
     snprintf(buffer + strlen(buffer), LEN - strlen(buffer),
              "typedef %s intptr_t;\n", typptr);
     snprintf(buffer + strlen(buffer), LEN - strlen(buffer),
              "typedef unsigned %s uintptr_t;\n", typptr);

     PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false);
}
