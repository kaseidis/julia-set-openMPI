
#include <complex.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

#include "bitmap.h"
#include "mpi_type.h"

#define DATAINDEX(m, n, x, y) ((y) * (m) + (x))

#define T 1000.0

/**
 * Generate julia set row, store in data.
 * @param buf   memeroy location to store julia set row
 * @param m     x-dimension
 * @param n     y-dimension
 * @param x     row id for generate
 */
void generate_julia_set_row(RgbTriple *buf, int m, int n, int y)
{
    float d_x = 3.0f / m;
    float d_y = 2.0f / n;
    float complex c = -0.7 + 0.26 * I;
    for (int x = 0; x < m; ++x)
    {
        float complex z = (-1.5 + x * d_x) + (-1.0 + y * d_y) * I;
        float t = 0.0;
        while (cabs(z) < 4.0 && t < T)
        {
            z = z * z + c;
            t += 1.0f;
        }
        buf[x].red = (int)(255 * t / T);
        buf[x].green = (int)(255 * t / T);
        buf[x].blue = (int)(255 * t / T);
    }
}

/**
 * function for IO rank
 */ 
void io_main(int startY, int endY, int m, int n, int n_ranks)
{
    // Generate rgb type
    MPI_Datatype mpi_rgb_type;
    gen_rgb_struct(&mpi_rgb_type);
    int maxY = ceil(n / n_ranks);
    // Allocate memeroy
    RgbTriple *data = (RgbTriple *)malloc(m * n * sizeof(RgbTriple));
    RgbTriple *row = (RgbTriple *)malloc(m * maxY * sizeof(RgbTriple));

    // Calculate julia set and parally receives data
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int y = startY; y < endY; ++y)
                generate_julia_set_row(&(data[DATAINDEX(m, n, 0, y)]), m, n, y);
        }
        #pragma omp section
        {
            MPI_Status stat;
            for (int i = 1; i < n_ranks; ++i)
            {
                MPI_Recv(row, m * maxY, mpi_rgb_type, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
                int startY = stat.MPI_TAG;
                RgbTriple *rowPtr = &(data[DATAINDEX(m, n, 0, startY)]);
                int count;
                MPI_Get_count(&stat, mpi_rgb_type, &count);
                memcpy(rowPtr, row, sizeof(RgbTriple) * count);
            }
        }
    }
    // Save julia set to bitmap
    save_bitmap(data, m, n, "julia.bmp");
    // Free memeroy location
    free(data);
    free(row);
    MPI_Type_free(&mpi_rgb_type);
}

/**
 * function for calculation rank
 */ 
void calc_main(int startY, int endY, int m, int n, int rank)
{
    // Generate rgb type
    MPI_Datatype mpi_rgb_type;
    gen_rgb_struct(&mpi_rgb_type);
    // Allocate memeroy
    RgbTriple *row = (RgbTriple *)malloc(m * (endY - startY) * sizeof(RgbTriple));

    // Calculate julia set by assigned row
    for (int y = startY; y < endY; ++y)
    {
        RgbTriple *rowPtr = &(row[DATAINDEX(m, n, 0, y - startY)]);
        generate_julia_set_row(rowPtr, m, n, y);
    }

    MPI_Send(row, m * (endY - startY), mpi_rgb_type, 0, startY, MPI_COMM_WORLD);

    // Free memeroy location
    free(row);
    MPI_Type_free(&mpi_rgb_type);
}

int main(int argc, char **argv)
{
    // Init MPI
    MPI_Init(&argc, &argv);

    int n_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Check arguments
    int m, n;
    if (argc == 3)
    {
        sscanf(argv[1], "%d", &m);
        sscanf(argv[2], "%d", &n);
    }
    else
    {
        if (rank == 0)
        {
            printf("Warning: Wrong argument count.\n");
            printf("\tUsage: %s [dimension-x] [dimension-y]\n", argv[0]);
        }
        MPI_Finalize();
        return -1;
    }

    // Calculate startX endX
    size_t startY = (int)(((float)rank) / n_ranks * n);
    size_t endY = (int)(((float)rank + 1) / n_ranks * n);
    if (rank == 0) // IO Rank
        io_main(startY,endY, m, n, n_ranks);
    else // Calculation only rank
        calc_main(startY,endY, m, n, rank);
    MPI_Finalize();
}