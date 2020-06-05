
project(parser)

include_directories(${PROJECT_SOURCE_DIR}/../include)

set(LEX_FILE      ${CMAKE_CURRENT_SOURCE_DIR}/scanner.l)
set(LEX_C_SOURCE  ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c)
set(LEX_H         ${PROJECT_INCLUDE_DIRECTORY}/scanner.h)

#add_custom_target(unionLexHeader DEPENDS ${LEX_H})
add_custom_command(OUTPUT ${LEX_C_SOURCE}
        DEPENDS ${LEX_FILE}
        PRE_BUILD
        COMMAND flex -o scanner.c scanner.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC
    parser.c
    scanner.c
    parse_data_or_func_def.c
    parse_import.c
    parse_typedef.c
    types.c
)

#find_package(LLVM)
# execute_process(COMMAND llvm-config --cflags
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     OUTPUT_VARIABLE LLVM
#     RESULT_VARIABLE LLVM_result )

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
        #${PROJECT_SOURCE_DIR}/../parser_support
)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra" "-g" "-D_DEBUGGING"
        "-I/usr/lib/llvm-7/include"
        "-D_GNU_SOURCE"
        "-D__STDC_CONSTANT_MACROS"
        "-D__STDC_FORMAT_MACROS"
        "-D__STDC_LIMIT_MACROS" )

