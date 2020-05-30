/* ************************************************************************** */
/**
 * @file     ui.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for creating the ncurses ui.
 *
 * ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief    Initialise ncurses and the standard screen
 *
 * @return   void
 *
 * @details
 *
 * This function is a wrapper to initialise ncurses. Currently, this
 * initialises the stdscr screen.
 *
 * There is a minimum terminal size of 130x35 enforced until scrolling works.
 *
 * ************************************************************************** */

void
initialise_ncurses_stdscr (void)
{
  initscr ();
  getmaxyx (stdscr, AtomixConfiguration.rows, AtomixConfiguration.cols);

  noecho ();
  cbreak ();
  keypad (stdscr, false);
  curs_set (0);

  refresh ();
}

/* ************************************************************************** */
/**
 * @brief    Cleans up the ncurses screen
 *
 * @return   void
 *
 * @details
 *
 * The function is a wrapper to clean up the ncurses screen. Currently, this
 * only ends the stdstr screen but this may change in the future.
 *
 * ************************************************************************** */

void
cleanup_ncurses_stdscr (void)
{
  logfile_close();
  delwin (MAIN_MENU_WINDOW.window);
  delwin (STATUS_BAR_WINDOW.window);
  delwin (CONTENT_VIEW_WINDOW.window);
  endwin ();
}

/* ************************************************************************** */
/**
 * @brief  Initialise the main panels for atomix.
 *
 * @details
 *
 * This function populates the global Window_t variables for each panel. Each
 * panel is given a width, height and start y and start x coordinates as well
 * given a WINDOW variable.
 *
 * For the menu window, keypad is enabled so the keyboard is usable.
 *
 * The aim of this style of global variables is to have each panel available
 * throughout the entire program, so any function is able to update the
 * screen.
 *
 * ************************************************************************** */

void
initialise_main_windows (void)
{
  int max_rows, max_cols;
  int origin_row, origin_col;

  max_rows = AtomixConfiguration.rows - 2;
  max_cols = AtomixConfiguration.cols - 2;
  origin_row = origin_col = 1;

  /*
   * Status bar for messages: horizontal bar across the bottom
   */

  STATUS_BAR_WINDOW.nrows = STATUS_BAR_HEIGHT;
  STATUS_BAR_WINDOW.ncols = max_cols;
  STATUS_BAR_WINDOW.start_y = origin_row + max_rows - STATUS_BAR_WINDOW.nrows;
  STATUS_BAR_WINDOW.start_x = origin_col;
  STATUS_BAR_WINDOW.window = newwin (STATUS_BAR_WINDOW.nrows, STATUS_BAR_WINDOW.ncols, STATUS_BAR_WINDOW.start_y, STATUS_BAR_WINDOW.start_x);
  if (STATUS_BAR_WINDOW.window == NULL)
    exit_atomix (EXIT_FAILURE, "initialise_main_windows : unable to allocate memory for STATUS_WINDOW");

  /*
   * Main menu: vertical bar on the left
   */

  MAIN_MENU_WINDOW.nrows = max_rows - STATUS_BAR_WINDOW.nrows;
  MAIN_MENU_WINDOW.ncols = MENU_WIDTH;
  MAIN_MENU_WINDOW.start_y = origin_row;
  MAIN_MENU_WINDOW.start_x = origin_col;
  MAIN_MENU_WINDOW.window = newwin (MAIN_MENU_WINDOW.nrows, MAIN_MENU_WINDOW.ncols, MAIN_MENU_WINDOW.start_y, MAIN_MENU_WINDOW.start_x);
  if (MAIN_MENU_WINDOW.window == NULL)
    exit_atomix (EXIT_FAILURE, "initialise_main_windows : unable to allocate memory for MENU_WINDOW");
  keypad (MAIN_MENU_WINDOW.window, true);
  nodelay (MAIN_MENU_WINDOW.window, true);

  /*
   * The content window: the workhorse where everything is displayed
   */

  CONTENT_VIEW_WINDOW.nrows = max_rows - STATUS_BAR_WINDOW.nrows;
  CONTENT_VIEW_WINDOW.ncols = max_cols - MAIN_MENU_WINDOW.ncols;
  CONTENT_VIEW_WINDOW.start_y = origin_row;
  CONTENT_VIEW_WINDOW.start_x = origin_col + MAIN_MENU_WINDOW.ncols;
  CONTENT_VIEW_WINDOW.window = newwin (CONTENT_VIEW_WINDOW.nrows, CONTENT_VIEW_WINDOW.ncols, CONTENT_VIEW_WINDOW.start_y, CONTENT_VIEW_WINDOW.start_x);
  if (CONTENT_VIEW_WINDOW.window == NULL)
    exit_atomix (EXIT_FAILURE, "initialise_main_windows: unable to allocate memory for CONTENT_WINDOW");
  keypad (CONTENT_VIEW_WINDOW.window, true);
  nodelay (CONTENT_VIEW_WINDOW.window, true);
}

/* ************************************************************************** */
/**
 * @brief  Draw the boundaries and backgrounds for the different windows.
 *
 * @details
 *
 * Draws the top title bar, as well as the title and subtitle. To separate the
 * menu from the content window, a 1 column boundary is draw using A_REVERSE.
 * The status bar background is also drawn, but is not updated with a message.
 *
 * ************************************************************************** */

void
draw_window_boundaries (void)
{
  int len;
  char title[LINELEN];

  box (stdscr, 0, 0);
	len = sprintf (title, "| atomix : version %s |", ATOMIX_VERSION_NUMBER);
  mvprintw (0, AtomixConfiguration.cols - len - 2, title);
  wrefresh (stdscr);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * TODO make this work -> currently the menu isn't redrawn for some reason
 *
 * ************************************************************************** */

void
redraw_screen (int sig)
{
  (void) sig;  // Suppress unused variable warning

  delwin (MAIN_MENU_WINDOW.window);
  delwin (CONTENT_VIEW_WINDOW.window);
  delwin (STATUS_BAR_WINDOW.window);
  endwin ();

  initialise_ncurses_stdscr ();
  initialise_main_windows ();

  clear ();
  wclear (MAIN_MENU_WINDOW.window);
  wclear (CONTENT_VIEW_WINDOW.window);
  wclear (STATUS_BAR_WINDOW.window);

  draw_window_boundaries ();
  main_menu (MENU_DRAW);
  update_status_bar (AtomixConfiguration.status_message);

  exit_atomix (EXIT_FAILURE, "Terminal resizing is not yet supported");

  /*
   * The problem seems to happen in menus -- when the menu is resized, it is
   * instantly drawn over or something
   */
}

/* ************************************************************************** */
/**
 * @brief  Write a bold message in the current window.
 *
 * @param[in]  win  The window to write the bold message in.
 * @param[in]  y    The y location of the message.
 * @param[in]  x    The x location of the message.
 * @param[in]  msg  The message to be display.
 * @param[in]  ...  The variable arguments for the message.
 *
 * @details
 *
 * Simply acts as a wrapper function to write a bold message. Should usually
 * only be used for a single message which is being made bold, otherwise you
 * may encounter performance issues.
 *
 * ************************************************************************** */

void
bold_message (Window_t win, int y, int x, char *fmt, ...)
{
  int len;
  char *msg;
  va_list va, va_c;
  WINDOW *window = win.window;

  va_start (va, fmt);
  va_copy (va_c, va);

  len = vsnprintf (NULL, 0, fmt, va);
  msg = malloc (len * sizeof (char));
  len = vsprintf (msg, fmt, va_c);

  if (x == LINE_CENTER)
    x = (win.ncols - len) / 2;

  wattron (window, A_BOLD);
  mvwprintw (window, y, x, "%s", msg);
  wattroff (window, A_BOLD);

  va_end (va);
  va_end (va_c);

  free (msg);
}

/* ************************************************************************** */
/**
 * @brief  Update the status bar with a message
 *
 * @param[in]  fmt  The formatted string
 * @param[in]  ...  The string arguments
 *
 * @details
 *
 * ************************************************************************** */

void
update_status_bar (char *fmt, ...)
{
  int len;
  char *tmpmsg;
  char *msg;
  va_list va, va_c;

  wclear (STATUS_BAR_WINDOW.window);

  va_start (va, fmt);
  va_copy (va_c, va);

  len = vsnprintf (NULL, 0, fmt, va);
  tmpmsg = malloc (len * sizeof (char) + 1);
  len = vsprintf (tmpmsg, fmt, va_c);

  va_end (va);
  va_end (va_c);

  msg = malloc (len * sizeof (char) + 5);
  len = sprintf (msg, "| %s |", tmpmsg);
  msg[len] = '\0';  // I don't trust sprintf (sometimes)

  mvwprintw (STATUS_BAR_WINDOW.window, 0, 1, msg);
  wrefresh (STATUS_BAR_WINDOW.window);
  strcpy (AtomixConfiguration.status_message, tmpmsg);

  free (tmpmsg);
  free (msg);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
home_screen (void)
{
  int i, crow;
  int nlines;
  WINDOW *window = CONTENT_VIEW_WINDOW.window;

  char *logo[] = {
    "        _                  _      ",
    "   __ _| |_ ___  _ __ ___ (_)_  __",
    "  / _` | __/ _ \\| '_ ` _ \\| \\ \\/ /",
    " | (_| | || (_) | | | | | | |>  < ",
    "  \\__,_|\\__\\___/|_| |_| |_|_/_/\\_\\"
  };

  wclear (window);
  nlines = ARRAY_SIZE (logo);
  crow = 2;

  for (i = 0; i < nlines; ++i)
    mvwprintw (window, crow++, (CONTENT_VIEW_WINDOW.ncols - strlen (logo[i])) / 2, logo[i]);

  crow++;

  bold_message (CONTENT_VIEW_WINDOW, crow++, LINE_CENTER, "Version %s", ATOMIX_VERSION_NUMBER);
  bold_message (CONTENT_VIEW_WINDOW, crow++, LINE_CENTER, "Author: Edward J. Parkinson");
  bold_message (CONTENT_VIEW_WINDOW, crow++, LINE_CENTER, "Contact: e.j.parkinson@soton.ac.uk");

  crow += 2;

  bold_message (CONTENT_VIEW_WINDOW, crow++, LINE_CENTER, "A summary of the atomic data is in 'Atomic Summary'");

  wrefresh (window);
}
