#
# atomix
# A tool to query and play around with the atomic data in Python. The
# description of this tool changes every time I write one :-).
#

project(atomix)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra -g)

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES
        src/log.c
        src/main.c
        src/atomic_data.c
        src/lines.c
        src/photoionization.c
        src/tools.c
        src/ui.c
        src/menu.c
        src/buffer.c
        src/query.c
        src/elements.c
        src/ions.c
        src/levels.c
        src/inner.c
        src/parse.c
        )

# The curses library is stored in various places depending on system
if(UNIX AND NOT APPLE)
        find_package(Curses REQUIRED)
        find_library(menu REQUIRED)
        find_library(form REQUIRED)
elseif(APPLE)
        include_directories(/usr/local/opt/ncurses/include)
        link_directories(/usr/local/opt/ncurses/lib)
endif()

# Create the atomix executable and link the libraries
add_executable(atomix ${SOURCE_FILES})
target_link_libraries(atomix m curses menu form)
