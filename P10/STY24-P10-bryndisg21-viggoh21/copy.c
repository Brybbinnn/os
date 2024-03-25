/* 
 * Group number (on canvas): 66
 * Student 1 name: Bryndís Gunnarsdóttir
 * Student 2 name: Viggó Ýmir Hafliðason
 */

#define _POSIX_C_SOURCE 2
#include "copy.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>


#include <string.h>


// No need to change this. Parses argc into the CopyArgs structure
int parseCopyArgs(int argc, char * const argv[], CopyArgs* args)
{
	if (args == NULL) {
		return -1;
	}

	// Initialize with default values
	args->blocksize  = 4096;

	int opt;
	while ((opt = getopt(argc, argv, "b:")) != -1) {
		switch (opt) {
			case 'b':
				args->blocksize = atoi(optarg);

				if ((errno != 0) || (args->blocksize <= 0)) {
					return -1; // Invalid blocksize argument.
				}

				break;

			default: /* '?' */
				return -1;
		}
	}

	if (optind != argc - 2) {
		return -1; // We expect two parameters after the options.
	}

	args->from = argv[optind];
	args->to   = argv[optind + 1];

	return 0;
}

int doCopy(CopyArgs* args) {
    if (args == NULL) {
        return -1;
    }

    // Open the source file for reading
    int src_fd = open(args->from, O_RDONLY);
    if (src_fd == -1) {
        return -1;
    }

    // Get the permissions of the source file
    struct stat src_stat;
    if (fstat(src_fd, &src_stat) == -1) {
        close(src_fd);
        return -1;
    }

    // Open the destination file with the same permissions as the source file
    int dest_fd = open(args->to, O_WRONLY | O_CREAT | O_EXCL, src_stat.st_mode & 07777);
    if (dest_fd == -1) {
        close(src_fd);
        return -1;
    }

    // Allocate a buffer to read from the source file
    char *buffer = malloc(args->blocksize);
    if (buffer == NULL) {
        close(src_fd);
        close(dest_fd);
        return -1;
    }

    // Read from the source file and write to the destination file
    ssize_t bytes_read, i;
    off_t offset = 0;
    while ((bytes_read = read(src_fd, buffer, args->blocksize)) > 0) {
        for (i = 0; i < bytes_read; ++i) {
            if (buffer[i] == '\0') {
                ++offset;
            } else {
                // If there were zero bytes before, we need to write them to the destination file
                if (offset != 0) {
                    if (lseek(dest_fd, offset, SEEK_CUR) == -1) {
                        close(src_fd);
                        close(dest_fd);
                        free(buffer);
                        return -1;
                    }
                    offset = 0;
                }
                // Write the byte to the destination file
                if (write(dest_fd, &buffer[i], 1) != 1) {
                    close(src_fd);
                    close(dest_fd);
                    free(buffer);
                    return -1;
                }
            }
        }
    }

    // If the last byte read was a zero byte, we need to write it to the destination file
    if (offset != 0) {
        if (lseek(dest_fd, offset - 1, SEEK_CUR) == -1) {
            close(src_fd);
            close(dest_fd);
            free(buffer);
            return -1;
        }
        // Write a single zero byte to the destination file
        if (write(dest_fd, "\0", 1) != 1) {
            close(src_fd);
            close(dest_fd);
            free(buffer);
            return -1;
        }
    }

    // Close both files and free memory

    free(buffer);
    close(src_fd);
    close(dest_fd);

    // Check if the read operation failed
    if (bytes_read == -1) {
        return -1;
    }

    return 0;
}
