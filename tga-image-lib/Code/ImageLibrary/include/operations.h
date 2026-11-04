#include <string.h>
#include "imageIO_TGA.h"

typedef struct dimension_flags {
    int w;
    int h;
    int v;
} dimension_flags;

typedef struct crop_rectangle {
    int x_left;
    int y_top;
    int width;
    int height;
} crop_rectangle;

typedef struct rotate_flags {
    int l;
    int ll;
    int r;
    int rr;
} rotate_flags;

int is_TGA(const char* filePath);

int is_numeric(const char* str);

char* get_dimensions(const RasterImage* image);

RasterImage get_channel_raster(const RasterImage* image, const char channel_name);

int check_crop_rectangle(const crop_rectangle* flags, const RasterImage* image);

RasterImage crop(const RasterImage* image, const crop_rectangle* flags);

RasterImage rotate(const RasterImage* image, const rotate_flags* flags);