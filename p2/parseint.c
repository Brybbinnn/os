#include "parseint.h"
#include <stdio.h>

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

int octalToDecimal(int n) {
    int decimalvalue = 0;
    int base = 1;
    int temp = n;
   
    while (temp) 
    {
        // Finding the last digit
        int lastdigit = temp % 10;
        temp = temp / 10;
        decimalvalue += lastdigit * base;
        base = base * 8;
    }
 
    return decimalvalue;
}


/*
 * Parses a non-negative integer, interpreted as octal when starting with 0,
 * decimal otherwise. Returns -1 on error.
 */
int parseInt(char *string) {
    int i, result = 0;

    for (i = 0; string[i] != '\0'; i++) {
        int digit = convertDecimalDigit(string[i]);

        if (digit == -1) {
            // Handle non-digit characters (e.g., '?')
            return -1;  // Or handle the error in another way
        }

        result = result * 10 + digit;
    }

    if (string[0] == '0') {
        return octalToDecimal(result);
    } else {
        return result;
    }
}




int main() {
    printf("%d", convertDecimalDigit('0'));
    printf("%s", "\nThe program finished running \"0\" through parseInt. \n");
    printf("%d", parseInt("010"));
    printf("%s", "\nThe program finished running \"010\" through parseInt. \n");
    printf("%d", parseInt("10"));
    printf("%s", "\nThe program finished running \"10\" through parseInt. \n");
    printf("%d", parseInt("?"));
    printf("%s", "\nThe program finished running \"?\" through parseInt. \n");
    printf("%d", convertDecimalDigit('9'));
    printf("%s", "\nThe program finished running \"9\" through parseInt. \n");
    printf("%d", parseInt("012"));
    printf("%s", "\nThe program finished running \"012\" through parseInt. \n");
    printf("%d", parseInt("350"));
    printf("%s", "\nThe program finished running \"350\" through parseInt. \n");
    printf("%d", parseInt("ö"));
    printf("%s", "\nThe program finished running \"ö\" through parseInt. \n");
}