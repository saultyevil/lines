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
  size_t len;
  char *chars;
} Line_t;

#define LINE_INIT {0, NULL}

struct Display_t
{
  int nlines;
  Line_t *lines;
};

struct Display_t DISPLAY;

/* ****************************************************************************
 *
 *                                  Includes
 *
 * ************************************************************************** */

#include <menu.h>
#include <curses.h>

#include "atomic.h"
#include "log.h"
#include "functions.h"


/* ****************************************************************************
 *
 *                                Windows
 *
 * ************************************************************************** */

typedef struct Window_t
{
  int y, x;
  int rows, cols;
  WINDOW *win;
} Window_t;

Window_t MENU_WINDOW;
Window_t TITLE_WINDOW;
Window_t STATUS_WINDOW;
Window_t CONTENT_WINDOW;

#define MENU_WIDTH 24
#define TITLE_HEIGHT 2
#define STATUS_HEIGHT 1
