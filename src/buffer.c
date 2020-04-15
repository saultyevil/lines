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
  wclear (win);

  if (!sb->buffer)
  {
    bold_message (win, 1, 1, "No text buffer to display!");
    return;
  }

  // TODO center the message: will require the line thing tho
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
    cleanup_ncurses_stdscr ();
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