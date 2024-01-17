#include "print.h"
#include <stdio.h>

void print_line(int64_t number, char *string)
{
    (void) number;
    (void) string;
    // Add code here.
    
    printf("%d %s", number, string);
}

int main(void) {

    print_line(42, "hello");
}
