#include "search_bracket.h"

int main(int argc, char *argv[])
{
    // Check for the right number of arguments for making points
    if (argc != 7) {
        printf("INVALID\n");
        return 1;
    }

    // Create the bracket, ensure f(c) < f(a or b)
    search_bracket bracket = store_bracket(argv);
    if (!fc_strict_min(bracket) || !acb_ordered(bracket)) {
        printf("INVALID\n");
        return 1;
    }

    // Use c as best guess for minimum, print it
    printf("VALID %.1f %.1f\n", bracket.c.x, bracket.c.y);

    return 0;
}