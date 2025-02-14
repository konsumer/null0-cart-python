cmake_minimum_required(VERSION 3.13)
project(pocketpy_wasm C)

# enable colors for ninja
set(CMAKE_COLOR_DIAGNOSTICS ON)

add_executable(main.wasm main.c pocketpy.c)
set_target_properties(main.wasm
    PROPERTIES
    SUFFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
