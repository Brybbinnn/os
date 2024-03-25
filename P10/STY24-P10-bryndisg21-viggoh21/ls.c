/* 
 * Group number (on canvas): 66
 * Student 1 name: Bryndís Gunnarsdóttir 
 * Student 2 name: Viggó Ýmir Hafliðason
 */

#include "ls.h"

// You may not need all these headers ...
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>


int list(const char* path, int recursive) {

	DIR *dir;
	struct dirent *item;
	struct stat info;
	char fullpath[1024];
	//   Return 0 on success, -1 otherwise (in case of any error).

	dir = opendir(path);
	if (dir == NULL) {
		return -1;
	}
		//  Iterate over all files in the given directory using the opendir(), readdir(), and closedir() system calls.1
	while ((item = readdir(dir)) != NULL) {
		
		//  Skip all hidden entries (i.e., file names that start with '.')
		if (item->d_name[0] == '.') {
			continue;
		} 
		
		// Concatenates the item currently being iterated through to the fullpath C string
		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, item->d_name);

		if (lstat(fullpath, &info) == -1) {
			return -1;
		}

		//  The typestr parameter to printLine shall be   /   for directories,   |   for pipes,   *   for executable programs. For symbolic links, it should be the string    ->    and the target file name of the symbolic link.
		const char* typestr = " ";
        if (S_ISDIR(info.st_mode)) typestr = "/";
        else if (S_ISFIFO(info.st_mode)) typestr = "|";
        else if (S_ISLNK(info.st_mode)) {
            char target[1024];
            ssize_t len = readlink(fullpath, target, sizeof(target) -1);
            if (len != -1) {
                target[len] = '\0'; // Null-terminate the target
            }
			
			char targetfile[1024];
			strcpy(targetfile, " -> ");
			strcat(targetfile, target);
			typestr = targetfile;

        }
        else if (info.st_mode & S_IXUSR) typestr = "*";

		//  Print the gathered information via the template's printLine() function. You shall print the complete file name with the path (if path is   p1   and readdir returns a name   ls.c  , you shall print   p1/ls.c  )
		_printLine(info.st_size, fullpath, typestr);

		//  If the recursive flag is set, and the current item is a directory, call list() recursively with the full path of the directory.
		if (recursive && S_ISDIR(info.st_mode)) {
            list(fullpath, recursive);
        }
	}

	closedir(dir);
	return 0;
}
