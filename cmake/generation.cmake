find_package(Python3 REQUIRED)

execute_process(
    COMMAND pip show cogapp
    RESULT_VARIABLE EXIT_CODE
    OUTPUT_QUIET
)

if (NOT ${EXIT_CODE} EQUAL 0)
    message(
        FATAL_ERROR
        "The \"cog\" Python3 package is not installed. Please install it using the following command: \"pip3 install cogapp\"."
    )
endif()

function(gen_file)
    set(singlevalues TEMPLATE ISA_FILE OUTPUT)
	set(multiplevalues REQUIES)
    cmake_parse_arguments(
        ARG
        ""
        "${singlevalues}"
		"${multiplevalues}"
		${ARGN}
    )
    set(DEPENDS_LIST ${ARG_TEMPLATE} ${ARG_ISA_FILE})
	add_custom_command(
        OUTPUT ${ARG_OUTPUT}
        COMMENT "Generate file for ${ARG_TEMPLATE}"
        COMMAND ${Python3_EXECUTABLE} -m cogapp -D isa_file=${ARG_ISA_FILE} -o ${ARG_OUTPUT} ${ARG_TEMPLATE} 
        DEPENDS ${DEPENDS_LIST}
    )
endfunction()