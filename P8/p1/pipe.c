// This changes the way some includes behave.
// This should stay before any include.
#define _GNU_SOURCE

#include "pipe.h"
#include <sys/wait.h> /* For waitpid */
#include <unistd.h> /* For fork, pipe */
#include <stdlib.h> /* For exit */
#include <stdio.h>
#include <string.h>


char *get_output(char *argv[])
{

    int pipes[2];

    if (pipe(pipes) == -1){
        perror("pipe failed");
        return NULL;
    }

    // Create child process
    int child_pid = fork();
    if (child_pid == -1) {
        perror("fork failed");

        return NULL;
    } else if (child_pid == 0) {
        // Replace program

        close(pipes[0]);
        dup2 (pipes[1], STDOUT_FILENO); //lausn við einu test case(punktur 6 í a)
        close(pipes[1]);

        execvp(argv[0], argv);


	perror("execvp failed");
	// The child process must terminate if execp failed!
        exit(255);
    } else {


        close(pipes[1]);

        char buffer[1024];
        ssize_t bytes_read;
        size_t total_bytes_read = 0;

        while ((bytes_read = read(pipes[0], buffer+total_bytes_read, sizeof(buffer)-total_bytes_read)) > 0) {
            total_bytes_read += bytes_read;
        
            if (memchr(buffer, '\n', total_bytes_read) != NULL) {
                break;
            }
        }
        
        if (bytes_read == -1) {
            perror("read failed");
            close(pipes[0]);
            return NULL;
        }

        char *line = malloc(total_bytes_read + 1) ;
        if (line == NULL) {
            perror("malloc failed");
            close(pipes[0]);
            return NULL;
        }

        memcpy(line, buffer, total_bytes_read);
        line[total_bytes_read] = '\0';
        close(pipes[0]);

	// let's wait for the child to terminate
	    int status;
        waitpid(child_pid, &status, 0);
	
        close(pipes[1]);

    //    char *ptr = strdup();

    
        return line;
    }
}