#!/bin/bash
cproto bound.c buffer.c main.c menu.c tools.c ui.c photoionization.c read_atomic_data.c query.c > functions.h
cproto log.c > log.h