cmake_minimum_required(VERSION 3.10)
project(pyAtomic)
set(CMAKE_C_STANDARD 99)

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES
        src/atomic.h
        src/functions.h
        src/pyAtomic.h
        src/log.h
        src/log.c
        src/main.c
        src/read_atomic_data.c
        src/lines.c src/xsections.c src/tools.c)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(pyAtomic ${SOURCE_FILES})
target_link_libraries(pyAtomic m)
