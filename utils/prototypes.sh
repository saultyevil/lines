#!/bin/bash
cproto bound.c buffer.c main.c menu.c tools.c ui.c xsections.c read_atomic_data.c > functions.h
cproto log.c > log.h