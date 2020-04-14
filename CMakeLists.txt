#
# atomix
# A tool to query and play around with the atomic data in Python.
#

project(atomix)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 99)

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES
        src/log.c
        src/main.c
        src/read_atomic_data.c
        src/bound.c
        src/xsections.c
        src/tools.c
        src/ui.c
        src/menu.c)

# The curses library is stored in various places depending on system
if(UNIX AND NOT APPLE)
        find_package(Curses REQUIRED)
        find_library(menu REQUIRED)
elseif(APPLE)
        include_directories(/usr/local/opt/ncurses/include)
        link_directories(/usr/local/opt/ncurses/lib)
endif()

# Create the atomix executable and link the libraries
add_executable(atomix ${SOURCE_FILES})
target_link_libraries(atomix m curses menu)
