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

// TODO: sort out includes

#include <form.h>
#include <menu.h>
#include <curses.h>

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
#define VERSION "3.1a"

#define MENU_QUIT -1
#define MENU_NULL -2
#define CONTROL_MENU TRUE
#define REDRAW_MENU FALSE

#define MENU_WIDTH 24
#define TITLE_HEIGHT 2
#define STATUS_HEIGHT 1

// Various other constants
#define const_C_SI 299792458
#define LINELEN 256

/* ****************************************************************************
 *
 *                                  Types
 *
 * ************************************************************************** */

typedef struct Line_t
{
  int len;
  char *chars;
} Line_t;

typedef struct Display_t
{
  int nlines;
  Line_t *lines;
} Display_t;

typedef struct MenuItem_t
{
  void *usrptr;
  int index;
  char *name;
  char *desc;
} MenuItem_t;

typedef struct Window_t
{
  int y, x;
  int rows, cols;
  WINDOW *win;
} Window_t;

/* ****************************************************************************
 *
 *                                  Includes
 *
 * ************************************************************************** */

// TODO: sort out includes

#include "atomic.h"
#include "log.h"
#include "functions.h"

/* ****************************************************************************
 *
 *                                Globals
 *
 * ************************************************************************** */

Window_t MENU_WINDOW;
Window_t TITLE_WINDOW;
Window_t STATUS_WINDOW;
Window_t CONTENT_WINDOW;

/* ****************************************************************************
 *
 *                                Configuration
 *
 * ************************************************************************** */

struct
{
  char *atomic_data;
} AtomixConfiguration;
