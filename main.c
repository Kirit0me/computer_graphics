#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "image.h"
#include "image_processing.h"

int main(int argc, char **argv)
{
    (void)argc;

    // Kernel box_blur;
    // box_blur.size = 21;
    // box_blur.weights = malloc(box_blur.size*box_blur.size*sizeof(double));
    // for (int row = 0; row < box_blur.size; row++) {
    //     for (int  col = 0; col < box_blur.size; col++)
    //         box_blur.weights[row*box_blur.size + col] = 1.0;
    // }

    // Image* pure = load(argv[1]);
    // Image* blur = apply_kernel(*pure, box_blur);
    // Image* sob = sobel(*pure);

    // save_image(*blur, "blur.ppm");
    // save_image(*sob, "sobel.ppm");
    // save_image(*pure, "identity.ppm");

    // Chapter 2

    // Image* better_broth = create_image(2160, 1440);
    // Image* mendelbroth = create_image(1000, 1000);
    // Image* julia_img = create_image(1000, 1000);
    // Image* red_img = create_image(1920, 1080);
    // Image* hw = create_image(100, 100);
    // Image* moddydoddy = create_image(720, 1080);
    // Pixel red;
    // red.r=255;red.g=0;red.b=0;
    // fill_image(red, red_img);
    // hello_world(hw);
    // mod_image(moddydoddy);
    // save_image(*moddydoddy, "mod.ppm");
    // save_image(*hw, "hw.ppm");
    // save_image(*yor, "copyyor.ppm");
    // complex z_min, z_max, z_min_j, z_max_j, c;
    // c.re = -0.74543;
    // c.im = 0.11301;
    // z_min.re = -2.0;
    // z_max.re = 0.5;
    // z_min.im = -1.25;
    // z_max.im = 1.25;
    // z_min_j.re = -1.2;
    // z_max_j.re = 1.2;
    // z_min_j.im = -1.2;
    // z_max_j.im = 1.2;
    // int iterations = 1500;
    // julia(julia_img, c, z_min_j, z_max_j, iterations);
    //mandelbrot(mendelbroth, z_min, z_max, iterations);
    // mandelbrot_p2(better_broth, z_min, z_max, iterations);
    //save_image(*mendelbroth, "mendelbrot.ppm");
    // save_image(*julia_img, "julia.ppm");
    //save_image(*better_broth, "betterbroth.ppm");

    // const char* folderPath = "julia_images";
    // mkdir(folderPath, 0777);
    // generate_julia_images(folderPath, 508);

    // Chapter 3

    Point a = {0, 0};
    Point b = {1000, 1000};
    Pixel red;
    red.r = 255;red.b = 0, red.g = 0;

    // Image* line = create_image(1200, 1200);
    // bressenham_line(line, a, b, red);
    // save_image(*line, "line.ppm");
    
    // Image* circle = create_image(1200, 1200);
    // draw_circle(circle, (Point){circle->width / 2, circle->height / 2}, 200, (Pixel){255, 0, 0});
    // save_image(*circle, "circle.ppm");
    
    Image* coke = create_image(1001, 1001);
    draw_koch_curve(coke, a, b, 6, red);
    save_image(*coke, "cococola.ppm");

}
