#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


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