#include <stdio.h>

// variables
long a = 12 + 56;
printf("%ld\n", a);

// infix operators
printf("%d\n", 12 + 56);
printf("%d\n", 12 - 56);
printf("%d\n", 12 * 56);
printf("%d\n", 12 / 56);

// comparison operators
printf("%d %d %d %d %d %d\n", 12 < 56, 12 <= 56, 12 == 56, 12 >= 56, 12 > 56, 12 != 56);
printf("%d %d %d %d %d %d\n", 12 < 12, 12 <= 12, 12 == 12, 12 >= 12, 12 > 12, 12 != 12);
printf("%d %d %d %d %d %d\n", 56 < 12, 56 <= 12, 56 == 12, 56 >= 12, 56 > 12, 56 != 12);

// assignment operators
a = 12;
a += 56;
printf("%d\n", a);

a = 12;
a -= 56;
printf("%d\n", a);

a = 12;
a *= 56;
printf("%d\n", a);

a = 12;
a /= 56;
printf("%d\n", a);

// prefix operators
printf("%d\n", +12);
printf("%d\n", -12);


void main() {}
