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

FileSystem *initFileSystem(char *diskFile) {
    if (diskFile == NULL) {
        return NULL;
    }

    int fd = open(diskFile, O_RDONLY);
    if (fd == -1) {
        perror("Error opening disk image");
        return NULL;
    }

    // Calculate the fixed size of the header by considering the size of the structure with one block_t
    size_t fixedHeaderSize = sizeof(FileSystemHeader) - sizeof(block_t);

    FileSystemHeader tmpHeader;
    ssize_t bytesRead = read(fd, &tmpHeader, fixedHeaderSize);
    if (bytesRead == -1) {
        perror("Error reading initial part of file system header");
        close(fd);
        return NULL;
    }
    if ((size_t)bytesRead != fixedHeaderSize) {
        fprintf(stderr, "Incomplete read of the file system header\n");
        close(fd);
        return NULL;
    }

    size_t fatSize = sizeof(block_t) * tmpHeader.fsBlocks;
    int headerSize = fixedHeaderSize + fatSize;

    FileSystemHeader *header = (FileSystemHeader *)malloc(headerSize);
    if (header == NULL) {
        perror("Memory allocation error for FileSystemHeader");
        close(fd);
        return NULL;
    }

    // Copy the already read part of the header
    memcpy(header, &tmpHeader, fixedHeaderSize);

    // Read the FAT into the allocated header memory
    bytesRead = read(fd, header->fat, fatSize);
    if (bytesRead == -1) {
        perror("Error reading FAT");
        close(fd);
        free(header);
        return NULL;
    }
    if ((size_t)bytesRead != fatSize) {
        fprintf(stderr, "Incomplete read of the FAT\n");
        close(fd);
        free(header);
        return NULL;
    }

    FileSystem *fs = (FileSystem *)malloc(sizeof(FileSystem));
    if (fs == NULL) {
        perror("Memory allocation error for FileSystem");
        close(fd);
        free(header);
        return NULL;
    }

    fs->header = header;
    fs->headerSize = headerSize;
    fs->fd = fd;

    return fs;
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
	if (handle->currentFileOffset < handle->length) {
		return 1;
	}

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

char _readFileByte(OpenFileHandle *handle) {
    char byte;
    // Initialize the byte with a default value (e.g., 0)
    // This is important in case readFile doesn't read any bytes (e.g., due to an error or EOF)
    byte = 0;

    // Call readFile to read exactly one byte into 'byte'
    int bytesRead = readFile(handle, &byte, 1);

    // Check if readFile successfully read one byte
    if (bytesRead == 1) {
        // Successfully read one byte, return it
        return byte;
    } else {
        return 0;
    }

    return byte;  // Return the read byte or the default value if no byte was read
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
