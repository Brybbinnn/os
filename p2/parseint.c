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
    
    // Initializing values
    int decimalvalue = 0;
    int base = 1;
    int temp = n;
   
    while (temp) 
    {
        int lastdigit = temp % 10;          // Find the last digit of temp
        temp = temp / 10;                   // Remove the last digit from temp
        decimalvalue += lastdigit * base;   // Convert and add the last digit to the decimal value
        base = base * 8;                    // Update the base value for the next digit
    }
 
    return decimalvalue;
}


/*
 * Parses a non-negative integer, interpreted as octal when starting with 0,
 * decimal otherwise. Returns -1 on error.
 */
int parseInt(char *string) {

    // Initializing values
    int i, result = 0;

    for (i = 0; string[i] != '\0'; i++) {
        int digit = convertDecimalDigit(string[i]);     // Convert character to digit

        if (digit == -1) {
            return -1;                                  // Handle non-digit characters by returning an error (-1)
        }

        result = result * 10 + digit;                   // Build the result by appending the converted digit
    }

    if (string[0] == '0') {
        return octalToDecimal(result);                  // If the first character is '0', interpret the result as octal and convert
    } else {
        return result;                                  // Otherwise, return the result as a decimal integer
    }
}