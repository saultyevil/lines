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
clean_up_display_buffer (void)
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
display_text_buffer (Window_t win, int start_y, int start_x)
{
  int i;
  WINDOW *the_win = win.win;

  wclear (the_win);

  if (DISPLAY.nlines == 0 || DISPLAY.lines == NULL)
  {
    bold_message (the_win, 1, 1,
                  "No text buffer to display! Did someone forget to load atomic data, or program something important?");
  }
  else
  {
    int ch;
    int line_start = 0;

    for (i = line_start; i < DISPLAY.nlines && i < win.rows - 2; ++i)
      mvwprintw (the_win, i + 1, 1, "%s", DISPLAY.lines[i].chars);
    update_status_bar("Press q to exit text scrolling or UP and DOWN to scroll the text");
    wrefresh (the_win);

    while ((ch = wgetch (the_win)) != 'q')
    {
      if (DISPLAY.nlines > win.rows - 2)
      {
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
          default:
            break;
        }

        if (line_start < 0)
          line_start = 0;
        if (line_start > DISPLAY.nlines - 1)
          line_start = DISPLAY.nlines - 1;

        wclear (the_win);
        int the_row = 1;
        for (i = line_start; i < DISPLAY.nlines; ++i)
        {
          if (the_row > win.rows - 2)
            break;
          mvwprintw (the_win, the_row, 1, "%s", DISPLAY.lines[i].chars);
          the_row++;
        }

        wrefresh (the_win);
      }
    }
  }

  wrefresh (the_win);
  clean_up_display_buffer ();
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
add_to_display_buffer (char *fmt, ...)
{
  int line_index;
  int len;
  va_list va, va_c;

  DISPLAY.nlines++;
  DISPLAY.lines = realloc (DISPLAY.lines, DISPLAY.nlines * sizeof (Line_t));
  line_index = DISPLAY.nlines - 1;

  if (DISPLAY.lines == NULL)
    error_exit_atomix (EXIT_FAILURE, "Unable to add additional line to the display buffer");

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
  len = vsprintf (DISPLAY.lines[line_index].chars, fmt, va_c);  // vsprintf NULL terminates the string
  DISPLAY.lines[line_index].chars[len] = '\0';  // okay, so I don't trust vsprintf...
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
add_separator_to_buffer (const int len)
{
  int i;
  char tmp[len + 1];

  for (i = 0; i < len; ++i)
    tmp[i] = '-';
  tmp[len] = '\0';

  add_to_display_buffer (tmp);
}
