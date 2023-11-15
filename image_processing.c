#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "image.h"
#include "image_processing.h"

Image *grayscale(Image image)
{
    Image *gray = create_image(image.width, image.height);
    Pixel pix;
    int index;
	uint8_t avg;
	for (int row = 0; row < image.height; row++) {
		for (int col = 0; col < image.width; col++) {
            index = row*image.width + col;
			pix = image.pixels[index];
			avg = (uint8_t) ((pix.r + pix.g + pix.b)/3.0);
			gray->pixels[index] = (Pixel) {avg, avg, avg};
		}
	}
    return gray;
}

Image *perceptual_grayscale(Image image)
{
    Image *gray = create_image(image.width, image.height);
    Pixel pix;
	uint8_t bt_601;
    int index;
	for (int row = 0; row < image.height; row++) {
		for (int col = 0; col < image.width; col++) {
            index = row*image.width + col;
            pix = image.pixels[index];
            bt_601 = (uint8_t) (0.2126*pix.r + 0.7152*pix.g + 0.0722*pix.b);
			gray->pixels[index] = (Pixel) {bt_601, bt_601, bt_601};
		}
	}
    return gray;
}

double kernel_min(Kernel kernel)
{
    double min = 0;
    for (int index = 0; index < kernel.size*kernel.size; index++) {
        if (kernel.weights[index] < 0)
            min += kernel.weights[index];
    }
    return min*255;
}

double kernel_max(Kernel kernel)
{
    double max = 0;
    for (int index = 0; index < kernel.size*kernel.size; index++) {
        if (kernel.weights[index] > 0)
            max += kernel.weights[index];
    }
    return max*255;
}

Accumulator convolve(Image image, Kernel kernel, int row, int col)
{
    Accumulator accumulator = {0, 0, 0};
    for (int r_off = -kernel.size/2; r_off <= kernel.size/2; r_off++) {
        for (int c_off = -kernel.size/2; c_off <= kernel.size/2; c_off++) {
            int ir = modulo(row + r_off, image.height);
            int ic = modulo(col + c_off, image.width);
            int kr = r_off + kernel.size/2;
            int kc = c_off + kernel.size/2;
            int index = ir*image.width + ic;
            Pixel pixel = image.pixels[index];

            accumulator.r += pixel.r*kernel.weights[kr*kernel.size + kc];
            accumulator.g += pixel.g*kernel.weights[kr*kernel.size + kc];
            accumulator.b += pixel.b*kernel.weights[kr*kernel.size + kc];
        }
    }
    return accumulator;
}

Image *apply_kernel(Image image, Kernel kernel)
{
    Image *conv = create_image(image.width, image.height);
    double max = kernel_max(kernel);
    double min = kernel_min(kernel);
    double alpha = max - min;
    for (int row = 0; row < image.height; row++) {
        for (int col = 0; col < image.width; col++) {
            Accumulator accumulator = convolve(image, kernel, row, col);

            accumulator.r = 255*(accumulator.r - min)/alpha;
            accumulator.g = 255*(accumulator.g - min)/alpha;
            accumulator.b = 255*(accumulator.b - min)/alpha;

            conv->pixels[row*image.width + col].r = accumulator.r;
            conv->pixels[row*image.width + col].g = accumulator.g;
            conv->pixels[row*image.width + col].b = accumulator.b;
            }
        printf("%lf\r", 100.0*(1.0*row)/image.height);
        fflush(stdout);
        }
    return conv;
}

Kernel sobel_x(int n)
{
    Kernel sx;
    sx.size = 3;
    sx.weights = malloc(sizeof(double)*sx.size*sx.size);
    sx.weights[0] = -1;
    sx.weights[1] = -2;
    sx.weights[2] = -1;
    sx.weights[3] = 0;
    sx.weights[4] = 0;
    sx.weights[5] = 0;
    sx.weights[6] = 1;
    sx.weights[7] = 2;
    sx.weights[8] = 1;
    return sx;
}
Kernel sobel_y(int n)
{
    Kernel sy;
    sy.size = 3;
    sy.weights = malloc(sy.size*sy.size*sizeof(double));
    sy.weights[0] = -1;
    sy.weights[1] = 0;
    sy.weights[2] = 1;
    sy.weights[3] = -2;
    sy.weights[4] = 0;
    sy.weights[5] = 2;
    sy.weights[6] = -1;
    sy.weights[7] = 0;
    sy.weights[8] = 1;
    return sy;
}

Image *sobel(Image image)
{
    Image *conv = create_image(image.width, image.height);
    Image *sobx, *soby;
    Kernel sx = sobel_x(3);
    Kernel sy = sobel_y(3);
    sobx = apply_kernel(image, sx);
    soby = apply_kernel(image, sy);
    save_image(*sobx, "sobel_x.ppm");
    save_image(*soby, "sobel_y.ppm");
    double max = kernel_max(sx);
    double min = kernel_min(sx);
    double alpha = max - min;
    for (int row = 0; row < image.height; row++) {
        for (int col = 0; col < image.width; col++) {
            Accumulator x = convolve(image, sx, row, col);
            Accumulator y = convolve(image, sy, row, col);
            x.r = 255*(x.r)/alpha;
            x.g = 255*(x.g)/alpha;
            x.b = 255*(x.b)/alpha;

            y.r = 255*(y.r)/alpha;
            y.g = 255*(y.g)/alpha;
            y.b = 255*(y.b)/alpha;

            Accumulator gradient = {
                sqrt(x.r*x.r + y.r*y.r),
                sqrt(x.g*x.g + y.g*y.g),
                sqrt(x.b*x.b + y.b*y.b),
            };
            gradient.r = (gradient.r > 255)? 255: gradient.r;
            gradient.g = (gradient.g > 255)? 255: gradient.g;
            gradient.b = (gradient.b > 255)? 255: gradient.b;
            conv->pixels[row*image.width + col].r = (uint8_t) gradient.r;
            conv->pixels[row*image.width + col].g = (uint8_t) gradient.g;
            conv->pixels[row*image.width + col].b = (uint8_t) gradient.b;
            }
        printf("%lf\r", 100.0*(1.0*row)/image.height);
        fflush(stdout);
        }
    return conv;
}

unsigned modulo(int value, unsigned m)
{
	int mod = value % (int) m;
	if (mod < 0)
        mod += m;
	return mod;
}
