cmake_minimum_required(VERSION 2.8)


include(FindFFMPEG)


set(FFMPEG_FAS_SOURCES
ffmpeg_fas.c
seek_indices.c )

set(FFMPEG_FAS_HEADERS
ffmpeg_fas.h
private_errors.h
seek_indices.h )

if(MSVC)
include_directories(${FFMPEG_INCLUDE_DIRS} ${MSINTTYPES_INCLUDE_DIR})
else(MSVC)
include_directories(${FFMPEG_INCLUDE_DIRS})
endif(MSVC)

add_library(ffmpeg_fas SHARED ${FFMPEG_FAS_SOURCES} ${FFMPEG_FAS_HEADERS})

set_target_properties(ffmpeg_fas PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FFMPEG_FAS_RUNTIME_OUTPUT}")
set_target_properties(ffmpeg_fas PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${FFMPEG_FAS_LIBRARY_OUTPUT}")
set_target_properties(ffmpeg_fas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${FFMPEG_FAS_ARCHIVE_OUTPUT}")
target_link_libraries(ffmpeg_fas ${FFMPEG_LIBRARIES} ${FFMPEG_LIBSWSCALE_LIBRARIES} )
