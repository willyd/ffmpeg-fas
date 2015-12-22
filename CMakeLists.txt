cmake_minimum_required(VERSION 2.8)

set (FFMPEG_FAS_BUILD_TESTS 1 CACHE BOOL "Set ON to build tests")

project(FFMPEG_FAS)

list(APPEND CMAKE_MODULE_PATH "${FFMPEG_FAS_SOURCE_DIR}/CMake")
set(FFMPEG_FAS_INCLUDE_DIR "${FFMPEG_FAS_SOURCE_DIR}/ffmpeg_fas")

#default locations
if(WIN32)
set(FFMPEG_FAS_RUNTIME_OUTPUT "${FFMPEG_FAS_BINARY_DIR}")
set(FFMPEG_FAS_LIBRARY_OUTPUT "${FFMPEG_FAS_BINARY_DIR}")
set(FFMPEG_FAS_ARCHIVE_OUTPUT "${FFMPEG_FAS_BINARY_DIR}")
else(WIN32)
set(FFMPEG_FAS_RUNTIME_OUTPUT "${FFMPEG_FAS_BINARY_DIR}/bin")
set(FFMPEG_FAS_LIBRARY_OUTPUT "${FFMPEG_FAS_BINARY_DIR}/lib")
set(FFMPEG_FAS_ARCHIVE_OUTPUT "${FFMPEG_FAS_BINARY_DIR}/lib")
endif(WIN32)

add_subdirectory(ffmpeg_fas)

if(${FFMPEG_FAS_BUILD_TESTS})
	add_subdirectory(test)
endif(${FFMPEG_FAS_BUILD_TESTS})

option(BUILD_SAMPLES "Build samples or not" OFF)
option(BUILD_PYTHON "Build python not" OFF)
if(BUILD_SAMPLES)
  add_subdirectory(sample)
endif()
if(BUILD_PYTHON)
  add_subdirectory(python)
endif()
