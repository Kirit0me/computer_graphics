#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}Pixel;

typedef struct{
    int height;
    int width;
    Pixel* pixels;
}Image;

typedef struct{
    double re;
    double im;
}complex;

Image* create_image(int w, int h);
void free_image(Image* image);
int save_image(Image image, char* fname);
void fill_image(Pixel color, Image* image);
void hello_world(Image* image);
void mod_image(Image* image);
Image* load(char* filename);
void mandelbrot(Image* image, complex z_min, complex z_max, int maxIterations);
void julia(Image* image, complex c, complex z_min, complex z_max, int maxIterations);
void mandelbrot_p2(Image* image, complex z_min, complex z_max, int maxIterations);
#endif