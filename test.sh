#!/bin/bash

echo "Testing Script for the Simple File Sys:"

echo "Press any key to build the project"
read -n 1 -s -r -p ""
rm filesys
make
echo "Build successfull"

echo "Press any key to test the 'L' option"
read -n 1 -s -r -p ""
echo "./filesys L"
./filesys L

echo "Press any key to test the 'P' option"
read -n 1 -s -r -p ""
echo "./filesys P msg"
./filesys P msg
echo "./filesys P t3"
./filesys P t3
echo "./filesys P abc"
./filesys P abc

echo "Press any key to test the 'M' option"
read -n 1 -s -r -p ""
echo "Before file creation"
./filesys
./filesys L
echo "./filesys M title"
./filesys M title
echo "After file creation"
./filesys
./filesys L

echo "All commands executed successfully!"
