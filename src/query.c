/** ************************************************************************* */
/**
 * @file     query.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for querying information from the user.
 *
 * TODO switch to using the forms library
 *
 * ************************************************************************** */

#include <form.h>
#include <string.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief     Query the user for the atomic data name.
 *
 * @details
 *
 * Will keep asking the user for an atomic data name until the data can be read
 * in without error, or until the user quits the program.
 *
 * ************************************************************************** */

int
query_atomic_data (void)
{
  int atomic_data_error;
  int valid = FALSE;
  char atomic_data_name[LINELEN];
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

    atomic_data_error = get_atomic_data (atomic_data_name);

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
 * ************************************************************************** */

void
query_wavelength_range (double *wmin, double *wmax)
{
  int valid = FALSE;
  WINDOW *win = CONTENT_WINDOW.win;

  wclear (win);
  echo ();

  bold_message (win, 1, 2, "Please input the wavelength range to query:");

  while (valid != TRUE)
  {

    *wmin = get_wavelength (win, "Minimum wavelength range: > ", 3, 2, 28);
    *wmax = get_wavelength (win, "Maximum wavelength range: > ", 5, 2, 28);

    if (*wmax > *wmin)
    {
      valid = TRUE;
    }
    else
    {
      bold_message (win, 7, 2, "Incorrect input. Try again...");
      wrefresh (win);
      wmove (win, 3, 0);
      wclrtoeol (win);
      wmove (win, 5, 0);
      wclrtoeol (win);
    }
  }

  noecho ();
}
