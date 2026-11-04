#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// Simple 2D point structure
typedef struct {
    float x;
    float y;
} point;

// Structure to hold a bracket of points (a, b, c)
typedef struct {
    point a;
    point b;
    point c;
} search_bracket;

// Creates a search_bracket from command line arguments,
// where argv contains points a, c, and b in that order
search_bracket store_bracket(char* argv[])
{
    point a = {atof(argv[1]), atof(argv[2])};
    // Important to note that c is entered second, not third
    point c = {atof(argv[3]), atof(argv[4])};
    // And then b is next
    point b = {atof(argv[5]), atof(argv[6])};
    search_bracket bracket = {a, b, c};
    return bracket;
}

// Checks if the points in the bracket are ordered such that a.x < c.x < b.x
bool acb_ordered(search_bracket bracket)
{
    return bracket.a.x < bracket.c.x && bracket.c.x < bracket.b.x;
}

// Checks if f(c) is strictly less than f(a) and f(b)
bool fc_strict_min(search_bracket bracket)
{
    return bracket.c.y < bracket.a.y && bracket.c.y < bracket.b.y;
}

// Checks if f(c) is less than or equal to f(a) and f(b)
bool fc_min(search_bracket bracket)
{
    return bracket.c.y <= bracket.a.y && bracket.c.y <= bracket.b.y;
}

// Checks if the bracket represents a parabola that opens upward
bool bracket_curves_up(search_bracket bracket)
{
    float x1 = bracket.a.x, y1 = bracket.a.y;
    float x2 = bracket.b.x, y2 = bracket.b.y;
    float x3 = bracket.c.x, y3 = bracket.c.y;

    float denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
    if (denom == 0)
    {
        printf("INVALID\n");
        exit(1);
    }

    float a = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;

    return a > 0;
}