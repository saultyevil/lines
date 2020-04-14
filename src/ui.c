/** ************************************************************************* */
/**
 * @file
 * @author   Edward Parkinson
 * @date
 *
 * @brief
 *
 * ************************************************************************** */

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
 * @brief
 *
 * @return
 *
 * @details
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

  if ((*win = newwin (MAX_ROWS, MAX_COLS, winrowstart, wincolstart)) == NULL)
  {
    clean_ncurses_screen ();
    printf ("BIG ERROR: Unable to allocate memory for menu screen\n");
    exit (EXIT_FAILURE);
  }

  keypad (*win, TRUE);
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
query_atomic_data (void)
{
  int err;
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

    if ((err = get_atomic_data (atomic_data_name)))
    {
      // TODO increase verbosity of error messages, i.e. write out actual error
      mvwprintw (win, 4, 0, "!! Invalid atomic data provided, try again.");
      mvwprintw (win, 5, 0, "!! Atomic data error %i\n", err);
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
 * @brief
 *
 * @return
 *
 * @details
 *
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
  int valid = FALSE;
  WINDOW *win;

  echo ();

  create_sub_window (&win);
  bold_message (win, 0, 0, "Please input the wavelength range to query:");

  while (valid != TRUE)
  {
    *wmin = get_wavelength (win, "Minimum wavelength range: ", 2, 0, 26);
    *wmax = get_wavelength (win, "Maximum wavelength range: ", 3, 0, 26);

    // TODO: some scheme for either waiting or pressing a key

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
  }

  delwin (win);
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
display_text_buffer (ScreenBuffer_t *sb, WINDOW *win, int y, int x)
{
  // TODO: Probably not the best thing to do - print error message in future
  if (!sb->buffer)
    return;

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
 * @param[in]     len
 *
 * @return void
 *
 * @details
 *
 * TODO variable argument input and using sprintf would be easier and make nicer code
 *
 * ************************************************************************** */

void
append_to_buffer (ScreenBuffer_t *sb, char *s, size_t len)
{
  char *new;
  new = realloc (sb->buffer, sb->len + len);

  if (!new)
  {
    clean_ncurses_screen ();
    printf ("\nUnable to add additional text to the output buffer :-(\n");
    exit (1);
  }

  memcpy (&new[sb->len], s, len);
  sb->buffer = new;
  sb->len += len;
}

/* ************************************************************************* */
/**
 * @brief   Print a line of dashes to the screen.
 *
 * @param[in,out]  sb   The screen buffer to append to, can be NULL
 *
 * @details
 *
 * The number of dashes is controlled by the constant ndash.
 *
 * ************************************************************************** */

void
append_separator (ScreenBuffer_t *sb)
{
  int i;
  const int ndash = 84;

  for (i = 0; i < ndash; ++i)
  {
    if (sb)
    {
      append_to_buffer (sb, "-", 1);
    }
    else
    {
      Log ("-");
    }
  }

  if (sb)
  {
    append_to_buffer (sb, "\n", 1);
  }
  else
  {
    Log ("\n");
  }
}