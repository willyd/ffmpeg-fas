cmake_minimum_required(VERSION 3.0)

set (FFMPEG_FAS_BUILD_TESTS 1 CACHE BOOL "Set ON to build tests")

project(FFMPEG-FAS VERSION 1.0.0)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

set(PROJECT_EXPORT_SET_NAME "${PROJECT_NAME_LOWER}-targets")
set(PROJECT_BIN_INSTALL_PREFIX "bin")
set(PROJECT_LIB_INSTALL_PREFIX "staticlib")
if(BUILD_SHARED_LIBS)
  set(PROJECT_LIB_INSTALL_PREFIX "lib")
endif()
set(PROJECT_CMAKE_INSTALL_PREFIX "CMake")
set(PROJECT_INC_INSTALL_PREFIX "include")

set(PROJECT_CONFIG_FILE_TEMPLATE "${PROJECT_SOURCE_DIR}/cmake/package-config.cmakein")
set(PROJECT_CONFIG_FILE          "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME_LOWER}-config.cmake")
set(PROJECT_VERSION_FILE         "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME_LOWER}-config-version.cmake")

list(APPEND CMAKE_MODULE_PATH "${FFMPEG_FAS_SOURCE_DIR}/CMake")
set(FFMPEG_FAS_INCLUDE_DIR "${FFMPEG_FAS_SOURCE_DIR}/ffmpeg_fas")
list(APPEND PROJECT_LIBRARIES ffmpeg_fas)
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

find_package(FFMPEG REQUIRED)

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

include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_CONFIG_FILE_TEMPLATE}
                             ${PROJECT_CONFIG_FILE}
                             INSTALL_DESTINATION ${PROJECT_CMAKE_INSTALL_PREFIX}
                             PATH_VARS PROJECT_INC_INSTALL_PREFIX
                             )
write_basic_package_version_file(${PROJECT_VERSION_FILE}
                                COMPATIBILITY AnyNewerVersion
                                )

install(FILES ${PROJECT_CONFIG_FILE} ${PROJECT_VERSION_FILE} ${PROJECT_PROPS_FILE}
       DESTINATION ${PROJECT_CMAKE_INSTALL_PREFIX}
       )

install(EXPORT ${PROJECT_EXPORT_SET_NAME} 
       DESTINATION ${PROJECT_CMAKE_INSTALL_PREFIX}
       )
