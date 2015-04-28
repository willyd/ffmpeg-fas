include(ffmpeg_fas_config_test)
include_directories(${FFMPEG_FAS_SOURCE_DIR}/ffmpeg_fas)

find_package(OpenCV)
if(OpenCV_FOUND)
    add_executable(opencv_integration opencv_integration.cpp videoreader.cpp videoreader.h)    
    ffmpeg_fas_config_test(opencv_integration)
    target_link_libraries(opencv_integration ${OpenCV_LIBS})
endif()

