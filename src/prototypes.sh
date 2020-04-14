#!/bin/bash
cproto lines.c main.c menu.c tools.c ui.c xsections.c > functions.h
cproto read_atomic_data.c > atomic_functions.h
cproto log.c > log_functions.h