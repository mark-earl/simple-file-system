# simple-file-system

## Overview

In order to access a file stored on a disk you have to know its location and sectors used. For user convenience we prefer to have the O.S. track this information so the user can simply refer to files by name.

This is the purpose of a file system; to match a file name with its location and footprint on the disk. File systems greatly vary in complexity so our purpose is to work with a very simple one.

* [floppya.img](floppya.img): A simulated 3½" 1.44Mb floppy disk formatted with our file system and including a couple of application programs.
* [filesys.c](filesys.c) Program for manipulating the files stored on the disk.

The purpose of this lab is to turn [filesys.c](filesys.c) into a simple tool for manipulating the files stored on the provided disk. It will accept options from the Linux command line and alter the disk accordingly.

In short, once compiled it supports these four commands:
* `./filesys D filename` delete the named file from the disk
* `./filesys L` list the files on the disk
* `./filesys M filename` create a text file and store it to disk
* `./filesys P filename` read the named file and print it to screen

Any other option yields an error message.

## Build Instructions
1) Ensure that you are in the project directory
```bash
~/simple-file-system$
```
2) Run `make`
```bash
~/simple-file-system$ make
```
3) Run `filesys` with any of the desired options
```bash
~/simple-file-system$ ./filesys L
```
## Documentation
Any and all C functions used in [filesys.c](filesys.c) are recorded in [documentation.md](documentation.md).
