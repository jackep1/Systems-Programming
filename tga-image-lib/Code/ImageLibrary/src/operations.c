#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "operations.h"

int is_TGA(const char* filePath)
{
    const char* dot = strrchr(filePath, '.');
    if (!dot || dot == filePath) {
        return 0;
    }
    return strcmp(dot + 1, "tga") == 0;
}

int is_numeric(const char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

char* get_dimensions(const RasterImage* image)
{
    size_t dimensions_size = 20;
    char* dimensions = (char*)malloc(dimensions_size);

    // Simple dimensions
    snprintf(dimensions, dimensions_size, "%d %d", image->width, image->height);
    return dimensions;
}

RasterImage get_channel_raster(const RasterImage* image, const char channel_name)
{
    RasterImage channel = newImage(image->width, image->height, RGBA32_RASTER);
    int* raster = (int*)image->raster;
    int* channel_raster = (int*)channel.raster;

    for (unsigned int i = 0; i < image->height; i++)
    {
        for (unsigned int j = 0; j < image->width; j++)
        {
            unsigned char* rgba = (unsigned char*)(raster + i * image->width + j);
            unsigned char alpha = rgba[3];
            switch (channel_name)
            {
            case 'r':
                channel_raster[i * image->width + j] = (rgba[0] << 0) | (alpha << 24);
                break;
            case 'g':
                channel_raster[i * image->width + j] = (rgba[1] << 8) | (alpha << 24);
                break;
            case 'b':
                channel_raster[i * image->width + j] = (rgba[2] << 16) | (alpha << 24);
                break;
            case 'a':
                channel_raster[i * image->width + j] = (alpha << 24);
                break;
            default:
                break;
            }
        }
    }

    return channel;
}

int check_crop_rectangle(const crop_rectangle* flags, const RasterImage* image)
{
    if (flags->x_left < 0 || flags->y_top < 0 || flags->width < 0 || flags->height < 0)
    {
        return 1;
    }
    if (flags->x_left >= image->width || flags->y_top >= image->height)
    {
        return 2;
    }
    if (flags->x_left + flags->width > image->width || flags->y_top + flags->height > image->height)
    {
        return 3;
    }
    return 0;
}

RasterImage crop(const RasterImage* image, const crop_rectangle* flags)
{
    RasterImage cropped = newImage(flags->width, flags->height, RGBA32_RASTER);
    int* raster = (int*)image->raster;
    int* cropped_raster = (int*)cropped.raster;

    for (unsigned int i = 0; i < flags->height; i++)
    {
        for (unsigned int j = 0; j < flags->width; j++)
        {
            cropped_raster[i * flags->width + j] = raster[(i + flags->y_top) * image->width + j + flags->x_left];
        }
    }

    return cropped;
}

RasterImage rotate(const RasterImage* image, const rotate_flags* flags)
{
    if (flags->l)
    {
        RasterImage rotated = newImage(image->height, image->width, RGBA32_RASTER);
        int* raster = (int*)image->raster;
        int* rotated_raster = (int*)rotated.raster;
        for (unsigned int i = 0; i < image->height; i++)
        {
            for (unsigned int j = 0; j < image->width; j++)
            {
                rotated_raster[(image->width - j - 1) * image->height + i] = raster[i * image->width + j];
            }
        }
        return rotated;
    } else if (flags->ll || flags->rr)
    {
        RasterImage rotated = newImage(image->width, image->height, RGBA32_RASTER);
        int* raster = (int*)image->raster;
        int* rotated_raster = (int*)rotated.raster;
        for (unsigned int i = 0; i < image->height; i++)
        {
            for (unsigned int j = 0; j < image->width; j++)
            {
                rotated_raster[((image->height - i - 1) * image->width) + (image->width - j - 1)] = raster[i * image->width + j];
            }
        }
        return rotated;
    } else if (flags->r)
    {
        RasterImage rotated = newImage(image->height, image->width, RGBA32_RASTER);
        int* raster = (int*)image->raster;
        int* rotated_raster = (int*)rotated.raster;
        for (unsigned int i = 0; i < image->height; i++)
        {
            for (unsigned int j = 0; j < image->width; j++)
            {
                rotated_raster[j * image->height + image->height - i - 1] = raster[i * image->width + j];
            }
        }
        return rotated;
    } else
    {
        fprintf(stderr, "Invalid rotation flags\n");
        exit(1);
    }
}