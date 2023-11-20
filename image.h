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

typedef struct {
    double r;
    double g;
    double b;
} Accumulator;

typedef struct {
    int size;
    double *weights;
} Kernel;

typedef struct{
    double re;
    double im;
}complex;

typedef struct {
    int x;
    int y;
}Point;

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
void generate_julia_images(const char* folderPath, int numImages);

void draw_circle(Image* image, Point center, int radius, Pixel color);
void bressenham_line(Image* image, Point a, Point b, Pixel color);
void bressenham_circle(Image* image, int r, Pixel color);
void bressenham_circle_midpoint(Image* image, int r, Pixel color);

void draw_koch_curve(Image* image, Point a, Point b, int depth, Pixel color);
void draw_koch_snowflake(Image* image, Point a, Point b, Point c, int depth, Pixel color);

#endif