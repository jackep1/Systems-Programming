#include "search_bracket.h"

int main(int argc, char *argv[])
{
    // Check for the right number of arguments for making points
    if (argc != 7)
    {
        printf("INVALID\n");
        return 1;
    }

    // Create the bracket, ensure f(c) < f(a or b)
    search_bracket bracket = store_bracket(argv);

    // If bracket is not ordered, bracket is invalid
    if (!acb_ordered(bracket))
    {
        printf("INVALID\n");
        return 1;
    }
    
    // If f(c) is not minimum y point, make sure bracket curves up
    if (!fc_min(bracket))
    {
        if (!bracket_curves_up(bracket))
        {
            // If no up curve, bracket is invalid
            printf("INVALID\n");
            return 1;
        }
        // If bracket curves up, minimum is outside bracket
        printf("OUTSIDE\n");
        return 1;
    }

    // If f(c) is minimum y point, minimum is inside bracket
    printf("INSIDE\n");

    return 0;
}