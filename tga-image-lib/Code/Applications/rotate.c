#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "imageIO_TGA.h"
#include "operations.h"

int main(int argc, char** argv)
{
    // Needs rotation specs and filename
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <-[llrr]> <filePath>\n", argv[0]);
        exit(1);
    }
    // File needs to be a .tga
    char* filename = argv[2];
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

    rotate_flags flags = {0, 0, 0, 0};
    if (strcmp(argv[1], "-ll") == 0)
    {
        flags.ll = 1;
    }
    else if (strcmp(argv[1], "-l") == 0)
    {
        flags.l = 1;
    }
    else if (strcmp(argv[1], "-rr") == 0)
    {
        flags.rr = 1;
    }
    else if (strcmp(argv[1], "-r") == 0)
    {
        flags.r = 1;
    }
    else
    {
        fprintf(stderr, "Invalid rotation string: %s\n", argv[1]);
        exit(1);
    }

    // Read the image and rotate it
    RasterImage image = readTGA(filename);
    RasterImage rotated = rotate(&image, &flags);

    // Create a filename
    char* rotated_filename = (char*)malloc(strlen(filename) + 9);

    // Cut off filename extension
    char* dot = strrchr(filename, '.');
    if (dot != NULL)
    {
        *dot = '\0';
    }

    sprintf(rotated_filename, "%s", filename);
    if (flags.l)
    {
        strcpy(rotated_filename + (dot - filename), " [L].tga");
    } else if (flags.ll)
    {
        strcpy(rotated_filename + (dot - filename), " [LL].tga");
    } else if (flags.r)
    {
        strcpy(rotated_filename + (dot - filename), " [R].tga");
    } else if (flags.rr)
    {
        strcpy(rotated_filename + (dot - filename), " [RR].tga");
    }

    // Write the rotated image
    writeTGA(rotated_filename, &rotated);

    // Free memory
    free(rotated_filename);
    free(image.raster);
    free(rotated.raster);

    return 0;
}