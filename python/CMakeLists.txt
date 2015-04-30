include(FindFFMPEG)

macro(make_python_string_list outvar)    
    foreach(item ${ARGN})
        list(APPEND ${outvar} "\"${item}\"")
    endforeach()
    string(REPLACE ";" ", " ${outvar} "${${outvar}}")
    set(${outvar} "[${${outvar}}]")    
endmacro()

# generate a list include and library dirs and libs to link to
if(WIN32)
    make_python_string_list(PYEXT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../ffmpeg_fas" "${MSINTTYPES_INCLUDE_DIR}")
else()
    make_python_string_list(PYEXT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../ffmpeg_fas")
endif()
make_python_string_list(PYEXT_LIB_DIRS ${CMAKE_BINARY_DIR})
make_python_string_list(PYEXT_LIBS "ffmpeg_fas")

## include dll dependencies
#if(WIN32)
#    file(GLOB FFMPEG_SHARED_LIBS "${FFMPEG_ROOT}/bin/*.dll")
#    set(PYEXT_SHARED_LIBS "${CMAKE_BINARY_DIR}/ffmpeg_fas.dll" ${FFMPEG_SHARED_LIBS})
#    foreach(_file ${PYEXT_SHARED_LIBS})
#        get_filename_component(_filename ${_file} NAME)
#        set(_outfile "${CMAKE_CURRENT_BINARY_DIR}/${_filename}")
#        list(APPEND PYEXT_SHARED_LIBS_NAME ${_filename})
#        list(APPEND PYEXT_OUTPUT_SHARED_LIBS ${_outfile})
#        add_custom_command(OUTPUT ${_outfile}
#                           COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_file} ${_outfile}
#                           COMMENT "Copying python extension source file: ${_file}"
#                           )
#    endforeach()
#    make_python_string_list(PYEXT_DATA_FILES ${PYEXT_SHARED_LIBS_NAME})
#endif()

# set the name of the extension and the extension file name
set(PYEXT_NAME "ffmpegfas")
set(PYEXT_FILE "ffmpegfas.pyx")

# configure the setup.py file
configure_file(setup.py.in ${CMAKE_CURRENT_BINARY_DIR}/setup.py)

set(PYEXT_INTPUT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/ffmpegfas.pyx" "${CMAKE_CURRENT_SOURCE_DIR}/cffmpegfas.pxd" )                          
foreach(_file ${PYEXT_INTPUT_SOURCES})
    get_filename_component(_filename ${_file} NAME)
    set(_outfile "${CMAKE_CURRENT_BINARY_DIR}/${_filename}")
    list(APPEND PYEXT_OUTPUT_SOURCES ${_outfile})
    add_custom_command(OUTPUT ${_outfile}
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_file} ${_outfile}
                       COMMENT "Copying python extension source file: ${_file}"
                       )
endforeach()

set(PYEXT_GENERATED_FILES "${CMAKE_CURRENT_BINARY_DIR}/ffmpegfas.c" 
                          "${CMAKE_CURRENT_BINARY_DIR}/ffmpegfas.pyd" # .pyd is Windows only beware!
                          "${CMAKE_CURRENT_BINARY_DIR}/build/"   
                          )
        
add_custom_command(OUTPUT ${PYEXT_GENERATED_FILES}
                   DEPENDS ${PYEXT_OUTPUT_SOURCES}                           
                   COMMAND python setup.py build_ext --inplace
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Compiling python extension with command:python setup.py build_ext --inplace --compiler=msvc" 
                   )

if(WIN32)
    set(BIN_DIRS "${CMAKE_BINARY_DIR}" "${FFMPEG_ROOT}/bin")
    string(REPLACE ";" "\\;" TARGET_DIRS "${BIN_DIRS}" )
    add_custom_command(OUTPUT "fixup"
                       DEPENDS ${PYEXT_GENERATED_FILES}                          
                       COMMAND ${CMAKE_COMMAND} -DTARGET_PATH="${CMAKE_CURRENT_BINARY_DIR}/ffmpegfas.pyd" -DTARGET_DIRS="${TARGET_DIRS}" -P ${CMAKE_CURRENT_SOURCE_DIR}/CopyDependencies.cmake 
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       COMMENT "Fixing up bundle" 
                       )
endif()

add_custom_target( python_interface ALL                                      
                   COMMENT "Building python extension" 
                   SOURCES setup.py.in ${PYEXT_INTPUT_SOURCES}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   DEPENDS ffmpeg_fas ${PYEXT_OUTPUT_SOURCES} ${PYEXT_GENERATED_FILES} ${PYEXT_OUTPUT_SHARED_LIBS} "fixup"
                   )


                