#include "print.h"
#include <stdio.h>

void print_line(int64_t number, char *string)
{
    (void) number;
    (void) string;
    
    printf("%lld %s", number, string);
}