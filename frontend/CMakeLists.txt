execute_process(
    COMMAND lex -o ${CMAKE_CURRENT_SOURCE_DIR}/Lexer/VMLangLexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Lexer/VMLangLexer.lex
    COMMAND bison -o ${CMAKE_CURRENT_SOURCE_DIR}/Parser/VMLangParser.cpp -d ${CMAKE_CURRENT_SOURCE_DIR}/Parser/VMLangParser.y
)

set(FRONT_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Lexer/VMLangLexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/VMLangParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser/VMLangParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AST/AST.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AST/AST.h

    ${CMAKE_BINARY_DIR}/src/generated/Decoder.cpp
)

add_executable(VMFrontend ${FRONT_SOURCES})

set_target_properties(VMFrontend PROPERTIES COMPILE_OPTIONS "")

target_include_directories(VMFrontend
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Lexer
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Parser
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/AST
                           PUBLIC ${CMAKE_SOURCE_DIR}/include
                           PUBLIC ${CMAKE_BINARY_DIR}/src
)

