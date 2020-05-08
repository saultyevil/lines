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

#include <form.h>
#include <menu.h>
#include <curses.h>

#define LINELEN 128
#define ATOMIX_VERSION_NUMBER "4.0"

typedef struct Config_t
{
  int rows, cols;
  int current_line, current_col;
  int atomic_data_loaded;
  char atomic_data[LINELEN];
} Config_t;

Config_t AtomixConfiguration;

/* ****************************************************************************
 * Buffer
 * ************************************************************************** */

#define SCROLL_DISBALE 0
#define SCROLL_ENABLE 1

#define atomic_summary_add(fmt, ...) \
{ \
  add_display(&ATOMIC_BUFFER, fmt, ##__VA_ARGS__); \
}

#define atomic_summary_show(scroll) \
{ \
  display_buffer(&ATOMIC_BUFFER, scroll); \
}

#define display_add(fmt, ...) \
{ \
  add_display(&DISPLAY_BUFFER, fmt, ##__VA_ARGS__); \
}

#define display_show(scroll) \
{\
  display_buffer(&DISPLAY_BUFFER, scroll);\
  clean_up_display(&DISPLAY_BUFFER); \
}

typedef struct Line_t
{
  int len;
  char *chars;
} Line_t;

typedef struct Display_t
{
  char name[LINELEN];
  int nlines;
  int maxlen;
  Line_t *lines;
} Display_t;

Display_t ATOMIC_BUFFER;
Display_t DISPLAY_BUFFER;

/* ****************************************************************************
 * Form
 * ************************************************************************** */

#define FORM_QUIT -1
#define FORM_BREAK -2
#define FORM_CONTINUE -3
#define FIELD_INPUT_LEN 30
#define FIELD_SKIP -1
#define FIELD_NO_BKG 999

typedef struct
{
  FIELD *field;
  Field_Options opts_on;
  Field_Options opts_off;
  chtype background;
  char buffer[FIELD_INPUT_LEN];
  int buffer_number;
} Query_t;

/* ****************************************************************************
 * Menu
 * ************************************************************************** */

#define MENU_QUIT -1
#define MENU_CONTINUE -2
#define INDEX_OTHER -3
#define ATOMIC_TEST -4
#define MENU_CONTROL TRUE
#define MENU_DRAW FALSE

typedef struct MenuItem_t
{
  void *func;
  int index;
  char *name;
  char *desc;
} MenuItem_t;

/* ****************************************************************************
 * UI
 * ************************************************************************** */

#define MIN_ROWS 35
#define MIN_COLS 130
#define MENU_WIDTH 24
#define TITLE_BAR_HEIGHT 2
#define STATUS_BAR_HEIGHT 1

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

/* ****************************************************************************
 * Misc
 * ************************************************************************** */

#define ELEMENT_NO_FOUND -1

#define ARRAY_SIZE(x) (sizeof x / sizeof x[0])
 
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#include "atomic.h"
#include "log.h"
#include "functions.h"
