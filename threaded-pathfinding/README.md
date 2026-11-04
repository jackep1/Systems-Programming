# Multiprocess Grid Pathfinding

A C++ program that finds routes between nodes on a weighted grid, computing each route in its own child process and, in later versions, each step of the route in grandchild processes.

**Course:** CSC 412 Operating Systems, University of Rhode Island · **Languages:** C++, Bash · **Platform:** Linux

## Overview

The input is a grid of movement costs and a list of node coordinates. The program links each node to its nearest neighbors, finds every path between a start node and an end node within a length range, then computes the cheapest route across the grid for each path using multiple processes.

# Algorithm

- Nodes connect to their three nearest neighbors, extending past three when distances tie
- Search over the node graph that returns every path within the minimum and maximum length
- Search for a cheapest route over the weight grid that moves to all eight adjacent squares, tracking predecessors and best costs
- One child process per path, writing results to scrap files
- Grandchild processes computing the sub-route for each step of a path

## Concepts Demonstrated

`fork`, process hierarchies (parent, child, grandchild), file-based communication, graph search, and bounds checking.

## Build and Run

```bash
bash Scripts/build.sh <baseName>     # builds Versions 1 to 3 as <baseName>1, <baseName>2, <baseName>3
bash Scripts/run.sh <baseName> <gridFile> <nodeFile> <startIdx> <endIdx> <scrapDir> <outputFile>
bash Scripts/cleanup.sh     # removes executables, scrap files, and output files
```

## Algorithm

1. Read the weight grid and node list, and compute each node's nearest neighbors.
2. Search the node graph for all paths between the start and end node within the length limits (number of nodes, not number of points on grid), and write them to the output file.
3. Fork one child per path. Each child writes its path, its total cost, and the grid coordinates it passes through to a scrap file.
4. In the versions with grandchildren, each child forks once per step so the cheapest route between each pair of nodes is computed in parallel. The three versions explore different process structures, with all work done by the parent in version 1, up to actual multiprocessing with child and grandchild processes handling real work in version 3.

## Project Structure

```
Programs/Version<x>               Source for each version
Scripts/                          build.sh and run.sh
Data/                             Grid and node files
Path Output Examples/             Expected output for sample inputs
```

## Credits

Assignment and data files from the course. The design and implementation are my work.
