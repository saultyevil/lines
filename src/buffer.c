/** ************************************************************************* */
/**
 * @file     buffer.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for adding and display various text buffers.
 * 
 * ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "atomix.h"

Display_t DISPLAY = {0, NULL};
Display_t ATOMIC = {0, NULL};

/* ************************************************************************** */
/**
 * @brief  Clean up a text buffer, generally once it has been printed.
 *
 * @details
 * 
 * Free the memory for all of the lines in the DISPLAY buffer, before freeing
 * the memory for DISPLAY.lines and re-initialising DISPLAY.
 *
 * ************************************************************************** */

void
clean_up_display (void)
{
  int i;

  for (i = 0; i < DISPLAY.nlines; ++i)
    free (DISPLAY.lines[i].chars);
  free (DISPLAY.lines);

  DISPLAY.nlines = 0;
  DISPLAY.lines = NULL;
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
clean_up_atomic_summary (void)
{
  int i;

  for (i = 0; i < ATOMIC.nlines; ++i)
    free (ATOMIC.lines[i].chars);
  free (ATOMIC.lines);

  ATOMIC.nlines = 0;
  ATOMIC.lines = NULL;
}

/* ************************************************************************** */
/**
 * @brief  Add a fmt string to the DISPLAY buffer.
 *
 * @param[in]  fmt  The formatted string
 * @param[in]  ...  The arguments for the formatted string
 *
 * @details
 *
 * This function assumes that the string being added is a single, entire, line.
 * A new line character is required at the end, but isn't mandatory. I think
 * this way you can slowly construct a single "display" line over multiple
 * strings stored in DISPLAY ... though, this isn't the intended way to use this
 * function.
 *
 * Uses a bit of a hack, vsnprintf, to figure out the storage requirement for
 * the string. The pointer for the new string is added to the DISPLAY buffer.
 *
 * PLS PLS PLS DO NOT HAVE A \n NEWLINE CHARACTER IN *fmt
 *
 * ************************************************************************** */

void
add_to_display (char *fmt, ...)
{
  int line_index;
  int len;
  va_list va, va_c;

  DISPLAY.nlines++;
  DISPLAY.lines = realloc (DISPLAY.lines, DISPLAY.nlines * sizeof (Line_t));
  line_index = DISPLAY.nlines - 1;

  if (DISPLAY.lines == NULL)
    exit_atomix (EXIT_FAILURE, "Unable to add additional line to the display buffer");

  DISPLAY.lines[line_index].len = 0;
  DISPLAY.lines[line_index].chars = NULL;

  /*
   * May be playing it extra safe here, but make a copy of va as va is not
   * necessarily preserved after vsnprintf...
   */

  va_start (va, fmt);
  va_copy (va_c, va);

  len = vsnprintf (NULL, 0, fmt, va);  // Hack: write 0 to NULL to determine length :-)
  DISPLAY.lines[line_index].chars = malloc (len * sizeof (char) + 1);
  len = vsprintf (DISPLAY.lines[line_index].chars, fmt, va_c);
  DISPLAY.lines[line_index].chars[len] = '\0';  // Redundant, but just in case
  DISPLAY.lines[line_index].len = len;

  logfile ("%s\n", DISPLAY.lines[line_index].chars);

  va_end (va);
  va_end (va_c);
}

/* ************************************************************************* */
/**
 * @brief  Add a line of dashes to the DISPLAY buffer.
 *
 * @param[in]  len  The number of dashes to draw.
 *
 * @details
 *
 * ************************************************************************** */

void
add_separator_to_display (const int len)
{
  int i;
  char tmp[len + 1];

  for (i = 0; i < len; ++i)
    tmp[i] = '-';
  tmp[len] = '\0';

  add_to_display (tmp);
}

/* ************************************************************************** */
/**
 * @brief  Scroll the text buffer up and down.
 *
 * @param[in]  win  The Window_t containing the buffer to be scrolled
 *
 * @details
 *
 *
 * ************************************************************************** */

void
scroll_display (Window_t win)
{
  int i;
  int ch;
  int line_start;
  int current_row;
  WINDOW *the_win = win.win;

  line_start = 0;

  update_status_bar ("Press q of F1 to exit text view or use UP, DOWN, PG UP or PG DN to scroll the text");

  while ((ch = wgetch (the_win)))
  {
    if (ch == 'q' || ch == KEY_F(1))
      break;

    if (DISPLAY.nlines > win.rows - 2)
    {
      wclear (the_win);

      /*
       * Control which line is printed first in the buffer. One can use the
       * UP or DOWN arrow keys as well as PAGE UP and PAGE DOWN
       */

      switch (ch)
      {
        case KEY_UP:
          line_start--;
          break;
        case KEY_DOWN:
          line_start++;
          break;
        case KEY_NPAGE:
          line_start += win.rows - 2;
          break;
        case KEY_PPAGE:
          line_start -= win.rows - 2;
          break;
        case KEY_HOME:
          line_start = 0;
          break;
        case KEY_END:
          line_start = DISPLAY.nlines - win.rows + 2;
          break;
        default:
          break;
      }

      /*
       * These are the boundaries for how big, or small, line_start can be. This
       * should prevent being able to scroll off the buffer and always keeping
       * the screen full.
       */

      if (line_start < 0)
        line_start = 0;
      if (line_start + win.rows - 2 > DISPLAY.nlines)
        line_start = DISPLAY.nlines - win.rows + 2;

      for (i = line_start, current_row = 1; i < DISPLAY.nlines && current_row < win.rows - 1; ++i, ++current_row)
        mvwprintw (the_win, current_row, 1, "%s", DISPLAY.lines[i].chars);
    }

    wrefresh (the_win);
  }
}

/* ************************************************************************** */
/**
 * @brief  
 *
 * @details
 *
 * ************************************************************************** */

void
add_to_atomic_summary (char *fmt, ...)
{
  int line_index;
  int len;
  va_list va, va_c;

  ATOMIC.nlines++;
  ATOMIC.lines = realloc (ATOMIC.lines, ATOMIC.nlines * sizeof (Line_t));
  line_index = ATOMIC.nlines - 1;

  if (ATOMIC.lines == NULL)
    exit_atomix (EXIT_FAILURE, "Unable to add additional line to the atomic summary");

  ATOMIC.lines[line_index].len = 0;
  ATOMIC.lines[line_index].chars = NULL;

  /*
   * May be playing it extra safe here, but make a copy of va as va is not
   * necessarily preserved after vsnprintf...
   */

  va_start (va, fmt);
  va_copy (va_c, va);

  len = vsnprintf (NULL, 0, fmt, va);  // Hack: write 0 to NULL to determine length :-)
  ATOMIC.lines[line_index].chars = malloc (len * sizeof (char) + 1);
  len = vsprintf (ATOMIC.lines[line_index].chars, fmt, va_c);
  ATOMIC.lines[line_index].chars[len] = '\0';  // Redundant, but just in case
  ATOMIC.lines[line_index].len = len;

  logfile ("%s\n", ATOMIC.lines[line_index].chars);

  va_end (va);
  va_end (va_c);
}

/* ************************************************************************** */
/**
 * @brief  
 *
 * @details
 *
 * ************************************************************************** */

void
display_atomic_summary (Window_t win)
{
  int i;
  WINDOW *the_win = win.win;

  wclear (the_win);

  if (ATOMIC.nlines == 0 || ATOMIC.lines == NULL)
  {
    bold_message (the_win, 1, 1, "No text in display buffer to show.");
  }
  else
  {
    for (i = 0; i < ATOMIC.nlines && i < win.rows - 2; ++i)
      mvwprintw (the_win, i + 1, 1, "%s", ATOMIC.lines[i].chars);

    wrefresh (the_win);
  }
}

/* ************************************************************************** */
/**
 * @brief  Print the DISPLAY buffer in the provided window.
 *
 * @param[in]  the_win      The window to print the buffer in
 * @param[in]  start_y  The starting row to print from
 * @param[in]  start_x  The column to print from
 *
 * @details
 *
 * Simply iterates through all of the lines and prints them one by one before
 * refreshing the screen. If DISPLAY.nlines is zero, or if the lines pointer in
 * DISPLAY is NULL, an error message is displayed instead.
 * 
 * TODO: center the output
 *
 * ************************************************************************** */

void
display (Window_t win, int scroll)
{
  int i;
  WINDOW *the_win = win.win;

  wclear (the_win);

  if (DISPLAY.nlines == 0 || DISPLAY.lines == NULL)
  {
    bold_message (the_win, 1, 1, "No text in display buffer to show.");
  }
  else
  {
    for (i = 0; i < DISPLAY.nlines && i < win.rows - 2; ++i)
      mvwprintw (the_win, i + 1, 1, "%s", DISPLAY.lines[i].chars);

    wrefresh (the_win);

    if (scroll == SCROLL_OK)
      scroll_display (win);
  }

  clean_up_display ();
}
