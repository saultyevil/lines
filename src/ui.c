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
 * TODO: add something to the status bar
 * TODO: function for updating the status bar
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
 * ************************************************************************** */

void
initialise_ncurses_stdscr (void)
{
  initscr ();
  clear ();
  noecho ();      // Most screens don't want echo
  cbreak ();      // Disable line buffering or summat
  keypad (stdscr, FALSE);  // Allow the screen to take input
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
  Log_close ();
  endwin ();
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
menu_exit_atomix (void)
{
  exit (EXIT_SUCCESS);
}

/* ************************************************************************** */
/**
 * @brief  Intialise the main panels for atomix.
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
  // The window containing the main menu and sub menus
  MENU_WINDOW.rows = LINES - TITLE_HEIGHT - STATUS_HEIGHT;
  MENU_WINDOW.cols = MENU_WIDTH;
  MENU_WINDOW.y = TITLE_HEIGHT;
  MENU_WINDOW.x = 0;
  MENU_WINDOW.win = newwin (MENU_WINDOW.rows, MENU_WINDOW.cols, MENU_WINDOW.y, MENU_WINDOW.x);
  keypad (MENU_WINDOW.win, TRUE);

  // The window containing the main title and
  TITLE_WINDOW.rows = TITLE_HEIGHT;
  TITLE_WINDOW.cols = COLS;
  TITLE_WINDOW.y = 0;
  TITLE_WINDOW.x = 0;
  TITLE_WINDOW.win = newwin (TITLE_WINDOW.rows, TITLE_WINDOW.cols, TITLE_WINDOW.y, TITLE_WINDOW.x);

  // The window for the status bar at the bottom
  STATUS_WINDOW.rows = STATUS_HEIGHT;
  STATUS_WINDOW.cols = COLS;
  STATUS_WINDOW.y = LINES - STATUS_HEIGHT;
  STATUS_WINDOW.x = 0;
  STATUS_WINDOW.win = newwin (STATUS_WINDOW.rows, STATUS_WINDOW.cols, STATUS_WINDOW.y, STATUS_WINDOW.x);

  // The window for displaying content to the user
  CONTENT_WINDOW.rows = LINES - TITLE_HEIGHT - STATUS_HEIGHT;
  CONTENT_WINDOW.cols = COLS - MENU_WIDTH;
  CONTENT_WINDOW.y = TITLE_HEIGHT;
  CONTENT_WINDOW.x = MENU_WIDTH;
  CONTENT_WINDOW.win = newwin (CONTENT_WINDOW.rows, CONTENT_WINDOW.cols, CONTENT_WINDOW.y, CONTENT_WINDOW.x);
  scrollok (CONTENT_WINDOW.win, TRUE);
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
  char title[LINELEN];
	char subtitle[LINELEN];

  wattron (TITLE_WINDOW.win, A_REVERSE | A_BOLD);
  wattron (STATUS_WINDOW.win, A_REVERSE);
  wattron (MENU_WINDOW.win, A_REVERSE);

  // This is for the top, title window
  for (j = 0; j < TITLE_WINDOW.rows; ++j)
    for (i = 0; i < TITLE_WINDOW.cols; ++i)
      mvwprintw (TITLE_WINDOW.win, j, i, " ");
	len = sprintf (title, "atomix : version %s", VERSION);
  mvwprintw (TITLE_WINDOW.win, 0, (TITLE_WINDOW.cols - len) / 2, title);

  // This is for the bottom, status bar
  for (j = 0; j < STATUS_WINDOW.rows; ++j)
    for (i = 0; i < STATUS_WINDOW.cols; ++i)
      mvwprintw (STATUS_WINDOW.win, j, i, " ");
  mvwprintw (STATUS_WINDOW.win, 0, 1, "Press q to quit when in the main menu");

  // This creates a 1 column boundary between the menu and content window
  for (j = 0; j < MENU_WINDOW.rows; ++j)
    mvwprintw (MENU_WINDOW.win, j, MENU_WINDOW.cols - 1, " ");

  wattroff (STATUS_WINDOW.win, A_REVERSE);
  wattroff (TITLE_WINDOW.win, A_REVERSE | A_BOLD);
  wattroff (MENU_WINDOW.win, A_REVERSE);

	// This is for the subtitle
	wattron (TITLE_WINDOW.win, A_REVERSE | A_ITALIC);
  len = sprintf (subtitle, "To boldly probe atomic data where others simply don't dare or want to...");
	mvwprintw (TITLE_WINDOW.win, 1, (TITLE_WINDOW.cols - len) / 2, subtitle);
	wattroff (TITLE_WINDOW.win, A_REVERSE | A_ITALIC);

  wrefresh (TITLE_WINDOW.win);
  wrefresh (STATUS_WINDOW.win);
  wrefresh (MENU_WINDOW.win);
}

/* ************************************************************************** */
/**
 * @brief      Write a bold message in the current window.
 *
 * @param[in]  win    The window to write the bold message in.
 * @param[in]  y      The y location of the message.
 * @param[in]  x      The x location of the message.
 * @param[in]  msg    The message to be display.
 * @param[in]  ...    The variable arguments for the message.
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


