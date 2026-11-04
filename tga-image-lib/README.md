# TGA Image Operations

Command-line tools in C that crop, rotate, and split TGA images, with bash scripts for building, and for processesing a whole folder.

**Course:** CSC 412 Operating Systems, University of Rhode Island · **Languages:** C, Bash · **Platform:** Linux

## Overview

Four small programs share one operations library. Together they read uncompressed TGA files, transform the pixels, and write new images. A script builds everything and runs the transformations on every image in a directory.

## Tools

| Program | Usage | What it does |
|---|---|---|
| `split` | `./split <file.tga>` | Writes the red, green, and blue channels as separate images |
| `rotate` | `./rotate -l\|-ll\|-r\|-rr <file.tga>` | Rotates the image left or right, once or twice |
| `crop` | `./crop <x> <y> <w> <h> <file.tga>` | Crops to the given rectangle after validating it fits the image |
| `dimensions` | `./dimensions -w\|-h\|-v <file.tga>` | Reports the image width and height |

The programs check that the input is a `.tga` file that exists, and that numeric arguments are valid.

## Build and Run

```bash
bash Scripts/script02.sh <imageDirectory> <outputDirectory>
```

The script builds the programs, runs `split` and both rotations on each `.tga` in the directory, and moves the outputs into the output directory with channel and rotation labels in the file names.

## Project Structure

```
Code/Applications/           crop.c, rotate.c, split.c, dimensions.c
Code/ImageLibrary/src/       operations.c, imageIO_TGA.c, rasterImage.c
Code/ImageLibrary/include/   operations.h, imageIO_TGA.h, rasterImage.h
Images/                      Sample images
Scripts/                     build.sh, script02.sh
Report.pdf                   Write-up
```

## Credits

The TGA reading and writing library, rasterImage library, and sample images were provided by the course (original code by Jean-Yves Herve). The files in the applications folder, the scripts, and operations.c/operations.h are my work.
