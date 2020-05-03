/** ************************************************************************* */
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
  keypad (stdscr, FALSE);
  curs_set (0);

  if (AtomixConfiguration.rows < MIN_ROWS || AtomixConfiguration.cols < MIN_COLS)
    exit_atomix (EXIT_FAILURE, "Minimum terminal size of 130x35 but current terminal size is %ix%i",
                 AtomixConfiguration.cols, AtomixConfiguration.rows);
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
  log_close ();
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

  max_rows = AtomixConfiguration.rows;
  max_cols = AtomixConfiguration.cols;

  // The window containing the main menu and sub menus
  MENU_WINDOW.rows = max_rows - TITLE_BAR_HEIGHT - STATUS_BAR_HEIGHT;
  MENU_WINDOW.cols = MENU_WIDTH;
  MENU_WINDOW.y = TITLE_BAR_HEIGHT;
  MENU_WINDOW.x = 0;
  MENU_WINDOW.win = newwin (MENU_WINDOW.rows, MENU_WINDOW.cols, MENU_WINDOW.y, MENU_WINDOW.x);
  if (MENU_WINDOW.win == NULL)
    exit_atomix (EXIT_FAILURE, "initialise_main_window s: unable to allocate memory for MENU_WINDOW");
  keypad (MENU_WINDOW.win, TRUE);

  // The window containing the main title and
  TITLE_WINDOW.rows = TITLE_BAR_HEIGHT;
  TITLE_WINDOW.cols = max_cols;
  TITLE_WINDOW.y = 0;
  TITLE_WINDOW.x = 0;
  TITLE_WINDOW.win = newwin (TITLE_WINDOW.rows, TITLE_WINDOW.cols, TITLE_WINDOW.y, TITLE_WINDOW.x);
  if (TITLE_WINDOW.win == NULL)
    exit_atomix (EXIT_FAILURE, "initialise_main_window s: unable to allocate memory for TITLE_WINDOW");

  // The window for the status bar at the bottom
  STATUS_WINDOW.rows = STATUS_BAR_HEIGHT;
  STATUS_WINDOW.cols = max_cols;
  STATUS_WINDOW.y = max_rows - STATUS_BAR_HEIGHT;
  STATUS_WINDOW.x = 0;
  STATUS_WINDOW.win = newwin (STATUS_WINDOW.rows, STATUS_WINDOW.cols, STATUS_WINDOW.y, STATUS_WINDOW.x);
  if (STATUS_WINDOW.win == NULL)
    exit_atomix (EXIT_FAILURE, "initialise_main_window s: unable to allocate memory for STATUS_WINDOW");

  // The window for displaying content to the user
  CONTENT_WINDOW.rows = max_rows - TITLE_BAR_HEIGHT - STATUS_BAR_HEIGHT;
  CONTENT_WINDOW.cols = max_cols - MENU_WIDTH;
  CONTENT_WINDOW.y = TITLE_BAR_HEIGHT;
  CONTENT_WINDOW.x = MENU_WIDTH;
  CONTENT_WINDOW.win = newwin (CONTENT_WINDOW.rows, CONTENT_WINDOW.cols, CONTENT_WINDOW.y, CONTENT_WINDOW.x);
  if (CONTENT_WINDOW.win == NULL)
    exit_atomix (EXIT_FAILURE, "initialise_main_window s: unable to allocate memory for CONTENT_WINDOW");
  keypad (CONTENT_WINDOW.win, TRUE);
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
  int i, j;
  int len;
  char *subtitle;
  char title[LINELEN];

  wattron (TITLE_WINDOW.win, A_REVERSE | A_BOLD);
  wattron (STATUS_WINDOW.win, A_REVERSE);
  wattron (MENU_WINDOW.win, A_REVERSE);

  // This is for the top, title window
  for (j = 0; j < TITLE_WINDOW.rows; ++j)
    for (i = 0; i < TITLE_WINDOW.cols; ++i)
      mvwprintw (TITLE_WINDOW.win, j, i, " ");
	len = sprintf (title, "atomix : version %s", ATOMIX_VERSION_NUMBER);
  mvwprintw (TITLE_WINDOW.win, 0, (TITLE_WINDOW.cols - len) / 2, title);

  // This is for the bottom, status bar
  for (j = 0; j < STATUS_WINDOW.rows; ++j)
    for (i = 0; i < STATUS_WINDOW.cols; ++i)
      mvwprintw (STATUS_WINDOW.win, j, i, " ");
  update_status_bar ("Press q to quit when in the main menu");

  // This creates a 1 column boundary between the menu and content window
  for (j = 0; j < MENU_WINDOW.rows; ++j)
    mvwprintw (MENU_WINDOW.win, j, MENU_WINDOW.cols - 1, " ");

  wattroff (STATUS_WINDOW.win, A_REVERSE);
  wattroff (TITLE_WINDOW.win, A_REVERSE | A_BOLD);
  wattroff (MENU_WINDOW.win, A_REVERSE);

	// This is for the subtitle
	wattron (TITLE_WINDOW.win, A_REVERSE | A_ITALIC);
	subtitle = get_random_subtitle();
	len = strlen (subtitle);
	mvwprintw (TITLE_WINDOW.win, 1, (TITLE_WINDOW.cols - len) / 2, subtitle);
	wattroff (TITLE_WINDOW.win, A_REVERSE | A_ITALIC);

  wrefresh (TITLE_WINDOW.win);
  wrefresh (STATUS_WINDOW.win);
  wrefresh (MENU_WINDOW.win);
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
bold_message (WINDOW *win, int y, int x, char *fmt, ...)
{
  int len;
  char *msg;
  va_list va, va_c;

  va_start (va, fmt);
  va_copy (va_c, va);

  len = vsnprintf (NULL, 0, fmt, va);
  msg = malloc (len * sizeof (char));
  len = vsprintf (msg, fmt, va_c);

  wattron (win, A_BOLD | A_UNDERLINE);
  mvwprintw (win, y, x, "%s", msg);
  wattroff (win, A_BOLD | A_UNDERLINE);

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
  char *msg;
  va_list va, va_c;

  wattron (STATUS_WINDOW.win, A_REVERSE);
  for (int i = 0; i < STATUS_WINDOW.cols; ++i)
    mvwprintw (STATUS_WINDOW.win, 0, i, " ");

  va_start (va, fmt);
  va_copy (va_c, va);

  len = vsnprintf (NULL, 0, fmt, va);
  msg = malloc (len * sizeof (char) + 1);
  len = vsprintf (msg, fmt, va_c);
  msg[len] = '\0';  // I don't trust vsprintf actually

  va_end (va);
  va_end (va_c);

  mvwprintw (STATUS_WINDOW.win, 0, 1, msg);
  wattroff (STATUS_WINDOW.win, A_REVERSE);

  wrefresh (STATUS_WINDOW.win);
}
