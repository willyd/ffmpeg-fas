cmake_minimum_required(VERSION 2.8)

project(FFMPEG_FAS)

set(CMAKE_MODULE_PATH "${FFMPEG_FAS_SOURCE_DIR}/CMake")
set(FFMPEG_FAS_INCLUDE_DIR "${FFMPEG_FAS_SOURCE_DIR}/ffmpeg_fas")

#default locations
set(FFMPEG_FAS_RUNTIME_OUTPUT "${FFMPEG_FAS_BINARY_DIR}/bin")
set(FFMPEG_FAS_LIBRARY_OUTPUT "${FFMPEG_FAS_BINARY_DIR}/lib")

add_subdirectory(ffmpeg_fas)
add_subdirectory(test)
