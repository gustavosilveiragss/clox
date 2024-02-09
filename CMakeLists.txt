cmake_minimum_required(VERSION 3.20)

project(imperative LANGUAGES C)

set(COMPILE_OPTIONS
    -pedantic
    -Wall
    -Werror
    -Wextra
)

# the shared library that is used by all
add_library(shared SHARED)
target_sources(shared PRIVATE
    lib/shared/src/Debug.c
    lib/shared/src/Memory.c
    lib/shared/src/Chunk.c
    lib/shared/src/Value.c
    lib/shared/src/Scanner.c
    lib/shared/src/Compiler.c
    lib/shared/src/VM.c
)
target_include_directories(shared PUBLIC lib/shared/include)
target_compile_options(shared PRIVATE ${COMPILE_OPTIONS})

function(add_standard_executable name)
    add_executable(${name})
    target_sources(${name} PRIVATE src/${name}/main.c)
    target_compile_options(${name} PRIVATE ${COMPILE_OPTIONS})
    target_link_libraries(${name} PRIVATE shared)
endfunction()

add_standard_executable(lox)
