file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)

add_custom_target(generate_all)

set(TEMPLATES 
    ${CMAKE_CURRENT_SOURCE_DIR}/templates/instruction_enum.h.temp
    ${CMAKE_CURRENT_SOURCE_DIR}/templates/Decoder.cpp.temp
)

foreach(TEMPL ${TEMPLATES})
    get_filename_component(OUTPUT_NAME_TEMPL ${TEMPL} NAME_WLE)
    get_filename_component(TARGET_NAME ${TEMPL} NAME_WE)

    gen_file(
        TEMPLATE ${TEMPL}
        ISA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/isa/isa.yml
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/${OUTPUT_NAME_TEMPL}
    )

    add_custom_target(${TARGET_NAME}_gen DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/generated/${OUTPUT_NAME_TEMPL})
    add_dependencies(generate_all ${TARGET_NAME}_gen)
endforeach()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)