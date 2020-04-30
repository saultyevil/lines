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

// TODO: sort out includes ffs

#include <form.h>
#include <menu.h>
#include <curses.h>

/* ****************************************************************************
 *
 *                                  Macros
 *
 * ************************************************************************** */

#define ARRAY_SIZE(x) (sizeof x / sizeof x[0])
 
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/* ****************************************************************************
 *
 *                                Constants
 *
 * ************************************************************************** */

// UI Constants
#define VERSION "3.5"

#define NO_SCROLL 0
#define SCROLL_OK 1

#define MENU_QUIT -1
#define MENU_NULL -2
#define INDX_OTHR -3
#define ATOMIC_TEST -4
#define CONTROL_MENU TRUE
#define REDRAW_MENU FALSE

#define MAX_FIELD_INPUT 30
#define FIELD_SKIP -1
#define FORM_QUIT -1
#define FORM_BREAK -2
#define FORM_CONTINUE -3
#define NO_BG 999

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

typedef struct
{
  FIELD *field;
  Field_Options opts_on;
  Field_Options opts_off;
  chtype background;
  char buffer[MAX_FIELD_INPUT];
  int buffer_number;
} Query_t;

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
