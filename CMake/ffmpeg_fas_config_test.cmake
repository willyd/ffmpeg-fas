
function(ffmpeg_fas_config_test TARGETNAME)
	set_target_properties(${TARGETNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${FFMPEG_FAS_RUNTIME_OUTPUT})
	add_dependencies(${TARGETNAME} ffmpeg_fas)
	target_link_libraries(${TARGETNAME} ffmpeg_fas)
endfunction(ffmpeg_fas_config_test)