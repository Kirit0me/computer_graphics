#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "image.h"

Image* create_image(int w, int h)
{
    Image* image;
    
    image = malloc(sizeof(Image));
    if (image == NULL) 
        return NULL;
    
    image->pixels = malloc(sizeof(Pixel)*h*w);
    image->height = h;
    image->width = w;
    if(image->pixels == NULL){
        free(image);
        return NULL;
    }

    return image;
}

void free_image(Image* image)
{
    if (image != NULL) {
        if(image->pixels!=NULL) free(image->pixels);
        free(image);
        exit(30);
    }
}
Image* load(char* filename)
{
    Image *image = malloc(sizeof(Image));
    if (image == NULL) {
        fprintf(stderr, "Could not allocate memory to Image: %s\n", strerror(errno));
        return NULL;
    }
    char magic[3];
    int maxval;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        free(image);
        return NULL;
    }
    fscanf(fp, "%2s", magic);
    if (magic[0] != 'P' || magic[1] != '6') {
        fprintf(stderr, "Not a valid P6 ppm file: %s\n", strerror(errno));
        free(image);
        fclose(fp);
        return NULL;
    }
    fscanf(fp, "%d%d%*c", &image->width, &image->height);
    image->pixels = malloc(image->width*image->height*sizeof(Pixel));
    if (image->pixels == NULL) {
        free(image);
        fprintf(stderr, "Could not allocate memory to pixels: %s\n", strerror(errno));
        return NULL;
    }
    fscanf(fp, "%d%*c", &maxval);
    fread(image->pixels, sizeof(Pixel),image->width*image->height, fp);
    return image;
}

int save_image(Image image, char* fname)
{
    FILE *fp = fopen(fname, "wb");
    if(fp == NULL) return -1;
    fprintf(fp, "P6\n%d %d\n255\n", image.width, image.height);
    for(int i=0; i<image.height;i++){
        for(int j = 0;j<image.width;j++){
            fwrite(&image.pixels[i*image.width + j].r, sizeof(uint8_t), 1, fp);
            fwrite(&image.pixels[i*image.width + j].g, sizeof(uint8_t), 1, fp);
            fwrite(&image.pixels[i*image.width + j].b, sizeof(uint8_t), 1, fp);
        }
    }
    return 0;
}
void fill_image(Pixel color, Image* image)
{
    for (int i = 0; i < image->width*image->height; i++) {
        image->pixels[i] = color;
    }
}

void hello_world(Image* image)
{
    int h = image->height;
    int w = image->width;
    for(int i = 0; i<h; ++i) {
        for(int j = 0; j<w; ++j) {
            double re = 255.99*(i*1.0)/(h-1);
            double ge = 255.99*(j*1.0)/(w-1);

            image->pixels[i*w + j].r = re;
            image->pixels[i*w + j].g = ge;
            image->pixels[i*w + j].b = 0;
        }
    }
}

void mod_image(Image* image)
{
    for(int i=0; i<image->height;i++){
        for(int j = 0;j<image->width;j++){
            if(i%2 == 0){
                image->pixels[i*image->width + j].r=255;
                image->pixels[i*image->width + j].g=0;
                image->pixels[i*image->width + j].b=0;
            } else {
                image->pixels[i*image->width + j].b=255;
                image->pixels[i*image->width + j].r=0;
                image->pixels[i*image->width + j].g=0;
            }
        }
    }
}

void mandelbrot(Image* image, complex z_min, complex z_max, int maxIterations)
{
    int h = image->height;
    int w = image->width;
    double delta_re = (z_max.re - z_min.re) / w;
    double delta_im = (z_max.im - z_min.im) / h;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            complex z;
            z.re = z_min.re + j * delta_re;
            z.im = z_min.im + i * delta_im;

            complex x;
            x.re = x.im = 0.0;

            int iteration = 0;
            while (x.re * x.re + x.im * x.im <= 4 && iteration < maxIterations) {
                double x_re_squared = x.re * x.re;
                double x_im_squared = x.im * x.im;

                x.re = x_re_squared - x_im_squared + z.re;
                x.im = 2 * x.re * x.im + z.im;

                iteration++;
            }

            Pixel color;
            if (x.re * x.re + x.im * x.im <= 4) {
                color.r = 0;
                color.g = 0;
                color.b = 0;
            } else {
                color.r = (uint8_t)(1.0 + 254.99 * iteration / maxIterations);
                color.g = (uint8_t)(1.0 + 254.99 * iteration / maxIterations);
                color.b = (uint8_t)(1.0 + 254.99 * iteration / maxIterations);
            }

            image->pixels[i * w + j] = color;
        }
    }
}

void mandelbrot_p2(Image* image, complex z_min, complex z_max, int maxIterations) 
{
    int h = image->height;
    int w = image->width;
    double delta_re = (z_max.re - z_min.re) / w;
    double delta_im = (z_max.im - z_min.im) / h;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            complex z;
            z.re = z_min.re + j * delta_re;
            z.im = z_min.im + i * delta_im;

            complex x;
            x.re = x.im = 0.0;

            int iteration = 0;
            while (x.re * x.re + x.im * x.im <= 4 && iteration < maxIterations) {
                double x_re_squared = x.re * x.re;
                double x_im_squared = x.im * x.im;

                x = (complex){x_re_squared - x_im_squared + z.re, 2 * x.re * x.im + z.im};

                iteration++;
            }

            Pixel color;
            double color_value = 1.0 + 254.99 * iteration / maxIterations;
            if (x.re * x.re + x.im * x.im <= 4) {
                color = (Pixel){0, 0, 0};
            } else {
                color = (Pixel){(uint8_t)color_value, (uint8_t)color_value, (uint8_t)color_value};
            }

            image->pixels[i * w + j] = color;
        }
        fprintf(stderr, "%f\n", i*100.0/h);
    }
}

void julia(Image* image, complex c, complex z_min, complex z_max, int maxIterations)
{
    int h = image->height;
    int w = image->width;
    double delta_re = (z_max.re - z_min.re) / w;
    double delta_im = (z_max.im - z_min.im) / h;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            complex z;
            z.re = z_min.re + j * delta_re;
            z.im = z_min.im + i * delta_im;

            complex x;
            x.re = z.re;
            x.im = z.im;

            int iteration = 0;
            while (x.re * x.re + x.im * x.im <= 4 && iteration < maxIterations) {
                double x_re_temp = x.re * x.re - x.im * x.im + c.re;
                x.im = 2 * x.re * x.im + c.im;
                x.re = x_re_temp;

                iteration++;
            }

            Pixel color;
            if (iteration == maxIterations) {
                color.r = color.g = color.b = 0;
            } else {
                color.r = (uint8_t)(1.0 + 254.99 * iteration / maxIterations);
                color.g = (uint8_t)(1.0 + 254.99 * iteration / maxIterations);
                color.b = (uint8_t)(1.0 + 254.99 * iteration / maxIterations);
            }

            image->pixels[i * w + j] = color;
        }
        fprintf(stderr, "%f\r", i*100.0/h);
        fflush(stderr);
    }
}

void generate_julia_images(const char* folderPath, int numImages)
{
    complex z_min = {-2.0, -2.0};
    complex z_max = {2.0, 2.0};
    int iterations = 100;  // Fixed iteration value for the example
    Image* image = create_image(1440, 1080);

    for (int i = 0; i < numImages; ++i) {
        // Create an Image structure (replace with your actual Image struct)

        // Set the current constant value (varying for each image)
        complex c = {(double)i / numImages - 1.5, (double)i / numImages - 1.0};

        // Call your Julia set function with the current constant value
        julia(image, c, z_min, z_max, iterations);

        // Generate the output filename based on the constant values
        char outputFilename[100];
        snprintf(outputFilename, sizeof(outputFilename), "%s/julia_image_%04d.ppm", folderPath, i);
        save_image(*image, outputFilename);
        fprintf(stderr, "%d / %d \n", i, numImages);
    }
}

void bressenham_line(Image* image, Point a, Point b, Pixel color)
{
    int x = a.x, y = a.y;
    int dx = abs(b.x - a.x);
    int dy = -abs(b.y - a.y);
    int sx = (a.x < b.x) ? 1 : -1;
    int sy = (a.y < b.y) ? 1 : -1;
    int err = dy + dx, e;

    while (x != b.x || y != b.y) {
        image->pixels[y * image->width + x] = color;
		e = 2*err;
		if (e >= dy) {
			if (x == b.x) break;
			err += dy;
			x += sx;
		}
		if (e <= dx) {
			if (y == b.y) break;
			err += dx;
			y += sy;
		}
	}
}


void bressenham_circle(Image* image, int r, Pixel color)
{
    int x = 0, y = r, d = 3-2*r;
    while(x<=y) {
        image->pixels[y*image->width + x] = color;
        if(d<0) d = d + 4*x + 6;
        else {
            d = d + 4*(x-y) + 10;
            y--;
        }
        x++;
    }
}

void bressenham_circle_midpoint(Image* image, int r, Pixel color)
{
    int x = 0, y = r, p = 1-r;
    while(x<=y) {
        image->pixels[y*image->width + x] = color;
        if(p<0) p = p + 2*x + 3;
        else {
            p = p + 2*(x-y) + 5;
            y--;
        }
        x++;
    }
}

static void set_pixel(Image* image, int x, int y, Pixel color) 
{
    if (x >= 0 && x < image->width && y >= 0 && y < image->height) {
        image->pixels[y * image->width + x] = color;
    }
}

void draw_circle(Image* image, Point center, int radius, Pixel color)
{
    int x = 0, y = radius;
    int p = 1 - radius;

    set_pixel(image, center.x, center.y + radius, color);
    set_pixel(image, center.x, center.y - radius, color);
    set_pixel(image, center.x + radius, center.y, color);
    set_pixel(image, center.x - radius, center.y, color);

    while (x <= y) {
        if (p < 0) {
            p = p + 2 * x + 3;
        } else {
            p = p + 2 * (x - y) + 5;
            y--;
        }
        x++;

        set_pixel(image, center.x + x, center.y + y, color);
        set_pixel(image, center.x - x, center.y + y, color);
        set_pixel(image, center.x + x, center.y - y, color);
        set_pixel(image, center.x - x, center.y - y, color);
        set_pixel(image, center.x + y, center.y + x, color);
        set_pixel(image, center.x - y, center.y + x, color);
        set_pixel(image, center.x + y, center.y - x, color);
        set_pixel(image, center.x - y, center.y - x, color);
    }
}

void draw_koch_curve(Image* image, Point a, Point b, int depth, Pixel color) 
{
    if (depth == 0) {
        bressenham_line(image, a, b, color);
    } else {
        Point p1 = a;
        Point p2 = {a.x + (b.x - a.x) / 3, a.y + (b.y - a.y) / 3};
        Point p3 = {
            (int)(0.5 * (a.x + b.x) + sqrt(3) / 6 * (a.y - b.y)),
            (int)(0.5 * (a.y + b.y) + sqrt(3) / 6 * (b.x - a.x))
        };
        Point p4 = {a.x + 2 * (b.x - a.x) / 3, a.y + 2 * (b.y - a.y) / 3};
        Point p5 = b;

        draw_koch_curve(image, p1, p2, depth - 1, color);
        draw_koch_curve(image, p2, p3, depth - 1, color);
        draw_koch_curve(image, p3, p4, depth - 1, color);
        draw_koch_curve(image, p4, p5, depth - 1, color);
    }
}

void draw_koch_snowflake(Image* image, Point a, Point b, Point c, int depth, Pixel color)
{
    draw_koch_curve(image, a, b, depth, color);
    draw_koch_curve(image, b, c, depth, color);
    draw_koch_curve(image, c, a, depth, color);
}

