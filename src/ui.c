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
#include <curses.h>

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
init_ncurses_screen (void)
{
  initscr ();
  clear ();
  noecho ();
  cbreak ();
  keypad (stdscr, TRUE);
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
clean_ncurses_screen (void)
{
  Log_close ();
  endwin ();
}

/* ************************************************************************** */
/**
 *  @brief     Write top and bottom banner to stdscr.
 *
 *  @details
 *
 *  This write a top and bottom banner to stdscr which will act as the base
 *  canvas for the UI. In the top bar is the name of the program as well as the
 *  current version number.
 *
 * ************************************************************************** */

void
write_banner_stdscr (void)
{
  int tlen, icol;
  char top_msg[LINELEN];

  tlen = sprintf (top_msg, "atomix : version %s", VERSION);

  attron (A_REVERSE);

  /*
   * Write the top banner and its message
   */

  for (icol = 0; icol < COLS; icol++)
    mvprintw (0, icol, " ");
  mvprintw (0, COLS / 2 - tlen / 2 + 1, "%s", top_msg);

  /*
   * Write the bottom banner and its message
   */

  for (icol = 0; icol < COLS; icol++)
    mvprintw (LINES - 1, icol, " ");

  attroff (A_REVERSE);
  refresh ();
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
 * @brief         Create a sub-window placed ontop of the main stdscr.
 *
 * @param[in,out] win   A pointer to the new window.
 *
 * @details
 *
 * Generic functions which creates a sub-window of a generic size. This is a
 * window which has a surrounding border.
 *
 * ************************************************************************** */

void
create_sub_window (WINDOW **win)
{
  const int winrowstart = 2;
  const int wincolstart = 2;

  if ((*win = newwin (MAX_ROWS_SUB_WIN, MAX_COLS_SUB_WIN, winrowstart, wincolstart)) == NULL)
  {
    clean_ncurses_screen ();
    printf ("BIG ERROR: Unable to allocate memory for menu screen\n");
    exit (EXIT_FAILURE);
  }

  keypad (*win, TRUE);
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

void
query_atomic_data (void)
{
  int atomic_data_error;
  int valid = FALSE;
  char atomic_data_name[LINELEN];

  WINDOW *win;

  echo ();

  while (valid != TRUE)
  {
    create_sub_window (&win);
    bold_message (win, 0, 0, "Please input the atomic data master file name:");
    wmove (win, 2, 0);
    wrefresh (win);

    wscanw (win, "%s", atomic_data_name);
    if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
      strcat (atomic_data_name, ".dat");

    atomic_data_error = get_atomic_data (atomic_data_name);

    if (atomic_data_error)
    {
      // TODO increase verbosity of error messages, i.e. write out actual error
      mvwprintw (win, 4, 0, "!! Invalid atomic data provided, try again.");
      mvwprintw (win, 5, 0, "!! Atomic data error %i\n", atomic_data_error);
      sleep (2);
    }
    else
    {
      // TODO write out atomic data diagnostic output to screen
      valid = TRUE;
    }

    wrefresh (win);
    delwin (win);
  }

  noecho ();
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
  mvwprintw (win, y, len, "%5.1f Angstroms", wl);
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
  WINDOW *win;

  echo ();

  while (valid != TRUE)
  {
    create_sub_window (&win);
    bold_message (win, 0, 0, "Please input the wavelength range to query:");

    *wmin = get_wavelength (win, "Minimum wavelength range: ", 2, 0, 26);
    *wmax = get_wavelength (win, "Maximum wavelength range: ", 3, 0, 26);

    // TODO: some scheme for either waiting or pressing a key

    if (*wmax > *wmin)
    {
      valid = TRUE;
      mvwprintw (win, 5, 0, "Wavelength input accepted.");
      wrefresh (win);
    }
    else
    {
      mvwprintw (win, 5, 0, "Invalid input, the minimum wavelength should be smaller than the maximum. Try again!");
      wrefresh (win);
    }

    sleep (2);
    delwin (win);
  }

  noecho ();
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @param[in] sb
 *
 * @return void
 *
 * @details
 *
 * ************************************************************************** */

void
display_text_buffer (Line_t *sb, WINDOW *win, int y, int x)
{
  // TODO: Probably not the best thing to do - print error message in future
  if (!sb->buffer)
    return;

  // TODO center the message
  mvwprintw (win, y, x, "%s", sb->buffer);
  wrefresh (win);
  free (sb->buffer);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @param[in,out] sb
 * @param[in]     s
 * @param[in]     ...
 *
 * @return void
 *
 * @details
 *
 * ************************************************************************** */

void
append_to_buffer (Line_t *sb, char *s, ...)
{
  int len;
  char *new;
  char msg[LINELEN];
  va_list args;

  va_start (args, s);
  len = vsprintf (msg, s, args);
  new = realloc (sb->buffer, sb->len + len);
  va_end (args);

  if (!new)
  {
    clean_ncurses_screen ();
    printf ("BIG ERROR: Unable to add additional text to the output buffer :-(\n");
    exit (1);
  }

  memcpy (&new[sb->len], msg, len);
  sb->buffer = new;
  sb->len += len;
}

/* ************************************************************************* */
/**
 * @brief          Add a line of dashes to the screen buffer.
 *
 * @param[in,out]  sb   The screen buffer to append to, can be NULL
 * @param[in]      len  The number of dashes to draw.
 *
 * @details
 *
 * ************************************************************************** */

void
append_separator_to_buffer (Line_t *sb, const int len)
{
  int i;
  char tmp[len + 1];

  for (i = 0; i < len; ++i)
    memcpy (&tmp[i], "-", 1);
  memcpy (&tmp[len], "\n", 2);

  append_to_buffer (sb, tmp, len + 1);
}