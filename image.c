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
            double be = 0;

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
        fprintf(stderr, "%f\n", i*100.0/h);
    }
}


