/* ************************************************************************** */
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
#include <signal.h>
#include <stdbool.h>

#include "atomix.h"

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
clean_up_display(Display_t *buffer)
{
  int i;

  for(i = 0; i < buffer->nlines; ++i)
    free(buffer->lines[i].chars);
  free(buffer->lines);

  buffer->nlines = buffer->maxlen = 0;
  buffer->lines = NULL;
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
 * Do not provide a new line character at the end, it probably will break
 * something.
 *
 * Uses a bit of a hack of vsnprintf to figure out the storage requirement for
 * the string. The pointer for the new string is added to the DISPLAY buffer.
 *
 * TODO check for \n or \r\n etc.
 *
 * ************************************************************************** */

void
add_display(Display_t *buffer, char *fmt, ...)
{
  int len;
  int line_index;
  va_list va, va_c;

  buffer->nlines++;
  buffer->lines = realloc(buffer->lines, buffer->nlines * sizeof(Line_t));
  line_index = buffer->nlines - 1;

  if(buffer->lines == NULL)
    exit_atomix(EXIT_FAILURE, "Unable to add additional line to the display buffer");

  buffer->lines[line_index].len = 0;
  buffer->lines[line_index].chars = NULL;

  /*
   * May be playing it extra safe here, but make a copy of va as va is not
   * necessarily preserved after vsnprintf (apparently)
   */

  va_start(va, fmt);
  va_copy(va_c, va);

  len = vsnprintf(NULL, 0, fmt, va);  // Hack: write 0 to NULL to determine length :-)
  buffer->lines[line_index].chars = malloc(len * sizeof(char) + 1);
  len = vsprintf(buffer->lines[line_index].chars, fmt, va_c);
  buffer->lines[line_index].chars[len] = '\0';  // I believe this is redundant, but just in case

  /*
   * Remove whitespace to stop being able to overscroll the buffer
   */

  trim_whitespaces(buffer->lines[line_index].chars);
  buffer->lines[line_index].len = strlen(buffer->lines[line_index].chars);;

  if(buffer->maxlen < buffer->lines[line_index].len)
    buffer->maxlen = len + 1;

  va_end(va);
  va_end(va_c);
  logfile("%s\n", buffer->lines[line_index].chars);
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
add_sep_display(const int len)
{
  int i;
  char line[len + 1];

  for(i = 0; i < len; ++i)
    line[i] = '-';
  line[len] = '\0';
  display_add(line);
}

/* ************************************************************************** */
/**
 * @brief  Add a message to display the current line of the buffer being viewed.
 *
 * @param[in]  win           The Window_t containing the buffer being displayed
 * @param[in]  current_line  The current line of the buffer at the top of the
 *                           window
 * @param[in]  total_lines   The total number of lines in the buffer
 *
 * @details
 *
 * When the maximum window scroll has been reached, END will be displayed
 * instead of the line number.
 *
 * TODO variable END line
 *
 * ************************************************************************** */

void
update_current_line_progress(Window_t win, int current_line, int total_lines)
{
  int len;
  char line_message[LINELEN];

  if(current_line + win.nrows - 2 > total_lines)
  {
    len = sprintf(line_message, "| END  / %-4d |", total_lines);
  }
  else
  {
    len = sprintf(line_message, "| %-4d / %-4d |", current_line, total_lines);
  }

  mvwprintw(win.window, win.nrows - 1, win.ncols - len - 2, line_message);

}

/* ************************************************************************** */
/**
 * @brief  Scroll the text buffer up and down.
 *
 * @param[in]  buffer             The buffer to scroll through
 * @param[in]  win                The Window_t which the buffer is being
 *                                displayed to
 * @param[in]  persistent_header  If true, a header will be stuck at the top
 *                                of win
 * @param[in]  header_row         The number of rows in the buffer to be
 *                                persistent
 *
 * @details
 *
 * The main purpose of this function is to allow the text buffer to be scrolled
 * both horizontally and vertically. The screen is only updated if a key has
 * been pressed which will scroll to avoid screen flicker.
 *
 * The persistent header functionality of this function allows the first
 * header_row lines in the buffer to become frozen at the top of the window. The
 * purpose of this it to act as a persistent header to make scrolling less
 * confusing when has scrolled very far and no longer has the original header
 * for reference.
 *
 * ************************************************************************** */

void
scroll_display(Display_t *buffer, Window_t win, bool persistent_header, int header_rows)
{
  int i, j;
  int ch;
  int row_origin, col_origin;
  int current_line, current_col;
  int srow, scol;
  bool screen_position_moved;
  WINDOW *window = win.window;

  current_line = current_col = 0;
  row_origin = col_origin = 1;
  screen_position_moved = false;

  /*
   * The buffer and row origin have to be incremented otherwise the persistent
   * header will be shown twice or will be overwritten by some other text in
   * the buffer.
   * If there is no header, then for safety we set the number of rows to 0.
   */

  if(persistent_header)
  {
    current_line = header_rows;
    row_origin += header_rows;
  }
  else
  {
    header_rows = 0;
  }

  update_status_bar("press q or F1 to exit text view or use the ARROW KEYS to navigate");

  while((ch = wgetch(window)))
  {
    if(ch == 'q' || ch == KEY_F(1))
      break;

    /*
     * Only allow key control when the buffer is large enough to scroll off
     * the screen
     */

    if(buffer->nlines > win.nrows - 2)
    {
      screen_position_moved = true;
      switch (ch)
      {
        case KEY_RESIZE:
          redraw_screen(SIGWINCH);
          break;
        case KEY_UP:
          current_line--;
          break;
        case KEY_DOWN:
          current_line++;
          break;
        case KEY_RIGHT:
          current_col++;
          break;
        case KEY_LEFT:
          current_col--;
          break;
        case KEY_NPAGE:
          current_line += win.nrows - 2;
          break;
        case KEY_PPAGE:
          current_line -= win.nrows - 2;
          break;
        case KEY_HOME:
          current_line = header_rows;
          break;
        case KEY_END:
          current_line = header_rows + buffer->nlines - (win.nrows - 2);
          break;
        default:
          screen_position_moved = false;
          break;
      }

      /*
       * Only update the screen if the view has been moved, this is to avoid
       * problems if wgetch has nodelay moved enabled
       */

      if(screen_position_moved)
      {
        wclear(window);

        if(current_line < header_rows)
          current_line = header_rows;
        if(current_line + (win.nrows - row_origin - 2) > buffer->nlines - 1)
          current_line = header_rows + buffer->nlines - (win.nrows - 2);

        if(current_col < 0)
          current_col = 0;
        if(current_col + (win.ncols - col_origin - 2) > buffer->maxlen - 1)
          current_col = AtomixConfiguration.current_col;

        AtomixConfiguration.current_line = current_line;
        AtomixConfiguration.current_col = current_col;

        /*
         * Write the header to screen, char by car
         */

        if(persistent_header)
        {
          for(i = 0, srow = 1; i < header_rows && srow < win.nrows - 1; ++i, ++srow)
          {
            for(j = current_col, scol = col_origin; j < buffer->lines[i].len && scol < win.ncols - 1; ++j, ++scol)
            {
              mvwprintw(window, srow, scol, "%c", buffer->lines[i].chars[j]);
            }
          }
        }

        /*
         * Write the buffer to screen, taking into account any header, char
         * by char
         */

        for(i = current_line, srow = row_origin; i < buffer->nlines && srow < win.nrows - 1; ++i, ++srow)
        {
          for(j = current_col, scol = col_origin; j < buffer->lines[i].len && scol < win.ncols - 1; ++j, ++scol)
          {
            mvwprintw(window, srow, scol, "%c", buffer->lines[i].chars[j]);
          }
        }

        update_current_line_progress(win, current_line + 1 - header_rows, buffer->nlines - header_rows);
        wrefresh(window);
      }

      screen_position_moved = false;
    }
  }
}

/* ************************************************************************** */
/**
 * @brief  Print a buffer into CONTENT_WINDOW window.
 *
 * @param[in]  *buffer            The buffer to be displayed to screen.
 * @param[in]  scroll             Allows the buffer to be scrolled, allowed
 *                                values are SCROLL_ENABLE or SCROLL_DISABLE
 * @param[in]  persistent_header  If true, a header will be stuck at the top
 *                                of win
 * @param[in]  header_row         The number of rows in the buffer to be
 *                                persistent
 *
 * @details
 *
 * Simply iterates through all of the lines and prints them one by one before
 * refreshing the screen. If DISPLAY.nlines is zero, or if the lines pointer in
 * DISPLAY is NULL, an error message is displayed instead.
 *
 * The persistent header functionality of this function allows the first
 * header_row lines in the buffer to become frozen at the top of the window. The
 * purpose of this it to act as a persistent header to make scrolling less
 * confusing when has scrolled very far and no longer has the original header
 * for reference.
 *
 * ************************************************************************** */

void
display_buffer(Display_t *buffer, int scroll, bool persisent_header, int header_rows)
{
  int i, j;
  int line_index;
  WINDOW *window = CONTENT_VIEW_WINDOW.window;

  wclear(window);

  if(buffer->nlines == 0 || buffer->lines == NULL)
  {
    bold_message(CONTENT_VIEW_WINDOW, 1, 1, "No text in %s buffer to show", buffer->name);
    wrefresh(window);
  }
  else
  {
    for(i = 0; i < buffer->nlines && i < CONTENT_VIEW_WINDOW.nrows - 2; ++i)
    {
      line_index = i;
      for(j = 0; j < buffer->lines[line_index].len && j < CONTENT_VIEW_WINDOW.ncols - 2; ++j)
      {
        mvwprintw(window, i + 1, j + 1, "%c", buffer->lines[line_index].chars[j]);
      }
    }

    update_current_line_progress(CONTENT_VIEW_WINDOW, 1, buffer->nlines - header_rows);
    wrefresh(window);

    if(scroll == SCROLL_ENABLE)
      scroll_display(buffer, CONTENT_VIEW_WINDOW, persisent_header, header_rows);
  }
}
