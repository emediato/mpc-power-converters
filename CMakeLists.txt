cmake_minimum_required(VERSION 3.28)
project(MPCv1 C)

set(CMAKE_C_STANDARD 11)

add_executable(MPCv1 core/main.c core/app.h
        core/app.c)

# matrix test
add_executable(test_matrix tests/matrix.c core/matrix.c core/matrix.h)
target_include_directories(test_matrix PUBLIC ./core)
target_link_libraries(test_matrix m)
