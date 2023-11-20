/**
 * @file filesys.c
 *
 * Based on a program by Michael Black, 2007
 * Revised 11.3.2020 O'Neil
 * Completed Mark P. Earl 12.8.2023 OS Lab 3
 * A Simple File System
 *
 * ACADEMIC INTEGRITY PLEDGE
 *
 * I have not used source code obtained from another student nor
 * any other unauthorized source, either modified or unmodified.
 *
 * All source code and documentation used in my program is either
 * my original work or was derived by me from the source code
 * published in the textbook for this course or presented in
 * class.
 *
 * I have not discussed coding details about this project with
 * anyone other than my instructor. I understand that I may discuss
 * the concepts of this program with other students and that another
 * student may help me debug my program so long as neither of us
 * writes anything during the discussion or modifies any computer
 * file during the discussion.
 *
 * I have violated neither the spirit nor letter of these restrictions.
 *
 * Signed: Mark P. Earl Date: 12.8.2023
 */

#include <stdio.h>
#include <string.h>

#define DISK_MAP_SECTOR_NUM 256
#define DISK_DIR_SECTOR_NUM 257
#define SECTOR_STORAGE_CAPACITY 512 // bytes
#define MAX_FILE_SIZE 12288

int main(int argc, char* argv[])
{
	/**
	 * @type FILE*: Pointer to an object type that identifies a stream and contains the information needed
	 * to control it, including a pointer to its buffer, its position indicator and all its state indicators.
	 *
	 * @doc: https://cplusplus.com/reference/cstdio/FILE/
	 */
	FILE* floppy;

	/**
	 * FILE * fopen ( const char * filename, const char * mode );
	 * @param filename: C string containing the name of the file to be opened.
	 * @param mode: C string containing a file access mode
	 * NOTE: "r+": read/update: Open a file for update (both for input and output). The file must exist.
	 *
	 * @retval: If the file is successfully opened, the function returns a pointer to a FILE object that
	 * can be used to identify the stream on future operations. Otherwise, a null pointer (NULL) is returned.
	 *
	 * @doc: https://cplusplus.com/reference/cstdio/fopen/
	 */

	// open the floppy image
	floppy=fopen("floppya.img","r+");

	// check to make sure the file exists
	if (floppy == NULL) {
		printf("floppya.img not found\n");
		return 0;
	}

	/**
	 * int fseek( FILE *stream, long offset, int origin );
	 * @param stream: file stream to modify
	 * @param offset: number of characters to shift the position relative to origin
	 * @param origin: position to which offset is added. It can have one of the following values: SEEK_SET, SEEK_CUR, SEEK_END
     *
	 **SEEK_SET argument to fseek indicating seeking from beginning of the file
	 * SEEK_CUR argument to fseek indicating seeking from the current file position
	 * SEEK_END argument to fseek indicating seeking from end of the file
     *
	 * @retval ​0​ upon success, nonzero value otherwise.
	 *
	 * @doc: https://cplusplus.com/reference/cstdio/fseek/
	 */

	// load the disk map from sector 256
	char map[SECTOR_STORAGE_CAPACITY];
	fseek(floppy, SECTOR_STORAGE_CAPACITY * DISK_MAP_SECTOR_NUM, SEEK_SET);
	for(int i = 0; i < SECTOR_STORAGE_CAPACITY; ++i) {

		/**
		 * int fgetc ( FILE * stream );
		 * @param stream: Pointer to a FILE object that identifies an input stream.
		 *
		 * @retval: On success, the character read is returned (promoted to an int value).
		 * The return type is int to accommodate for the special value EOF, which indicates failure:
		 * If the position indicator was at the end-of-file, the function returns EOF and sets the eof indicator (feof) of stream.
		 * If some other reading error happens, the function also returns EOF, but sets its error indicator (ferror) instead.
		*/
		map[i] = fgetc(floppy);
	}

	// load the directory from sector 257, refer to documentation for fseek and fgetc
	char dir[SECTOR_STORAGE_CAPACITY];
	fseek(floppy, SECTOR_STORAGE_CAPACITY * DISK_DIR_SECTOR_NUM, SEEK_SET);
	for (int i = 0; i < SECTOR_STORAGE_CAPACITY; ++i) {
		dir[i] = fgetc(floppy);
	}

	/**
	 * @option L: List files
	 * Tweak the existing code to get rid of the disk map and list the files so that the names print out in the
	 * traditional MSDOS 8-dot-3 format (cal.x, msg.t, etc.) without spaces in the middle. Also remove the
	 * starting sectors, keep only a list of file names and bytes used. Finally, by the way we designed the file
	 * system, a total of 511 sectors (or 511 x 512 = 261,632 bytes) is being tracked. At the end of the list of files,
	 * print out the total space used by the files and the total free space remaining, both in terms of the number
	 * of bytes.
	 */
	if (argc == 2 && strcmp(argv[1], "L") == 0) {

		printf("\nFiles on Disk:\n");
		printf("Filename\tBytes Used\n");

		int usedSpace = 0;
		for (int i = 0; i < 512; i += 16) {
			if (dir[i] == 0) break;

			// Printing file names in 8-dot-3 format
			for (int j = 0; j < 8; j++) {
				if (dir[i + j] == 0) {
					printf(".");
					break;
				}
				printf("%c", dir[i + j]);
			}

			// Printing file extension (type)
			printf("%c\t\t", dir[i + 8]);

			// Calculating and printing bytes used by each file
			int startSector = dir[i + 9];
			int numSectors = dir[i + 10];

			int bytesUsed = 512 * numSectors;
			usedSpace += bytesUsed;

			printf("%d\n", bytesUsed);
		}

		// Calculating free space
		int freeSpace = 261632 - usedSpace;

		printf("\nTotal space used by files: %d bytes\n", usedSpace);
		printf("Total free space remaining: %d bytes\n", freeSpace);
	}

	/**
	 * @option P: Print file
	 * Start with the disk directory and map loaded as in the current program. Do the following:
	 * 1. Go through the directory trying to match the file name (without “t” or “x” extension). If you don’t find
	 * it, return with a “file not found” error. If you do find it but it’s an executable file and not printable
	 * return with an error message.
	 * 2. Using the starting sector number and sector count in the directory, load the file into a new buffer of
	 * size 12288 (our max file size) via fseek and fgetc as shown in the starter file.
	 * 3. Starting from index zero print each individual character until you run into a zero (the end-of-file
	 * delimiter). Return.
	 * This is easy to test given the four files on the disk image, only one of which (msg) should be viewable. Also
	 * remember to test the “file not found” error.
	 */
	else if (argc == 3 && strcmp(argv[1], "P") == 0) {

		int found = 0;
		int startSector = -1;
		int numSectors = -1;

		char* targetFile = argv[2];

		for (int i = 0; i < 512; i += 16) {
			if (dir[i] == 0) break;

			// Check if the filename matches without extension
			char filename[9];
			strncpy(filename, (char*)&dir[i], 8);
			filename[8] = '\0';

			if (strcmp(filename, strtok(targetFile, ".")) == 0) {
				found = 1;
				startSector = dir[i + 9];
				numSectors = dir[i + 10];
				if (dir[i + 8] == 'x' || dir[i + 8] == 'X') {
					printf("Error: Executable file, cannot be printed\n");
					fclose(floppy);
					return 1;
				}
				break;
			}
		}

		if (!found) {
			printf("Error: File not found\n");
			fclose(floppy);
			return 1;
		}

		printf("Printing file '%s':\n", targetFile);

		// Load the file content
		fseek(floppy, startSector * 512, SEEK_SET);
		char buffer[MAX_FILE_SIZE];
		int bytesRead = 0;
		for (int i = 0; i < numSectors; i++) {
			for (int j = 0; j < 512; j++) {
				char c = fgetc(floppy);
				if (c == 0) {
					break; // End of file
				}
				buffer[bytesRead++] = c;
			}
		}

		// Print the file content
		buffer[bytesRead] = '\0'; // Null-terminate the buffer
		printf("%s\n", buffer);
	}

	else {
		// print disk map
		printf("Disk usage map:\n");
		printf("      0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
		printf("     --------------------------------\n");
		for (int i = 0; i < 16; ++i) {
			switch(i) {
				case 15: printf("0xF_ "); break;
				case 14: printf("0xE_ "); break;
				case 13: printf("0xD_ "); break;
				case 12: printf("0xC_ "); break;
				case 11: printf("0xB_ "); break;
				case 10: printf("0xA_ "); break;
				default: printf("0x%d_ ", i); break;
			}
			for (int j = 0; j < 16; ++j) {

				if (map[16 * i + j] == -1) {
					printf(" X");
				}

				else {
					printf(" .");
				}
			}
			printf("\n");
		}

		// print directory
		printf("\nDisk directory:\n");
		printf("Name    Type Start Length\n");
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; i += 16) {

			if (dir[i] == 0){
				break;
			}

			for (int j = 0; j < 8; ++j) {

				if (dir[i+j]==0){
					printf(" ");
				}

				else {
					printf("%c", dir[i + j]);
				}
			}

			if ((dir[i + 8] == 't') || (dir[i + 8] == 'T')) {
				printf("text");
			}

			else {
				printf("exec");
			}

			printf(" %5d %6d bytes\n", dir[i+9], SECTOR_STORAGE_CAPACITY*dir[i + 10]);
		}
	}


	/*
		//write the map and directory back to the floppy image
		fseek(floppy,512*256,SEEK_SET);
		for (i=0; i<512; i++) fputc(map[i],floppy);

		fseek(floppy,512*257,SEEK_SET);
		for (i=0; i<512; i++) fputc(dir[i],floppy);
	*/

	fclose(floppy);
}
