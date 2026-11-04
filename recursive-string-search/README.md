# Recursive Text Search

A Bash script and C programs that count how many times a string appears across a directory tree, with extra-credit versions that search a directory and filter by file extension.

**Course:** CSC 412 Operating Systems, University of Rhode Island · **Languages:** C, Bash · **Platform:** Linux

## Overview

The script03.sh script walks a folder recursively, gathers statistics about the tree, and hands the text files to a C program that counts matches. The C program counts overlapping matches, so searching `aaaaa` for `a` returns 5.

## Programs

| Program | Usage | What it does |
|---|---|---|
| `script03.sh` | `./script03.sh <target> <rootFolder>` | Recursively counts folders and files, finds the maximum depth, lists `.txt` files, and searches them |
| `prog03` | `./prog03 <target> <file1> <file2> ...` | Counts matches of the target in each file |
| `prog03EC1` | `./prog03EC1 <target> <directory>` | Searches every non-hidden file in a directory, without any recursion to further directories |
| `prog03EC2` | `./prog03EC2 <target> <directory> <ext> ...` | Performs the same work as EC1, but limited to the given file extensions |

## Build and Run

All scripts are intended to be run from the project root. Build with build.sh, run the full recursively searching program with script03.sh, or use individual executables as below.

```bash
bash Scripts/build.sh
bash Scripts/script03.sh "bread" sandbox
./Executables/prog03 "butter" ./sandbox/one.txt
./Executables/prog03EC1 "bread" ./sandbox
./Executables/prog03EC2 "butter" ./sandbox .txt
```

`sandbox/` holds a small directory tree for testing.

## How It Works

`search_folder` tracks the current folder and depth, updates the maximum depth, counts files, collects `.txt` files with `find`, and recurses into each subfolder. `search_file_for` reads line by line and advances one character after each match so matches may overlap.

## Credits

Assignment and sandbox testing files from the course. Scripts and C programs are my work.
