cmake_minimum_required(VERSION 3.5)

project(lossy_compress CXX)

add_executable(${PROJECT_NAME}_kmeans src/kmeans.cpp)
add_executable(${PROJECT_NAME}_lbg src/lbg.cpp)
add_executable(${PROJECT_NAME}_compressor src/compress.cpp)
add_executable(${PROJECT_NAME}_decompressor src/decompress.cpp)
