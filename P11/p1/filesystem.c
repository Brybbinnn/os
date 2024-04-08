/* 
 * Group number (on canvas): 66
 * Student 1 name: Bryndís Gunnarsdóttir
 * Student 2 name: Viggó Ýmir Hafliðason
 */


#include "filesystem.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

FileSystem *initFileSystem(char *diskFile)
{
	if (diskFile == NULL) {
		return NULL;
	}

	// Implement a function that opens a full disk image

	// allocate a file system data structure
	FileSystem *fs = malloc(sizeof(FileSystem));
	// read and store in memory the full header and FAT from the disk image
	// return a reference to the data structure


	// ----------------
	// open file system image and read header + FAT into a FileSystemHeader data structure
	// (all memory allocated dynamically with malloc)^
	// ----------------


	return NULL;
}

// Create a file handle representing a file that is part of a specific file system (fs),
// starting at block (blockIndex) in that file system, and a file length (length) in bytes
// You can make use of this function in your code....
static OpenFileHandle *_openFileAtBlock(FileSystem *fs, uint32_t blockIndex,
	uint32_t length)
{
	assert(fs != NULL);
	assert(blockIndex < fs->header->fsBlocks);

	OpenFileHandle *handle = malloc(sizeof(OpenFileHandle));
	if (handle == NULL) {
		return NULL;
	}

	handle->fileSystem		= fs;
	handle->currentBlock	  = blockIndex;
	handle->currentFileOffset = 0;
	handle->length			= length;

	return handle;
}

static int _hasMoreBytes(OpenFileHandle *handle)
{
	(void)handle;

	// ----------------
	// Check if there are more bytes to read in the file.
	// ----------------

	return 0;
}

#define INVALID_BLOCK_INDEX (uint32_t)-1


int _findDirectoryEntry(OpenFileHandle *dir, char *name, DirectoryEntry *dirEntry)
{
	// TODO: Implement
	(void)dir;
	(void)name;
	(void)dirEntry;

	return -1;
}

OpenFileHandle *openFile(FileSystem *fs, char *dir, char *name)
{
	if ((fs == NULL) || (name == NULL)) {
		return NULL;
	}

	// Open the root directory file.
	OpenFileHandle *root = _openFileAtBlock(fs, ROOT_DIRECTORY_BLOCK,
						fs->header->rootDirectorySize);
	if (root == NULL) {
		return NULL;
	}
  
	// ----------------
	// For the bonus part only: If dir is not NULL:
	// find the directory (in the root directory) with that name
	// open that directory, and use that instead of root for searching the file name
	// ----------------
	(void)dir;

	// ----------------
	// Find the directory entry with that name.
	// You can use readFile to read from the directory stream.
	// ----------------

	closeFile(root);

	// ----------------
	// Return a file handle if the file could be found.
	// ----------------
	return NULL;

}

void closeFile(OpenFileHandle *handle)
{
	if (handle == NULL) {
		return;
	}

	// Since we opened the file system with read access only, we do not have
	// any pending modifications that might need to be written back to the file
	// prior closing. Hence, we can just free the handle and call it a day.
	free(handle);
}

char _readFileByte(OpenFileHandle *handle)
{
	(void)handle;

	// ----------------
	// Read a byte from the file. This should never fail, because the function
	// must not be called if there are not more bytes to read.
	// ----------------

	return 0;
}

int readFile(OpenFileHandle *handle, char *buffer, int length)
{
	if ((handle == NULL) || (buffer == NULL)) {
		return -1;
	}

	int n = 0;
	while ((n < length) && _hasMoreBytes(handle)) {
		buffer[n] = _readFileByte(handle);

		++n;
	}

	return n;
}
