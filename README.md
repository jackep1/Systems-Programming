# Systems Programming

C and C++ projects on processes, threads, synchronization, and Unix tooling, built for CSC 412 (Operating Systems) at the University of Rhode Island. These are the smaller first five assignments from the course, larger projects for a Pac-Man game and a robot grid simulation have their own repositories. Projects work when used as intended but are not robust in error handling, because the assignment specs allowed this and time was limited.

| Project | What it does | Key concepts | Language |
|---|---|---|---|
| [Simple Optimization](simple-optimization) | Finds local minima from sample points or a parabolic fit | Numerical methods, build scripts | C |
| [TGA Image Operations](tga-image-operations) | Command-line tools to crop, rotate, split channels, and report dimensions of TGA images | File I/O, image buffers, scripting | C, Bash |
| [Recursive Text Search](recursive-text-search) | Counts matches of a string across a directory tree, with file extension filters | Recursion, directory traversal, Bash and C together | C, Bash |
| [Multiprocess Grid Pathfinding](multiprocess-grid-pathfinding) | Finds low-cost routes across a weighted grid using child and grandchild processes | `fork`, scrap files, shortest-path search | C++, Bash |
| [Multiprocess Document Assembly](multiprocess-document-assembly) | Reassembles a document from line fragments spread across many files | `fork`, `execv`, pipes, lock files | C++, Bash |

## Concepts Covered

- Process creation and management: `fork`, `execv`, `wait`
- Inter-process communication: unnamed pipes, scrap files, lock files
- Thread synchronization: `std::mutex`, per-resource locks, detached threads
- Shell scripting for building, running, and batch testing
- Building each program in several versions and comparing designs

## Building

Projects build with `g++` (C++20) or `gcc` and include a `build.sh` script.

See each project's README for exact commands.

## Credits

Assignments and starter code for some projects were provided by the CSC 412 course staff. My work is described in each project README.
