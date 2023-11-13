#include <stdio.h>
#include "image.h"

int main()
{
    Image* mendelbroth = create_image(3000, 3000);
    // Image* red_img = create_image(1920, 1080);
    // Image* hw = create_image(100, 100);
    // Image* yor = load("yor.ppm");
    // Image* moddydoddy = create_image(720, 1080);
    // Pixel red;
    // red.r=255;red.g=0;red.b=0;
    // fill_image(red, red_img);
    // hello_world(hw);
    // mod_image(moddydoddy);
    // save_image(*moddydoddy, "mod.ppm");
    // save_image(*hw, "hw.ppm");
    // save_image(*yor, "copyyor.ppm");
    complex z_min, z_max;
    z_min.re = -2.0;
    z_max.re = 0.5;
    z_min.im = -1.25;
    z_max.im = 1.25;
    int iterations = 1000;

    mandelbrot(mendelbroth, z_min, z_max, iterations);
    save_image(*mendelbroth, "mendelbrot.ppm");
}
