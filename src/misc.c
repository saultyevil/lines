/** ************************************************************************* */
/**
 * @file     misc.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * We may not need the functions which are in here...
 *
 * ************************************************************************** */

#include <stdlib.h>

#include "atomix.h"

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
    cleanup_ncurses_stdscr ();
    printf ("BIG ERROR: Unable to allocate memory for menu screen\n");
    exit (EXIT_FAILURE);
  }

  keypad (*win, TRUE);
}
