include(GetPrerequisites)
get_filename_component(TARGET_DIR "${TARGET_PATH}" PATH)
get_prerequisites("${TARGET_PATH}" REQS 1 1 "${TARGET_PATH}" "${TARGET_DIRS}")
foreach(_req ${REQS})
    if(NOT _req MATCHES "python[0-9][0-9]\\.dll") # ignore python*.dll dependency
        gp_resolve_item("${TARGET_PATH}" ${_req} "${TARGET_PATH}" "${TARGET_DIRS}" RESOLVED_REQ)        
        file(COPY ${RESOLVED_REQ} DESTINATION ${TARGET_DIR})
    endif()
endforeach()
