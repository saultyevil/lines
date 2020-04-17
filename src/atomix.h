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
#define VERSION "3.1a"
#define MENU_QUIT -1
#define MENU_NULL -2

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

struct Line_t
{
  int len;
  char *chars;
};

struct Display_t
{
  int nlines;
  struct Line_t *lines;
};

struct MenuItem_t
{
  void *usrptr;
  int index;
  char *name;
  char *desc;
};

/* ****************************************************************************
 *
 *                                  Includes
 *
 * ************************************************************************** */

#include <form.h>
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

struct Window_t
{
  int y, x;
  int rows, cols;
  WINDOW *win;
} Window_t;

/* ****************************************************************************
 *
 *                                Globals
 *
 * ************************************************************************** */

struct Display_t DISPLAY;
struct Window_t MENU_WINDOW;
struct Window_t TITLE_WINDOW;
struct Window_t STATUS_WINDOW;
struct Window_t CONTENT_WINDOW;
