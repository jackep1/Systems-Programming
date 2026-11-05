# Multiprocess Document Assembly

A C++ program that reassembles a document from line fragments scattered across many data files, using multiple processes that communicate through scrap files, pipes, and lock files.

**Course:** CSC 412 Operating Systems, University of Rhode Island · **Languages:** C++20, Bash · **Platform:** Linux

## Overview

Each data file holds lines tagged with their position in the document. The program splits the files among several worker processes, each worker reads and reorders its share, and the parent merges everything into one output file. The assignment was built in five versions, each changing how the processes divide work and communicate.

## Highlights

- A distributor that records which files each process owns, and a processor that reads and reorders the lines
- An even split of files across processes, with leftover files given to the first processes
- Communication through scrap files, then through unnamed pipes
- Lock files so processors wait until every distributor has finished
- Separate distributor and processor executables started with `execv`

## Concepts Demonstrated

`fork`, `execv`, `wait`, unnamed pipes, non-blocking reads, lock files, and file-based hand-offs between processes.

## Build and Run

```bash
bash Scripts/build.sh v4     # builds one version (v1 to v4); omit the argument to build all
bash Scripts/script04.sh <pathToDataFolder> <pathToReassembledOutput>   # Builds/runs the given version and deletes all executables
```

The program takes the number of processes, the data folder, and the output file path.

## How It Works

1. **Version 1:** One process calls the distributor and processor functions in turn as "pseudo-processes," a single-process reference design.
2. **Version 2:** `fork` once per distributor and once per processor, passing results through numbered scrap files.
3. **Version 3:** The distributor and processor become separate programs launched with `execv`.
4. **Version 4:** Unnamed pipes replace shared scrap files, and lock files control when processors may start.

## Project Structure

```
Programs/Version<x>/              main.cpp, distributor.cpp, processor.cpp
Scripts/                          build.sh and run scripts
Data-Sets/                        Sample input files
```

## Limitations

- The program assumes it is launched from the run scripts, so argument checking is minimal.
- Some sources of errors are unaccounted for because the assignment specs allowed, and time was limited.
- Some repeated logic (building the line map) should be factored into a helper function.

## Credits

Assignment and sample data from the course. The design and implementation are my work.
