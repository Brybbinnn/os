#include "run_program.h"

#define ERROR_CODE 255

int run_program(char *file_path, char *argv[])
{
    (void) file_path;
    (void) argv;

    /*
        Meant to check if *file_path variable contains
        any "/" characters and return a boolean value
        if it does.

        This also checks if the file addressed in the
        file_path variable is in the current directory
        because if the file wasn't in the CWD the path
        to it would have to contain a "/".
    */
    if (strchr(file_path, '/') != NULL) {
        if (execv(file_path, argv) == -1) {
            perror(ERROR_CODE);
            exit(ERROR_CODE);
        }
    } else {

        char *path = getenv("PATH"); // what is path bro
        if (path == NULL) {
            printf("Error");
        }
    }



    /*
        May be unnecesary. Or stupid.
    */
    if (file_path == 0) {
        return ERROR_CODE;                  // Special Error Value
    
    } else if (contains_slash == 0) {
        

    } else if (contains_slash == 1) {
        if (execv(file_path, argv) != -1)

    
    } else {                                // Every case above this checks
        return 0;                           // for errors, so this else runs
    }                                       // only when the function is successful.


    // remember to return ERROR_CODE on error.
    return ERROR_CODE;
}
