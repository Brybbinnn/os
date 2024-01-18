#include "parseint.h"
#include <stdio.h>
#include <string.h>

/*
 * Returns the value of c or -1 on error
 */
int convertDecimalDigit(char c)
{
    (void)c;                  
    if (c >= '0' && c <= '9') {
        // Convert char to integer and return
        return c - '0';
    } else {
        // Not a valid decimal digit, return -1
        return -1;
    }
}

/*
 * Parses a non-negative integer, interpreted as octal when starting with 0,
 * decimal otherwise. Returns -1 on error.
 */
int parseInt(char *string)
{
    (void)string;

    int octal_sum = 0;

    if (string[0] == '0'){
        for (int i = 0; i = strlen(string); i++) {
            octal_sum += string[i];
        }
        printf("%d", octal_sum);
    } else {
        printf("%d", string);
    }
    return -1;
}

int main(void) {
   printf("%d", convertDecimalDigit('?'));
   printf("%s", "The program finished running \"?\" through convertDecimalDigit. \n");
   printf("%d", parseInt("010"));
   printf("%s", "The program finished running \"010\" through parseInt. \n");
   printf("%d", parseInt("10"));
   printf("%s", "The program finished running \"10\" through parseInt. \n");
   printf("%d", parseInt("?"));
   printf("%s", "The program finished running \"?\" through parseInt. \n");
}