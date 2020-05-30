#!/bin/bash
cproto bound.c buffer.c main.c menu.c tools.c ui.c photoionization.c atomic_data.c query.c \
       elements.c ions.c levels.c subtitle.c inner.c > functions.h
cproto log.c > log.h
