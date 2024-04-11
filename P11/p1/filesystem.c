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
        return NULL;
    }

    // Calculate the fixed size of the header by considering the size of the structure with one block_t
    // size_t fixedHeaderSize = sizeof(FileSystemHeader) - sizeof(block_t);
    size_t fixedHeaderSize = 3*sizeof(uint32_t);

    /*
        0. git good // git commit -m "good"
        1. read disk file
        2. make FileSystem var
        3. make FileSystemHeader
        4. ...
        5. Profit!🤑
    */

    FileSystem *fs = (FileSystem *)malloc(sizeof(FileSystem));
    fs->header = (FileSystemHeader*)malloc(sizeof(FileSystemHeader));
    
    ssize_t bytesRead = read(fd, fs->header, fixedHeaderSize);
    if (bytesRead == -1) {
        close(fd);
        return NULL;
    }
    if ((size_t)bytesRead != fixedHeaderSize) {
        fprintf(stderr, "Incomplete read of the file system header\n");
        close(fd);
        return NULL;
    }
    // FIXED: HEADER konnfirmed correct
    printf("Magic: %#010x\n", fs->header->magic);
    printf("Block count: %d\n", fs->header->fsBlocks);
    printf("root dir : %d\n", fs->header->rootDirectorySize);
    //
    // size_t fatSize = sizeof(block_t) * header.fsBlocks;   fat[1] = 4 fat[1] = 0
    size_t fatSize = read(fd, fs->header->fat, fs->header->fsBlocks*sizeof(uint16_t));
    int headerSize = fixedHeaderSize + fatSize;
    printf("fat[0]: %d\n", fs->header->fat[0]);
    printf("fat[1]: %d\n\n", fs->header->fat[1]);
    
    printf("fatSize: %ld\n", fatSize);
    printf("Header size: %d\n", headerSize);

    printf("mem loc 1 %p\n", &fs->header->magic);
    printf("mem loc 2 %p\n", &fd);



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

int _findDirectoryEntry(OpenFileHandle *dir, char *name, DirectoryEntry *dirEntry) {
    if (dir == NULL || name == NULL || dirEntry == NULL) {
        return -1;  // Invalid arguments
    }

    // Assuming that DIRECTORY_ENTRY_SIZE is the size of a DirectoryEntry structure
    const size_t DIRECTORY_ENTRY_SIZE = sizeof(DirectoryEntry);
    DirectoryEntry entry;

    // Start from the beginning of the directory file
    dir->currentFileOffset = 0;

    while (1) {
        // Read one DirectoryEntry at a time
        size_t bytesRead = readFile(dir, (char *)&entry, DIRECTORY_ENTRY_SIZE);

        // Check if we have read a full DirectoryEntry
        if (bytesRead == DIRECTORY_ENTRY_SIZE) {
            // Compare the current entry's name with the requested name
            if (strcmp(entry.name, name) == 0) {
                // Name matches, copy the found entry to dirEntry and return 0
                *dirEntry = entry;
                return 0;  // Found the requested entry
            }
            // If names don't match, continue reading the next entry
        } else {
            // Either an error occurred, or we've reached the end of the directory file
            // Since we didn't find the entry, return -1
            break;
        }
    }

    return -1;  // Entry not found
}


OpenFileHandle *openFile(FileSystem *fs, char *dir, char *name) {
	(void)*dir;

    if ((fs == NULL) || (name == NULL)) {
        return NULL;
    }

    // Open the root directory file.
    OpenFileHandle *root = _openFileAtBlock(fs, ROOT_DIRECTORY_BLOCK, fs->header->rootDirectorySize);
    if (root == NULL) {
        return NULL;  // Failed to open root directory
    }
  
    DirectoryEntry dirEntry;
    int found = _findDirectoryEntry(root, name, &dirEntry);

    // Close the root directory handle as it's no longer needed
    closeFile(root);

    if (found != 0 || dirEntry.type != FTYPE_REGULAR) {
        // File name not found or entry is not a regular file
        return NULL;
    }

    // If the file is found and it's a regular file, open a new file handle for it
    OpenFileHandle *fileHandle = _openFileAtBlock(fs, dirEntry.firstBlock, dirEntry.length);
    return fileHandle;  // Returns the new file handle or NULL if _openFileAtBlock fails
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
