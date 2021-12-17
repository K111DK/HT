cmake_minimum_required(VERSION 3.20)
project(HF C)

set(CMAKE_C_STANDARD 99)

add_executable(HF main.c BasicStruct.h Compress.h DeCompress.h utils.h)
