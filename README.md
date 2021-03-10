# Homework 7 - MPCS HPC Winter 2021

## Requirement
See write-up at: https://www.dropbox.com/s/3ikenr4uqzs9fep/homework%207%20-%20mpcs%20hpc%20winter%202021.pdf?dl=0

## Author Name
Shenghua Chen

## Comments

### Files
Here is file list:
- ``julia_serial.c`` Code for Julia set generation with serial implementation.
- ``julia_mpi_static.c`` Code for Julia set generation with OpenMPI static decomposition.
- ``julia_mpi_dynamic.c`` Code for Julia set generation with OpenMPI dynamic load balancing.
- ``bitmap.c/h`` Code for generate bmp.
- ``mpi_type.c/h`` code for OpenMPI RGB struct based on ``RgbTriple`` in ``bitMap.h``

## Instruction for compile
Navigate to problem folder, run ``cmake .`` then ``make``.

Usage:
```
./julia_serial [dimension-x] [dimension-y]
mpirun -np [number of process] ./julia_mpi_static [dimension-x] [dimension-y]
mpirun -np [number of process] ./julia_mpi_dynamic [dimension-x] [dimension-y] [subdomain size]
```
