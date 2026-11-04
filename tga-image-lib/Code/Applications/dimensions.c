// This program named dimensions takes as argument the path to a TGA image file and reports
// the dimensions of the image. This program may be called with different optional modifiers that
// affect the output of the program:
// • -h Output image height only;
// • -w Output image width only;
// • -v verbose output.
// The modifier -v may be combined with -h or with -w, but the modifiers -h and
// -w are mutually exclusive.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "imageIO_TGA.h"
#include "operations.h"

int main(int argc, char** argv)
{
    // Need to have a filename
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [-vwh] <filename>\n", argv[0]);
        exit(1);
    }

    dimension_flags flags = {0, 0, 0};
    int option;
    while ((option = getopt(argc, argv, "vwh")) != -1)
    {
        switch (option) {
            case 'v': flags.v = 1; break;
            case 'w': flags.w = 1; break;
            case 'h': flags.h = 1; break;
            case '?': fprintf(stderr, "Invalid modifier: -%c\n", optopt); exit(1);
        }
    }

    // Can't ask for only width and only height
    if (flags.w && flags.h)
    {
        fprintf(stderr, "Modifiers -w and -h cannot be used together\n");
        exit(1);
    }

    if (argc - optind != 1)
    {
        fprintf(stderr, "Usage: %s [-vwh] <filename>\n", argv[0]);
        exit(1);
    }

    // File needs to be a .tga
    char* filename = argv[optind];
    if (!is_TGA(filename))
    {
        fprintf(stderr, "The argument is not a tga file: %s\n", filename);
        exit(1);
    }
    // File needs to exist
    if (access(filename, F_OK) == -1)
    {
        fprintf(stderr, "File not found: %s\n", filename);
        exit(1);
    }

    RasterImage image = readTGA(filename);
    if (image.raster == NULL || image.width == 0 || image.height == 0)
    {
        fprintf(stderr, "Failed to read image: %s\n", filename);
        exit(1);
    }

    int width, height;
    char* dimensions = get_dimensions(&image);
    if (dimensions == NULL)
    {
        fprintf(stderr, "Failed to determine image dimensions\n");
        free(image.raster);
        exit(1);
    }
    if (sscanf(dimensions, "%d %d", &width, &height) != 2)
    {
        fprintf(stderr, "Failed to parse image dimensions\n");
        free(dimensions);
        free(image.raster);
        exit(1);
    }
    if (flags.v && flags.w)
    {
        // Verbose width
        printf("width: %d\n", width);
    }
    else if (flags.v && flags.h)
    {
        // Verbose height
        printf("height: %d\n", height);
    }
    else if (flags.v)
    {
        // Verbose width and height
        printf("width: %d, height: %d\n", width, height);
    }
    else if (flags.w)
    {
        // Simple width
        printf("%d\n", width);
    }
    else if (flags.h)
    {
        // Simple height
        printf("%d\n", height);
    }
    else
    {
        // Simple width and height
        printf("%d %d\n", width, height);
    }

    free(dimensions);
    free(image.raster);

    return 0;
}