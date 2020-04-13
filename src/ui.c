/** ************************************************************************* */
/**
 * @file
 * @author   Edward Parkinson
 * @date
 *
 * @brief
 *
 * ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

#include "ui.h"
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
 * @brief
 *
 * @return
 *
 * @details
 *
 *
 * ************************************************************************** */

void
create_sub_window (WINDOW **win)
{
  const int winrowstart = 2;
  const int wincolstart = 2;

  write_banner_stdscr ();

  if ((*win = newwin (MAX_ROWS, MAX_COLS, winrowstart, wincolstart)) == NULL)
  {
    clean_ncurses_screen ();
    printf ("BIG ERROR: Unable to allocate memory for menu screen\n");
    exit (EXIT_FAILURE);
  }

  keypad (*win, TRUE);
//  scrollok (*win, TRUE);
  wrefresh (*win);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @return
 *
 * @details
 *
 *
 * ************************************************************************** */

void
query_wavelength_range (double *wmin, double *wmax)
{
  int valid;
  WINDOW *win;

  create_sub_window (&win);
  wattron (win, A_BOLD);
  mvwprintw (win, 0, 0, "Please input the wavelength range:");
  wattroff (win, A_BOLD);

  valid = FALSE;
  while (valid != TRUE)
  {
    /*
     * Get minimum wavelength range
     */

    mvwprintw (win, 2, 0, "Minimum wavelength range: ");
    wrefresh (win);
    wscanw (win, "%lf", wmin);
    mvwprintw (win, 2, 26, "%5.1f Angstroms", *wmin);

    /*
     * Get maximum wavelength range
     */

    mvwprintw (win, 3, 0, "Maximum wavelength range: ");
    wrefresh (win);
    wscanw (win, "%lf", wmax);
    mvwprintw (win, 3, 26, "%5.1f Angstroms", *wmax);
    wrefresh (win);

    /*
     * TODO: wait or press key to continue
     */

    wattron (win, A_BOLD);

    if (*wmax > *wmin)
    {
      valid = TRUE;
      mvwprintw (win, 5, 0, "Wavelength input accepted.");
      wrefresh (win);
      sleep (2);
    }
    else
    {
      mvwprintw (win, 5, 0, "Invalid input, the minimum wavelength should be smaller than the maximum. Try again!");
      wrefresh (win);
      sleep (2);
    }
    wattroff (win, A_BOLD);
  }

  delwin (win);
}
