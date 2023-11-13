#include <stdio.h>
#include "image.h"

int main()
{
    Image* better_broth = create_image(1440, 1080);
    Image* mendelbroth = create_image(1000, 1000);
    Image* julia_img = create_image(1000, 1000);
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
    complex z_min, z_max, z_min_j, z_max_j, c;
    c.re = -0.74543;
    c.im = 0.11301;
    z_min.re = -2.0;
    z_max.re = 0.5;
    z_min.im = -1.25;
    z_max.im = 1.25;
    z_min_j.re = -1.2;
    z_max_j.re = 1.2;
    z_min_j.im = -1.2;
    z_max_j.im = 1.2;
    int iterations = 724*4;
    //julia(julia_img, c, z_min_j, z_max_j, iterations);
    //mandelbrot(mendelbroth, z_min, z_max, iterations);
    mandelbrot_p2(better_broth, z_min, z_max, iterations);
    //save_image(*mendelbroth, "mendelbrot.ppm");
    //save_image(*julia_img, "julia.ppm");
    save_image(*better_broth, "betterbroth.ppm");
}
