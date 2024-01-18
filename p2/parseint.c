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

/*
 * Parses a non-negative integer, interpreted as octal when starting with 0,
 * decimal otherwise. Returns -1 on error.
 */
int parseInt(char *string) {
    (void)string;

    const char *p = string;

    int string_array[4] = {};

    if (p[0] == '0') {
        printf("%g", "bitch if");
        int octalToDecimal(int string);

    } else if (p[0] != '0') {
        printf("%g", "bitch else if");
        for (int i = 0; i != '\0'; i++) {
            string_array[i] = convertDecimalDigit(i);
        }
    }
}

//     int octal_sum = 0;

//     if (string[0] == '0'){
//         for (int i = 0; string[i] != '\0'; i++) {
//             if (string[i] == '0'){
//                 octal_sum += 0
//             }
//         }

//     } else if (string[0] != '0') {
//         printf("%d", string);
//     } else {
//         return -1;
//     }

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
 


int main(void) {
    printf("%d", convertDecimalDigit('0'));
    printf("%s", "The program finished running \"0\" through convertDecimalDigit. \n");
    printf("%d", parseInt("010"));
    printf("%s", "The program finished running \"010\" through parseInt. \n");
    printf("%d", parseInt("10"));
    printf("%s", "The program finished running \"10\" through parseInt. \n");
    printf("%d", parseInt("?"));
    printf("%s", "The program finished running \"?\" through parseInt. \n");
}