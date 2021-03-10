#include "bitmap.h"
#include <complex.h>
#include <stdlib.h>

#define DATAINDEX(m, n, x, y) (y * m + x)

#define T 1000.0

/**
 * Generate julia set, store in data.
 * @param buf   memeroy location to store julia set
 * @param m     x-dimension
 * @param n     y-dimension
 */ 
void generate_julia_set(RgbTriple * buf, int m, int n) {
    float d_x = 3.0f / m;
    float d_y = 2.0f / n;
    float complex c = -0.7 + 0.26 * I;
    for (int x=0; x < m; ++x)
        for (int y=0; y < n; ++y) {
            float complex z = (-1.5+x*d_x) + (-1.0+y*d_y) * I;
            float t = 0.0;
            while (cabs(z)<4.0 && t<T) {
                z = z * z + c;
                t += 1.0f;
            }
            buf[DATAINDEX(m,n,x,y)].red = (int) (255 * t / T);
            buf[DATAINDEX(m,n,x,y)].green = (int) (255 * t / T);
            buf[DATAINDEX(m,n,x,y)].blue = (int) (255 * t / T);
        }
}

int main(int argc, char** argv) {
    int m,n;
    // Check arguments
    if (argc == 3)
    {
        sscanf(argv[1], "%d", &m);
        sscanf(argv[2], "%d", &n);
    }
    else
    {
        printf("Warning: Wrong argument count.\n");
        printf("\tUsage: %s [dimension-x] [dimension-y]\n", argv[0]);
        return -1;
    }
    // Allocate memeroy
    RgbTriple *data = (RgbTriple *) malloc(m * n * sizeof(RgbTriple));
    
    // Calculate julia set
    generate_julia_set(data,m,n);

    
    // Save julia set to bitmap
    save_bitmap(data, m, n, "julia.bmp");

    // Free memeroy location
    free(data);
}