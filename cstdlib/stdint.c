/* Dynamically create intXX_t and uintXX_t types */

#include <limits.h>
#include <stdint.h>

#include "../interpreter.h"
#include "../picoc.h"

/* INT8_MIN */
/* INT8_MAX */
/* INT_FAST8_MIN */
/* INT_FAST8_MAX */
/* INT_LEAST8_MIN */
/* INT_LEAST8_MAX */
/* UINT8_MIN */
/* UINT8_MAX */
/* UINT_FAST8_MIN */
/* UINT_FAST8_MAX */
/* UINT_LEAST8_MIN */
/* UINT_LEAST8_MAX */
/* INT16_MIN */
/* INT16_MAX */
/* INT_FAST16_MIN */
/* INT_FAST16_MAX */
/* INT_LEAST16_MIN */
/* INT_LEAST16_MAX */
/* UINT16_MIN */
/* UINT16_MAX */
/* UINT_FAST16_MIN */
/* UINT_FAST16_MAX */
/* UINT_LEAST16_MIN */
/* UINT_LEAST16_MAX */
/* INT32_MIN */
/* INT32_MAX */
/* INT_FAST32_MIN */
/* INT_FAST32_MAX */
/* INT_LEAST32_MIN */
/* INT_LEAST32_MAX */
/* UINT32_MIN */
/* UINT32_MAX */
/* UINT_FAST32_MIN */
/* UINT_FAST32_MAX */
/* UINT_LEAST32_MIN */
/* UINT_LEAST32_MAX */
/* INT64_MIN */
/* INT64_MAX */
/* INT_FAST64_MIN */
/* INT_FAST64_MAX */
/* INT_LEAST64_MIN */
/* INT_LEAST64_MAX */
/* UINT64_MIN */
/* UINT64_MAX */
/* UINT_FAST64_MIN */
/* UINT_FAST64_MAX */
/* UINT_LEAST64_MIN */
/* UINT_LEAST64_MAX */


const char *fn = "stdint.h";
#define LEN 40
static void make_types(Picoc *pc, const char *type, int length, bool exact) {
     char buffer[LEN];

     if (exact) {
          snprintf(buffer, LEN, "typedef signed %s int%d_t;", type, length);
          PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false); 
          snprintf(buffer, LEN, "typedef unsigned %s uint%d_t;", type, length);
          PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false); 
     }
     snprintf(buffer, LEN, "typedef signed %s int_fast%d_t;", type, length);
     PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false); 
     snprintf(buffer, LEN, "typedef unsigned %s uint_fast%d_t;", type, length);
     PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false); 
     snprintf(buffer, LEN, "typedef signed %s int_least%d_t;", type, length);
     PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false); 
     snprintf(buffer, LEN, "typedef unsigned %s uint_least%d_t;", type, length);
     PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false); 
}
     
const char StdintDefs[] = "\
typedef long intmax_t;\
typedef unsigned long uintmax_t;\
";

void StdintSetupFunc(Picoc *pc)
{
     char *typptr ;
     char buffer[LEN];

     /* The easy cases */
     make_types(pc, "char", 8, CHAR_BIT == 8);
     make_types(pc, "short", 16, sizeof(short) * CHAR_BIT == 16);

     typptr = sizeof(short) >= sizeof(void *) ? "short"
           : sizeof(int) >= sizeof(void *) ? "int" : "long";
     snprintf(buffer, LEN, "typedef %s intptr_t;", typptr);
     PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false);
     snprintf(buffer, LEN, "typedef unsigned %s uintptr_t;", typptr);
     PicocParse(pc, fn, buffer, strlen(buffer), true, false, false, false);
     
     if (sizeof(short) * CHAR_BIT >= 32 && sizeof(int) * CHAR_BIT >= 64) {
          make_types(pc, "short", 32, sizeof(short) * CHAR_BIT == 32);
          make_types(pc, "int", 64, sizeof(int) * CHAR_BIT == 64);
     } else {
          if (sizeof(int) * CHAR_BIT >= 32) {
               make_types(pc, "int", 32, sizeof(int) * CHAR_BIT == 32);
          } else if (sizeof(long) * CHAR_BIT >= 32) {
               make_types(pc, "long", 32, false);
          }
          if (sizeof(long) * CHAR_BIT >= 64) {
               make_types(pc, "long", 64, sizeof(int) * CHAR_BIT == 64);
          }
     }
}
