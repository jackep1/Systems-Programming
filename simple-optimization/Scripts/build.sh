#!/bin/bash 

# This script is meant to be run from the root folder Prog01.  It assumes that the 
# source file prog01v1 is inside the Programs folder.  It builds the executable 
# directly at the root of the project folder.

gcc  -Wall -std=c99 Programs/prog01v1.c -o ./prog01v1
gcc  -Wall -std=c99 Programs/prog01v2.c -o ./prog01v2
gcc  -Wall -std=c99 Programs/prog01v3.c -o ./prog01v3