#!/bin/bash

gcc -Wall -std=c99 -I ./Code/ImageLibrary/include ./Code/ImageLibrary/src/*.c ./Code/Applications/main.c -o main
gcc -Wall -std=c99 -I ./Code/ImageLibrary/include ./Code/ImageLibrary/src/*.c ./Code/Applications/main2D.c -o main2D
gcc -Wall -std=c99 -I ./Code/ImageLibrary/include ./Code/ImageLibrary/src/*.c ./Code/Applications/dimensions.c -o dimensions
gcc -Wall -std=c99 -I ./Code/ImageLibrary/include ./Code/ImageLibrary/src/*.c ./Code/Applications/split.c -o split
gcc -Wall -std=c99 -I ./Code/ImageLibrary/include ./Code/ImageLibrary/src/*.c ./Code/Applications/crop.c -o crop
gcc -Wall -std=c99 -I ./Code/ImageLibrary/include ./Code/ImageLibrary/src/*.c ./Code/Applications/rotate.c -o rotate