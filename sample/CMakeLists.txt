function(MAKE_SAMPLE NAME_SAMPLE)
    add_executable(${NAME_SAMPLE} ${CMAKE_CURRENT_SOURCE_DIR}/${NAME_SAMPLE}.cpp)
    target_link_libraries(${NAME_SAMPLE} PUBLIC XML-Parser)	
	
	target_compile_definitions(${NAME_SAMPLE} 
	PUBLIC
		-D SAMPLE_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/"
	)
		
	set_target_properties(${NAME_SAMPLE} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_INSTALL_PREFIX}/bin/")
	
	install(TARGETS ${NAME_SAMPLE})	
endfunction()

# MAKE_SAMPLE("ReadMe")

MAKE_SAMPLE("Example-01")
MAKE_SAMPLE("Example-02")
MAKE_SAMPLE("Example-03")
MAKE_SAMPLE("Example-04")
MAKE_SAMPLE("Example-05")
