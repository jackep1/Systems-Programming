# Simple Optimization

A collection of C programs that evaluate three-point search brackets for a local minimum. The project also includes a script for building the executables. The focus is input validation, simple geometry, and parabolic fitting rather than advanced optimization.

**Course:** CSC 412 Operating Systems, University of Rhode Island · **Language:** C · **Platform:** Linux

## Overview

The program takes a list of `(x, y)` points as command-line arguments, narrows down a bracket around the lowest point, and reports the minimum. It was the first assignment of the course and focuses on C fundamentals and build scripting.

## Build and Run

To build, run the following from the repository root:

```bash
bash Scripts/build.sh
```

This creates three executables:

- `prog01v1`
- `prog01v2`
- `prog01v3`

To run, arguments are pairs of `x y` values. Invalid input prints `INVALID`. Examples:

```bash
./prog01v1 3 4.5 1 10.2 6 2.5  # three (x, y) points
./prog01v2 1.3 4.5 3.3 -2.8 6.2 -1.5  # points on upward curving parabola
./prog01v3 3.0 4.5 1.5 10.2 6.1 2.5 8.4 9.5  # v3 accepts longer lists
```

## Input Format

Points are entered as an x-coordinate followed by a y-coordinate:

```text
x1 y1 x2 y2 x3 y3
```

For example:

```bash
./prog01v1 3.0 4.5 1.5 10.2 6.1 2.5
```

## Versions

### Version 1

Version 1 accepts exactly three points. The bracket must be in `(a, c, b)` order, with the middle point's y-coordinate strictly lower than the other two points.

A valid bracket prints the estimated minimum:

```text
VALID x y
```

The reported point is `c`. Invalid input prints `INVALID`.

### Version 2

Version 2 accepts exactly three points and determines whether an upward-curving parabola through the points has its minimum inside the bracket.

- `INSIDE` - the minimum is inside the bracket.
- `OUTSIDE` - the minimum is outside the bracket.
- `INVALID` - the input is invalid or the points form a downward-curving parabola.

### Version 3

Version 3 accepts at least three points. It processes each successive group of three points as a bracket:

```text
(x1, y1), (x2, y2), (x3, y3)
```

The program applies the Version 1 validation to each bracket. Valid brackets print the middle point as a local-minimum guess:

```text
VALID x y
```

## Credits

Assignment is from the course. Implementation is my work.