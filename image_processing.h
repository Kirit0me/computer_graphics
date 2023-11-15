//code from https://github.com/ishaanvatus/image_processing

#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "image.h"


unsigned modulo(int value, unsigned m);
double kernel_min(Kernel kernel);
double kernel_max(Kernel kernel);
Accumulator convolve(Image image, Kernel kernel, int row, int col);
Image *apply_kernel(Image image, Kernel kernel);
Image *sobel(Image image);
Kernel sobel_y(int n);
Kernel sobel_x(int n);

#endif