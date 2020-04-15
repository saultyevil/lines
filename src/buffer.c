/** ************************************************************************* */
/**
 * @file     buffer.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <string.h>


#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
clean_up_display_buffer (void)
{
  int i;
  for (i = 0; i < DISPLAY.nlines; ++i)
    free (DISPLAY.lines[i].chars);
  free (DISPLAY.lines);
  DISPLAY.nlines = 0;
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
 * TODO center the output
 * TODO this should be more generic rather than LINES - 2
 *
 * ************************************************************************** */

void
display_text_buffer (WINDOW *win, int start_y, int start_x)
{
  int i;

  wclear (win);

  if (DISPLAY.nlines == 0)
  {
    bold_message (win, 1, 1, "No text buffer to display!");
    return;
  }

  for (i = 0; i < DISPLAY.nlines && i + start_y < LINES - 2; ++i)
  {
    mvwprintw (win, i + start_y, start_x, "%s", DISPLAY.lines[i].chars);
  }

  wrefresh (win);
  clean_up_display_buffer ();
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
add_to_display_buffer (char *fmt, ...)
{
  int current_line;
  int len;
  va_list va, va_c;

  // Allocate a new line in the Display struct
  current_line = DISPLAY.nlines++;
  DISPLAY.lines = realloc (DISPLAY.lines, DISPLAY.nlines * sizeof (Line_t));

  if (!DISPLAY.lines)
  {
    cleanup_ncurses_stdscr ();
    printf ("BIG ERROR: Unable to add additional line to the display buffer :-(\n");
    exit (1);
  }

  DISPLAY.lines[current_line].len = 0;
  DISPLAY.lines[current_line].chars = NULL;

  // Write the message and arguments to the line
  va_start (va, fmt);
  va_copy (va_c, va);
  len = vsnprintf (NULL, 0, fmt, va);  // Hack to determine length :-)
  DISPLAY.lines[current_line].chars = malloc (len * sizeof (char));
  len = vsprintf (DISPLAY.lines[current_line].chars, fmt, va_c);  // vsprintf NULL terminates the string
  DISPLAY.lines[current_line].len = len;
  va_end (va);
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
append_separator_to_buffer (const int len)
{
  int i;
  char tmp[len + 2];

  for (i = 0; i < len; ++i)
    memcpy (&tmp[i], "-", 1);
  memcpy (&tmp[len], "\n\0", 2);

  add_to_display_buffer (tmp);
}