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
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief    Initialise the ncurses screen
 *
 * @return   void
 *
 * @details
 *
 * This function is a wrapper to initialise the ncurses screen. Currently, this
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
  keypad (stdscr, TRUE);  // Allow the screen to take input
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
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
draw_window_boundaries (void)
{
  int i, j;
  int len;
  char title[LINELEN];

  wattron (TITLE_WINDOW.win, A_REVERSE);
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

  // This creates a 1 column boundary between the menu and content window
  for (j = 0; j < MENU_WINDOW.rows; ++j)
    mvwprintw (MENU_WINDOW.win, j, MENU_WINDOW.cols - 1, " ");

  wattroff (STATUS_WINDOW.win, A_REVERSE);
  wattroff (TITLE_WINDOW.win, A_REVERSE);
  wattroff (MENU_WINDOW.win, A_REVERSE);

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
 * TODO add variable argument code
 *
 * ************************************************************************** */


void
bold_message (WINDOW *win, int y, int x, char *msg, ...)
{
  wattron (win, A_BOLD);
  mvwprintw (win, y, x, "%s", msg);
  wattroff (win, A_BOLD);
}

/* ************************************************************************** */
/**
 * @brief     Query the user for the atomic data name.
 *
 * @details
 *
 * Will keep asking the user for an atomic data name until the data can be read
 * in without error, or until the user quits the program.
 *
 * TODO graceful exit for this function - q key?
 *
 * ************************************************************************** */

int
query_atomic_data (void)
{
  int atomic_data_error;
  int valid = FALSE;
  char atomic_data_name[LINELEN];
  Line_t sb = LINE_INIT;

  WINDOW *win = CONTENT_WINDOW.win;

  wclear (win);

  echo ();

  bold_message (win, 1, 2, "Please input the atomic data master file name:");

  while (valid != TRUE)
  {
    mvwprintw (win, 3, 2, "> ");
    wmove (win, 3, 4);
    wrefresh (win);

    wscanw (win, "%s", atomic_data_name);
    if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
      strcat (atomic_data_name, ".dat");

    atomic_data_error = get_atomic_data (atomic_data_name, &sb);

    if (atomic_data_error)
    {
      // TODO increase verbosity of error messages, i.e. write out actual error
      mvwprintw (win, 5, 2, "Invalid atomic data provided, try again.");
      mvwprintw (win, 6, 2, "Error number: %i\n", atomic_data_error);
      wmove (win, 3, 0);
      wclrtoeol (win);
    }
    else
    {
      // TODO write out atomic data diagnostic output to screen
      valid = TRUE;
    }

    wrefresh (win);
  }

  noecho ();

  display_text_buffer (win, 1, 1);

  return valid;
}

/* ************************************************************************** */
/**
 * @brief      Query a wavelength value from the user.
 *
 * @param[in]  win  The window to place the query in.
 * @param[in]  msg  A message to prompt the user what to input.
 * @param[in]  y    The y location of the prompt message.
 * @param[in]  x    The x location of the prompt message.
 *
 * @return     The wavelength input by the user.
 *
 * @details
 *
 * Originally a function to avoid code duplication. The input value should be
 * echo'd back to the user, but once the value will be updated on the screen.
 *
 * TODO check for empty input
 *
 * ************************************************************************** */

double
get_wavelength (WINDOW *win, char *msg, int y, int x, int len)
{
  double wl;

  mvwprintw (win, y, x, "%s", msg);
  wrefresh (win);
  wscanw (win, "%lf", &wl);
  mvwprintw (win, y, len + 2, "%5.1f Angstroms", wl);
  wrefresh (win);

  return wl;
}

/* ************************************************************************** */
/**
 * @brief       Get the wavelength range to consider.
 *
 * @param[out]  wmin   The smallest wavelength transition to find.
 * @param[out]  wmax   The largest wavelength transition to find.
 *
 * @details
 *
 * This function will keep looping until either the user quits, or until the
 * input is correct; i.e., wmin < wmax. Echo mode is temporarily turned on for
 * this function, so the user can see what they are typing.
 *
 * TODO graceful exit of this function
 *
 * ************************************************************************** */

void
query_wavelength_range (double *wmin, double *wmax)
{
  int valid = FALSE;
  WINDOW *win = CONTENT_WINDOW.win;

  wclear (win);

  echo ();

  while (valid != TRUE)
  {
    bold_message (win, 1, 2, "Please input the wavelength range to query:");

    *wmin = get_wavelength (win, "Minimum wavelength range: > ", 3, 2, 28);
    *wmax = get_wavelength (win, "Maximum wavelength range: > ", 5, 2, 28);

    // TODO: some scheme for either waiting or pressing a key

    if (*wmax > *wmin)
    {
      valid = TRUE;
    }
    else
    {
      bold_message (win, 7, 2, "Invalid input!");
      wrefresh (win);
      sleep (2);
    }
  }

  noecho ();
}

