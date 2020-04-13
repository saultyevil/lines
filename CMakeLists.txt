cmake_minimum_required(VERSION 3.10)
project(atomix)
set(CMAKE_C_STANDARD 99)

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES
        src/atomic.h
        src/functions.h
        src/atomix.h
        src/log.h
        src/log.c
        src/main.c
        src/read_atomic_data.c
        src/lines.c
        src/xsections.c
        src/tools.c
        src/ui.c src/ui.h src/menu.c)

find_package(Curses REQUIRED)
find_library(menu REQUIRED)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(atomix ${SOURCE_FILES})
target_link_libraries(atomix m curses menu)
