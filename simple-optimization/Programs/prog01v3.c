#include "search_bracket.h"

int main(int argc, char *argv[])
{
    // Check for the right number of arguments for making points
    if (argc < 7 || (argc - 1) % 2 != 0)
    {
        printf("INVALID%d\n", argc);
        return 1;
    }
    int num_points = (argc - 1) / 2;
    int num_brackets = num_points - 2;

    search_bracket* brackets = malloc(num_brackets * sizeof(search_bracket));
    for (int i = 0; i < num_brackets; i++)
    {
        // Create the bracket, ensure f(c) < f(a or b)
        brackets[i] = store_bracket(argv + (2 * i));
        if (!fc_strict_min(brackets[i]) || !acb_ordered(brackets[i]))
        {
            printf("%.1f %.1f %.1f %.1f %.1f %.1f",
               brackets[i].a.x, brackets[i].a.y,
               brackets[i].c.x, brackets[i].c.y,
               brackets[i].b.x, brackets[i].b.y);
            printf(" INVALID\n");
            continue;
        }

        // Print the bracket points for debugging
        printf("%.1f %.1f %.1f %.1f %.1f %.1f",
               brackets[i].a.x, brackets[i].a.y,
               brackets[i].c.x, brackets[i].c.y,
               brackets[i].b.x, brackets[i].b.y);
        // Use c as best guess for minimum, print it
        printf(" VALID %.1f %.1f\n", brackets[i].c.x, brackets[i].c.y);
    }

    free(brackets);
    return 0;
}