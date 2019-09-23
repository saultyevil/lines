cmake_minimum_required(VERSION 3.10)
project(lines)
set(CMAKE_C_STANDARD 99)

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES
        src/main.c
        src/get_atomic_data.c
        src/rdpar.c
        src/synonyms.c
        src/xlog.c)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(lines ${SOURCE_FILES})
target_link_libraries(lines m)
