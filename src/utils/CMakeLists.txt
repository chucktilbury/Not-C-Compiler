
project(utils)

add_library(${PROJECT_NAME} STATIC
    errors.c
    configure.c
    hash_table.c
    ptr_lists.c
    data_lists.c
    stacks.c
    tok_to_strg.c
    memory.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-g" "-D_DEBUGGING"
        "-I/usr/lib/llvm-7/include"
        "-D_GNU_SOURCE"
        "-D__STDC_CONSTANT_MACROS"
        "-D__STDC_FORMAT_MACROS"
        "-D__STDC_LIMIT_MACROS" )

