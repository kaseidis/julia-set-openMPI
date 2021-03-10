#include <mpi.h>
#include <stdlib.h>
#include "bitmap.h"

void gen_rgb_struct(MPI_Datatype *mpi_rgb_type) {
    // Create MPI struct
    const int nitems=3;
    int          blocklengths[3] = {1,1,1};
    MPI_Datatype types[3] = {MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR};
    MPI_Aint     offsets[3];
    offsets[0] = offsetof(RgbTriple, blue);
    offsets[1] = offsetof(RgbTriple, green);
    offsets[2] = offsetof(RgbTriple, red);
    MPI_Type_create_struct(nitems, blocklengths, offsets, types, mpi_rgb_type);
    MPI_Type_commit(mpi_rgb_type);
}
