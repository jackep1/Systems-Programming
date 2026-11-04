#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "imageIO_TGA.h"
#include "operations.h"

int main(int argc, char** argv)
{
    // Needs coordinates and a filename
    if (argc != 6)
    {
        fprintf(stderr, "Usage: %s <x> <y> <w> <h> <filePath>\n", argv[0]);
        exit(1);
    }
    // File needs to be a .tga
    char* filename = argv[5];
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

    crop_rectangle flags = {0, 0, 0, 0};
    if (is_numeric(argv[1]) && is_numeric(argv[2]) && is_numeric(argv[3]) && is_numeric(argv[4]) &&
        atoi(argv[1]) >= 0 && atoi(argv[2]) >= 0 && atoi(argv[3]) > 0 && atoi(argv[4]) > 0)
    {
        flags.x_left = atoi(argv[1]);
        flags.y_top = atoi(argv[2]);
        flags.width = atoi(argv[3]);
        flags.height = atoi(argv[4]);
    } else
    {
        fprintf(stderr, "The arguments defining the cropping rectangle must be positive integers.\n");
        exit(1);
    }

    // Read the image
    RasterImage image = readTGA(filename);
    if (image.raster == NULL || image.width == 0 || image.height == 0)
    {
        fprintf(stderr, "Failed to read image: %s\n", filename);
        exit(1);
    }

    // Check for rectangle vs. dimension errors
    int rect_errs = check_crop_rectangle(&flags, &image);

    if (rect_errs == 1)
    {
        fprintf(stderr, "The arguments defining the cropping rectangle must be positive integers.\n");
        exit(1);
    } else if (rect_errs == 2)
    {
        fprintf(stderr, "The corner of the cropping rectangle lies outside the image.\n");
        exit(1);
    } else if (rect_errs == 3)
    {
        fprintf(stderr, "The cropping rectangle extends outside the image.\n");
        exit(1);
    }
    // Crop the image
    RasterImage cropped = crop(&image, &flags);
    if (cropped.raster == NULL)
    {
        fprintf(stderr, "Failed to crop image: %s\n", filename);
        free(image.raster);
        exit(1);
    }

    // Create a filename
    char* croppedFilename = (char*) malloc(strlen(filename) + 12);

    // Cut off filename extension
    char* dot = strrchr(filename, '.');
    if (dot != NULL)
    {
        *dot = '\0';
    }

    // Copy the filename and add crop tag
    sprintf(croppedFilename, "%s [crop].tga", filename);

    // Check if the file already exists
    // int num_previous = 0;
    // while (access(croppedFilename, F_OK) != -1 && num_previous <= 99)
    // {
    //     if (num_previous == 99)
    //     {
    //         fprintf(stderr, "Too many files with the same name. Exiting...\n");
    //         exit(1);
    //     }
    //     num_previous++;
    // }
    // if (num_previous == 1)
    // {
    //     // Renaming of old, un-numbered [crop].tga file
    //     const char* croptag = " [crop 1].tga\0";
    //     const char* renamed_filename = strdup(filename);
    //     strcat(renamed_filename, croptag);
    //     rename(filename, renamed_filename);
    //     sprintf(croppedFilename, "%s [crop %d].tga", filename, num_previous);
    // } else
    // {
    //     sprintf(croppedFilename, "%s [crop %d].tga", filename, num_previous);
    // }

    
    // Write the image
    if (writeTGA(croppedFilename, &cropped) != 0)
    {
        fprintf(stderr, "Failed to write cropped image: %s\n",
                croppedFilename);
        free(croppedFilename);
        free(image.raster);
        free(cropped.raster);
        exit(1);
    }

    // Free memory
    free(croppedFilename);
    free(image.raster);
    free(cropped.raster);

    return 0;
}