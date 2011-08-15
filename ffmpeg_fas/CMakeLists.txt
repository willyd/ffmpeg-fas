cmake_minimum_required(VERSION 2.8)
include(../CMake/FindFFMPEG.cmake)

project(ffmpeg_fas)

set(FFMPEG_FAS_SOURCES
ffmpeg_fas.c
seek_indices.c )

set(FFMPEG_FAS_HEADERS
ffmpeg_fas.h
private_errors.h
seek_indices.h )


add_library(ffmpeg_fas SHARED ${FFMPEG_FAS_SOURCES} ${FFMPEG_FAS_HEADERS})
set_target_properties(ffmpeg_fas PROPERTIES LIBRARY_OUTPUT_DIRECTORY ..)


#this will become uncommented once pkg-config is removed from the build sys
#to increase compatibility with windows.
#target_link_libraries(ffmpeg_fas avcodec avformat avutil swscale)
