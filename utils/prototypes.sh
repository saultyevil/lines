#!/bin/bash
cproto ../src/lines.c ../src/main.c ../src/menu.c ../src/tools.c ../src/ui.c ../src/xsections.c > ../src/functions.h
cproto ../src/read_atomic_data.c > ../src/atomic_functions.h
cproto ../src/log.c > ../src/log_functions.h