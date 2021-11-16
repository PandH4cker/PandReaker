cmake_minimum_required(VERSION 3.20)
project(PandReaker C)

set(CMAKE_C_STANDARD 99)
find_package(OpenMP REQUIRED)

add_executable(PandReaker src/main.c src/breakerFor.c includes/breakerFor.h)
target_link_libraries(PandReaker PRIVATE OpenMP::OpenMP_C crypt)
