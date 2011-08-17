cmake_minimum_required(VERSION 2.8)

include(ffmpeg_fas_config_test)
include_directories(${FFMPEG_FAS_SOURCE_DIR}/ffmpeg_fas)
include_directories(${FFMPEG_FAS_SOURCE_DIR}/test)

project(ffmpeg_fas_tests)

add_subdirectory(dump_frames)
add_subdirectory(dump_keyframes)
add_subdirectory(movie_info)
add_subdirectory(seek_test)
add_subdirectory(show_seek_table)

add_subdirectory(external_seek_test)
add_subdirectory(generate_seek_table)

