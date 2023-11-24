# makefile for simple-file-system

filesys: filesys.c
	gcc -Wall -Wextra -o filesys filesys.c
clean:
	rm ./filesys
