cmake_minimum_required(VERSION 3.28)
project(MPCv1 C)

set(CMAKE_C_STANDARD 11)

add_executable(MPCv1 core/main.c
        core/app.h)
