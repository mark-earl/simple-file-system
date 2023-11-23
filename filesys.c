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

// Includes
#include <stdio.h>
#include <string.h>

// Constants (in bytes)
#define DISK_MAP_SECTOR_NUM 256
#define DISK_DIR_SECTOR_NUM 257
#define SECTOR_STORAGE_CAPACITY 512
#define TOTAL_BYTES 261632
#define MAX_FILE_SIZE 12288

int main(int argc, char* argv[])
{
	// stream object
	FILE* floppy;

	// open the floppy image
	floppy=fopen("floppya.img","r+");

	// check to make sure the file exists
	if (floppy == NULL) {
		printf("floppya.img not found\n");
		return 0;
	}

	// load the disk map from sector 256
	char map[SECTOR_STORAGE_CAPACITY];
	fseek(floppy, SECTOR_STORAGE_CAPACITY * DISK_MAP_SECTOR_NUM, SEEK_SET);
	for(int i = 0; i < SECTOR_STORAGE_CAPACITY; ++i) {
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
	 *
	 * 00020220 6d 73 67 00 00 00 00 00 74 05 01 00 00 00 00 00 |msg.....t.......|
	 * 00020220: Starting address
	 * 6d 73 67 00 00 00 00 00: File name
	 * 74: File type
	 * 05: Starting Position
	 * 01: Number of Sectors
	 */
	if (argc == 2 && strcmp(argv[1], "L") == 0) {

		printf("\nFiles on Disk:\n");
		printf("Filename\tBytes Used\n");

		int usedSpace = 0;
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; i += 16) {

			// No entry? Go to next one.
			if (dir[i] == 0) continue;

			// Printing file names in 8-dot-3 format
			for (int j = 0; j < 8; ++j) {
				if (dir[i + j] == 0) {
					printf(".");
					break;
				}
				printf("%c", dir[i + j]);
			}

			// Printing file extension (9th byte)
			printf("%c\t\t", dir[i + 8]);

			// Calculating and printing bytes used by each file
			int numSectors = dir[i + 10]; // (11th byte)
			int bytesUsed = SECTOR_STORAGE_CAPACITY * numSectors;
			usedSpace += bytesUsed;

			printf("%d\n", bytesUsed);
		}

		// Calculating and output free space
		int freeSpace = TOTAL_BYTES - usedSpace;
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
	 * size 12,288 (our max file size) via fseek and fgetc as shown in the starter file.
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

		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; i += 16) {
			if (dir[i] == 0) continue;

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
		fseek(floppy, startSector * SECTOR_STORAGE_CAPACITY, SEEK_SET);
		char buffer[MAX_FILE_SIZE];
		int bytesRead = 0;
		for (int i = 0; i < numSectors; ++i) {
			for (int j = 0; j < SECTOR_STORAGE_CAPACITY; ++j) {
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

	/**
	 * @option M: Create and store a text file
	 * Prompt the user for a string of text and create a 1-sector (512 byte) file storing it. Writing a file means
	 * finding a free directory entry and setting it up, finding free space on the disk for the file, and setting the
	 * appropriate map byte(s). Your function should do the following:
	 * 1. Search through the directory, doing two things simultaneously:
	 * a. If you find the file name already exists, terminate with a “duplicate or invalid file name” error.
	 * b. Otherwise find and note a free directory entry (one that begins with zero).
	 * 2. Copy the name to that directory entry. If the name is shorter than 8 characters, fill in the remaining
	 * bytes with zeros. If the name is longer than 8 characters keep only the first 8. Include the “t” file type
	 * at the ninth location in the entry.
	 * 3. To write the actual file to disk:
	 * a. Find a free sector on the disk by searching through the map for a zero.
	 * b. Set its map entry to 255.
	 * c. Add the starting sector number and length (1) to the file's directory entry.
	 * d. Write the buffer holding the file to the correct sector.
	 * 4. Write the map and directory sectors back to the disk.
	 * If there are no free directory entries or not enough free sectors left, your function should terminate with
	 * an “insufficient disk space” error. Testing should be obvious (create a file and try printing it out) but
	 * remember the error cases.
	 */
    else if (argc == 3 && strcmp(argv[1], "M") == 0) {

		char* newFileName = argv[2];

		int freeDirEntryIndex = -1;
		int freeSector = -1;

		// Search for a free directory entry
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; i += 16) {

			// If we found a free index, break
			if (dir[i] == 0) {
				freeDirEntryIndex = i;
				break;
			}

			// Get the current filename
			char filename[9];
			strncpy(filename, (char*)&dir[i], 8);
			filename[8] = '\0';

			// Ensure the new filename is unique
			if (strcmp(filename, newFileName) == 0) {
				printf("Error: Duplicate or invalid file name\n");
				fclose(floppy);
				return 1;
			}
		}

		// Check directory space
		if (freeDirEntryIndex == -1) {
			printf("Error: Insufficient directory space\n");
			fclose(floppy);
			return 1;
		}

		// Copy the file name to the directory entry
		strncpy((char*)&dir[freeDirEntryIndex], newFileName, 8);

		// Fill remaining bytes with zeros if the name is shorter than 8 characters
		for (int i = strlen(newFileName); i < 8; ++i) {
			dir[freeDirEntryIndex + i] = 0;
		}

		// Set file type as "t" (text file) (9th location)
		dir[freeDirEntryIndex + 8] = 't';

		// Find a free sector on the disk in the map
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; ++i) {
			if (map[i] == 0) {
				freeSector = i;
				map[i] = 255; // Set map entry to 255 (0xFF) (indicating it's used)
				break;
			}
		}

		// Check disk space
		if (freeSector == -1) {
			printf("Error: Insufficient disk space\n");
			fclose(floppy);
			return 1;
		}

		char content[512]; // 511 bytes for content + 1 for null terminator
		printf("Enter content for the file (up to 512 characters):\n");
		fgets(content, sizeof(content), stdin);

		// Truncate content if it exceeds 512 bytes
		content[511] = '\0';

		// Find where we want to write
		fseek(floppy, freeSector * SECTOR_STORAGE_CAPACITY, SEEK_SET);

		// Fill the sector with null bytes (0s)
		char eraseBuffer[SECTOR_STORAGE_CAPACITY];
		memset(eraseBuffer, 0, sizeof(eraseBuffer));
    	fwrite(eraseBuffer, sizeof(char), SECTOR_STORAGE_CAPACITY, floppy);

		// Find where we want to write (again traveling "backwards")
		fseek(floppy, freeSector * SECTOR_STORAGE_CAPACITY, SEEK_SET);
		// Write the content to the file
		fwrite(content, sizeof(char), strlen(content), floppy);

		// Update directory entry with file location information
		dir[freeDirEntryIndex + 9] = freeSector; // Start sector
		dir[freeDirEntryIndex + 10] = 1; // Only uses one sector

		// Write the map and directory sectors back to the disk
		fseek(floppy, SECTOR_STORAGE_CAPACITY * DISK_MAP_SECTOR_NUM, SEEK_SET);
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; ++i) {
			fputc(map[i], floppy);
		}

		fseek(floppy, SECTOR_STORAGE_CAPACITY * DISK_DIR_SECTOR_NUM, SEEK_SET);
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; ++i) {
			fputc(dir[i], floppy);
		}

		printf("File '%s' created successfully\n", newFileName);
	}

	/**
	 * @option D: Delete file
  	 * Deleting a file takes two steps. First, you need to change all the sectors reserved for the file in the disk
  	 * map to free. Second, you need to set the first byte in the file's directory entry to zero.
  	 * Your function should find the file in the directory and delete it if it exists. To accomplish this, do the
  	 * following:
  	 * 1. Search through the directory and try to find the file name. If you can’t find it terminate with a “file not
  	 * found” error.
  	 * 2. Set the first byte of the file name to zero.
  	 * 3. Based on the starting sector and file length, step through the sectors numbers listed as belonging to
  	 * the file. For each sector, set the corresponding map byte to zero. For example, if sector 7 belongs to
  	 * the file, set the eighth map byte to zero (index 7, since the map starts at sector 0).
  	 * 4. Write the character arrays holding the directory and map back to their appropriate sectors.
  	 * Notice that this does not actually delete the file from the disk. It just makes the disk space used by the
  	 * file available to be overwritten by something else. This is typically done in operating systems; it makes
  	 * deletion fast and un-deletion possible. Test as above: create a file and then delete it, followed by a check
  	 * of the hexdump.
	 */
	else if (argc == 3 && strcmp(argv[1], "D") == 0) {

		char* deleteFileName = argv[2];

		int fileEntryIndex = -1;
		int startSector = -1;
		int numSectors = -1;

		// Search for the file in the directory
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; i += 16) {
			if (dir[i] == 0) {
				continue;
			}

			char filename[9];
			strncpy(filename, (char*)&dir[i], 8);
			filename[8] = '\0';

			if (strcmp(filename, deleteFileName) == 0) {
				fileEntryIndex = i;
				startSector = dir[i + 9];
				numSectors = dir[i + 10];
				break;
			}
		}

		if (fileEntryIndex == -1) {
			printf("Error: File not found\n");
			fclose(floppy);
			return 1;
		}

		// Mark the directory entry as deleted
		dir[fileEntryIndex] = 0;

		// Release sectors in the disk map
		for (int i = startSector; i < startSector + numSectors; ++i) {
			map[i] = 0;
		}

		// Write the updated map and directory sectors back to the disk
		fseek(floppy, SECTOR_STORAGE_CAPACITY * 256, SEEK_SET);
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; ++i) {
			fputc(map[i], floppy);
		}

		fseek(floppy, SECTOR_STORAGE_CAPACITY * 257, SEEK_SET);
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; ++i) {
			fputc(dir[i], floppy);
		}

		printf("File '%s' deleted successfully\n", deleteFileName);
	}

	// print disk map
	else if (argc == 2 && strcmp(argv[1], "DISK_MAP") == 0) {
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
				if (map[16 * i + j] == -1) {printf(" X");}
				else {printf(" .");}
			}
			printf("\n");
		}

		// print directory
		printf("\nDisk directory:\n");
		printf("Name    Type Start Length\n");
		for (int i = 0; i < SECTOR_STORAGE_CAPACITY; i += 16) {

			if (dir[i] == 0){break;}

			for (int j = 0; j < 8; ++j) {
				if (dir[i+j]==0){printf(" ");}
				else {printf("%c", dir[i + j]);}
			}

			if ((dir[i + 8] == 't') || (dir[i + 8] == 'T')) {printf("text");}
			else {printf("exec");}

			printf(" %5d %6d bytes\n", dir[i+9], SECTOR_STORAGE_CAPACITY*dir[i + 10]);
		}
	}

	fclose(floppy);
}
