/** ************************************************************************* */
/**
 * @file     atomix.h
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * The main include file for atomix.
 *
 * ************************************************************************** */

/* ****************************************************************************
 *
 *                                  Macros
 *
 * ************************************************************************** */

#define ARRAY_SIZE(x) (sizeof x / sizeof x[0])

/* ****************************************************************************
 *
 *                                Constants
 *
 * ************************************************************************** */

// UI Constants
#define VERSION "3.0a"
#define MAX_ROWS_SUB_WIN LINES - 4
#define MAX_COLS_SUB_WIN COLS - 2
#define MENU_QUIT -1

// Various other constants
#define const_C_SI 299792458
#define LINELEN 256

/* ****************************************************************************
 *
 *                                  Types
 *
 * ************************************************************************** */

typedef struct
{
  int nlines;
  char **lines;
} ScreenBuffer_t;

#define SB_INIT {0, NULL};

typedef struct
{
  size_t len;
  char *buffer;
} Line_t;

#define LINE_INIT {0, NULL}

/* ****************************************************************************
 *
 *                                  Macros
 *
 * ************************************************************************** */

// TODO clean up includes for final release

#include <menu.h>
#include <curses.h>

#include "atomic.h"

#include "log.h"
#include "functions.h"
