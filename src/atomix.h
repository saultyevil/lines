/* ************************************************************************** */
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
#define ATOMIX_VERSION_NUMBER "5.0"

/* ****************************************************************************
 * Configuration
 * ************************************************************************** */

typedef enum Screens
{
  sc_unassigned,
  sc_main_menu,
  sc_menu,
  sc_text_view,
  sc_atomic_view,
  sc_form,
} Screens;

typedef struct Config_t
{
  int rows, cols;
  int current_line, current_col;
  int atomic_data_loaded;
  char atomic_data[LINELEN];
  char status_message[LINELEN];
  Screens current_screen;
  MENU *current_menu;
  FORM *current_form;
} Config_t;

Config_t AtomixConfiguration;

/* ****************************************************************************
 * UI
 * ************************************************************************** */

#define MENU_WIDTH 25
#define STATUS_BAR_HEIGHT 1

typedef struct Window_t
{
  int start_y, start_x;
  int nrows, ncols;
  WINDOW *window;
} Window_t;

Window_t MAIN_MENU_WINDOW;
Window_t STATUS_BAR_WINDOW;
Window_t CONTENT_VIEW_WINDOW;

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

typedef struct MenuDump_t
{
  int n;
  int index;
  char *msg;
  MenuItem_t *m;
} MenuDump_t;

/* ****************************************************************************
 * Form
 * ************************************************************************** */

#define FORM_QUIT -1
#define FORM_BREAK -2
#define FORM_CONTINUE -3
#define FIELD_INPUT_LEN 30
#define FIELD_SKIP -1
#define FIELD_NO_BKG 999

typedef struct Query_t
{
  FIELD *field;
  Field_Options opts_on;
  Field_Options opts_off;
  chtype background;
  char buffer[FIELD_INPUT_LEN];
  int buffer_number;
} Query_t;

typedef struct QueryDump_t
{
  int n;
  char *msg;
  Query_t *q;

} QueryDump_t;

/* ****************************************************************************
 * Buffer
 * ************************************************************************** */

#define SCROLL_DISBALE 0
#define SCROLL_ENABLE 1
#define LINE_CENTER -1

#define atomic_summary_add(fmt, ...) \
{ \
  add_display(&ATOMIC_BUFFER, fmt, ##__VA_ARGS__); \
}

#define atomic_summary_show(scroll) \
{ \
  AtomixConfiguration.current_screen = sc_atomic_view; \
  display_buffer(&ATOMIC_BUFFER, scroll, false, 0); \
}

#define display_add(fmt, ...) \
{ \
  add_display(&DISPLAY_BUFFER, fmt, ##__VA_ARGS__); \
}

#define display_show(scroll, persistent_header, header_rows) \
{\
  AtomixConfiguration.current_screen = sc_text_view; \
  display_buffer(&DISPLAY_BUFFER, scroll, persistent_header, header_rows); \
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
 * Misc
 * ************************************************************************** */

#define ELEMENT_NO_FOUND -1

#define ARRAY_SIZE(x) (sizeof x / sizeof x[0])

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/* ****************************************************************************
 * Includes
 * ************************************************************************** */

#include "atomic.h"
#include "log.h"
#include "functions.h"
