#include "run_program.h"

#define ERROR_CODE 255

int run_program(char *file_path, char *argv[])
{
    (void) file_path;
    (void) argv;

    /*
        Declared as _Bool because bool alias needs to
        be imported with <stdbool.h>.

        All values besides 0 are stored in _Bool as
        True. 
    */
    _Bool contains_slash = 0;

    /*
        Meant to check if *file_path variable contains
        any "/" characters and return a boolean value
        if it does.

        This also checks if the file addressed in the
        file_path variable is in the current directory
        because if the file wasn't in the CWD the path
        to it would have to contain a "/".
    */
    for (int i = 0; i != "\0"; i++){
        if (file_path[i] == "/"){
            contains_slash = 1;
        }
    }


    /*
        May be unnecesary. 
    */
    if (contains_slash == 0) {
        fopen(file_path, "r");

    } else if (contains_slash == 1) {
        fopen(file_path, "r");
    
    }


    // remember to return ERROR_CODE on error.
    return ERROR_CODE;
}
