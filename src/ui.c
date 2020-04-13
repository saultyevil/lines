/** ************************************************************************* */
/**
 * @file
 * @author   Edward Parkinson
 * @date
 *
 * @brief
 *
 * ************************************************************************** */

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
write_banner (void)
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
