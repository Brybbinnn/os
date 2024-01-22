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
    (void)string;

    int i, result = 0;
    char ans[3] = {};

    for (int i = 0; string[i] != '\0'; i++) {
        result = result * 10 + (string[i] - '0');
    }

    if (string[0] == '0') {
        return octalToDecimal(result);
    } else {
        for (int i = 0; string[i] != '\0'; i++) {
            ans[i] = convertDecimalDigit(string[i]);
        }
        return *ans;
    }
}


int main() {
    printf("%d", parseInt("010"));
    printf("%s", "The program finished running \"010\" through parseInt. \n");
    printf("%d", parseInt("10"));
    printf("%s", "The program finished running \"10\" through parseInt. \n");
    printf("%d", parseInt("?"));
    printf("%s", "The program finished running \"?\" through parseInt. \n");
}
