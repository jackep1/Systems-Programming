#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "imageIO_TGA.h"
#include "operations.h"

int main(int argc, char** argv)
{
    // Need to have a filename
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    // File needs to be a .tga
    char* filename = argv[1];
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

    // Split the image into its channels
    RasterImage red = get_channel_raster(&image, 'r');
    RasterImage green = get_channel_raster(&image, 'g');
    RasterImage blue = get_channel_raster(&image, 'b');

    // Create filenames
    char* redFilename = malloc(strlen(filename) + 5);
    char* greenFilename = malloc(strlen(filename) + 5);
    char* blueFilename = malloc(strlen(filename) + 5);

    // Cut off extension
    char* dot = strrchr(filename, '.');
    if (dot != NULL)
    {
        *dot = '\0';
    }
    
    // Copy in filenames and add color channel
    strcpy(redFilename, filename); strcat(redFilename, " [r].tga");
    strcpy(greenFilename, filename); strcat(greenFilename, " [g].tga");
    strcpy(blueFilename, filename); strcat(blueFilename, " [b].tga");
    // sprintf(redFilename, "%s [r].tga", filename);
    // sprintf(greenFilename, "%s [g].tga", filename);
    // sprintf(blueFilename, "%s [b].tga", filename);

    // Write the images
    writeTGA(redFilename, &red);
    writeTGA(greenFilename, &green);
    writeTGA(blueFilename, &blue);

    *dot = '.';

    // Free memory
    free(redFilename);
    free(greenFilename);
    free(blueFilename);

    free(image.raster);
    free(red.raster);
    free(green.raster);
    free(blue.raster);

    return 0;
}