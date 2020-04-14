#!/bin/bash
cproto lines.c main.c menu.c tools.c ui.c xsections.c read_atomic_data.c > functions.h
cproto log.c > log.h