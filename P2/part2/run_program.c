#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
    RETURNS ERROR VALUE 255 TO REPORT AN ERROR CONDITION AND 0 TO INDICATE
    SUCCESS

    IF FILE_PATH IS NULL YOU SHOULD RETURN THE SPECIAL ERROR VALUE (255)
    IF ARGS IS NULL YOU SHOULD RUN THE PROGRAM WITHOUT ARGUMENTS
    THE PROGRAM STARTER(RUN_PROGRAM) SHOULD NOT BE TERMINATED BY AN EXCEPTION
    IN EITHER OF THE TWO CASES ABOVE

    
    MODIFY THE STARTER(RUN_PROGRAM) TO RETURN THE EXIT STATUS OF THE PREVIOUSLY 
    STARTED/EXITED PROCESS
    KEEP A RETURN VALUE OF 255 TO INDICATE ERROR CONDITIONS IN YOUR OWN PROGRAM
*/



int run_program(char *file_path, char *argv[])
{
    (void) file_path;
    (void) argv;

    if (file_path == NULL) {
        return 255;
    }

    // Check if the program contains a path separator ("/"), indicating a full path
    pid_t pid = fork();
    if ((WIFSTOPPED(pid))) {
            exit(255);
        }
    if (pid == 0) { /* child process */
        if (1 == 1) {
            // If a full path is provided, directly execute the program
            int code = execv(file_path, argv);
            if (code != 0) {
                exit(255);
            } else {
                execv(file_path, argv);
                exit(0);
            }
        } else {
            // If only the name is provided, search for the program in PATH directories
            char *path = getenv("PATH");
            if (path == NULL) {
                exit(255);
            }
            else {
                execvp(path, argv);
                exit(0);
            }
        }
    }
    else { /* pid != 0; parent process */
        int code = waitpid(pid, 0, 0);
        if ((WIFSTOPPED(code))) {
            exit(255);
        }
    }
    return 255;
}