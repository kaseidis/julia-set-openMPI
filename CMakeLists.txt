cmake_minimum_required(VERSION 3.13)

project(HW7)
include(CheckLanguage)

find_package(MPI)
find_package(OpenMP)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(bitmap bitmap.c)
target_include_directories(bitmap PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(julia_serial julia_serial.c)
target_include_directories(julia_serial PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(julia_serial m bitmap)

if (MPI_FOUND)
    add_library(mpi_type mpi_type.c)
    target_include_directories(mpi_type PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(mpi_type PUBLIC MPI::MPI_C) 

    add_executable(julia_mpi_static julia_mpi_static.c)
    target_include_directories(julia_mpi_static PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    if (OPENMP_FOUND)
        target_link_libraries(julia_mpi_static PUBLIC m mpi_type bitmap MPI::MPI_C OpenMP::OpenMP_C)
    else()
        target_link_libraries(julia_mpi_static PUBLIC m mpi_type bitmap MPI::MPI_C)
    endif()

    add_executable(julia_mpi_dynamic julia_mpi_dynamic.c)
    target_include_directories(julia_mpi_dynamic PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(julia_mpi_dynamic PUBLIC m mpi_type bitmap MPI::MPI_C)



endif()