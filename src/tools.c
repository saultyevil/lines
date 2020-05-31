/* ************************************************************************** */
/**
 * @file     tools.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Various utility functions.
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief  Get the name of an element given the atomic number
 *
 * @param[in]   z        The atomic number of the element
 * @param[out]  element  The name of the element
 *
 * @return  void
 *
 * @details
 *
 * This simply loops through the element structure to find the name of the
 * element given the atomic number z.
 *
 * ************************************************************************** */

void
get_element_name (int z, char *element)
{
  int i;

  // Probably not the best
  if (element == NULL)
    return;
  if (ele == NULL)
    return;

  for (i = 0; i < nelements; ++i)
  {
    if (ele[i].z == z)
    {
      strcpy (element, ele[i].name);
      break;
    }
  }
}

/* ************************************************************************** */
/**
 * @brief  Determine if an element is in the atomic data or not.
 *
 * @param[in]  z  The atomic number of the element.
 *
 * @return  Returns the index in the ele array if it exists, otherwise
 *          ELEMENT_NO_FOUND is returned.
 *
 * @details
 *
 * Loops over the ele array trying to match the provided z with the z of all the
 * elements in the atomic data.
 *
 * ************************************************************************** */

int
find_element (int z)
{
  int i;
  int found = FALSE;

  for (i = 0; i < nelements; ++i)
  {
    if (ele[i].z == z)
    {
      found = TRUE;
      break;
    }
  }

  if (!found)
  {
    error_atomix ("Element Z = %i is not in the atomic data", z);
    return ELEMENT_NO_FOUND;
  }

  return i;
}

/* ************************************************************************** */
/**
 * @brief  Print an error message to the content window.
 *
 * @param[in]  fmt  The formatted error string
 * @param[in]  ...  The arguments for the string
 *
 * @details
 *
 * When an error message is printed, atomix will wait for the user to press q
 * or F1 to continue.
 *
 * ************************************************************************** */

void
error_atomix (char *fmt, ...)
{
  int ch;
  int len;
  char *str;
  va_list va, va_c;

  va_start (va, fmt);
  va_copy (va_c, va);

  len = vsnprintf (NULL, 0, fmt, va);
  str = malloc (len * sizeof (char) + 1);
  len = vsprintf (str, fmt, va_c);
  str[len] = '\0';

  va_end (va);
  va_end (va_c);

  wclear (CONTENT_VIEW_WINDOW.window);
  bold_message (CONTENT_VIEW_WINDOW, 1, 1, str);
  update_status_bar ("press q or F1 to continue");
  wrefresh (CONTENT_VIEW_WINDOW.window);

  while ((ch = wgetch (CONTENT_VIEW_WINDOW.window)))
  {
    if (ch == 'q' || ch == KEY_F(1))
      break;
  }

  free (str);
}

/* ************************************************************************** */
/**
 * @brief  Exit atomix when something has gone wrong.
 *
 * @param[in]  errno  An error number
 * @param[in]  fmt    A formatted string describing the error
 * @param[in]  ...    The arguments for the formatted string
 *
 * @details
 *
 * Cleans up atomix by flushing the log and cleaning the ncurses screen. The
 * error message is then printed to the screen before atomix exits fully. Note
 * that ncurses is cleaned up first, as this function prints to stdout.
 *
 * ************************************************************************** */

void
exit_atomix (int errno, char *fmt, ...)
{
  va_list va;

  logfile_flush();
  cleanup_ncurses_stdscr ();

  printf ("Fatal Error: errno = %i: ", errno);
  va_start (va, fmt);
  vprintf (fmt, va);
  va_end (va);
  printf ("\n");

  exit (errno);
}

/* ************************************************************************** */
/**
 * @brief  Used toe exit atomix from the main menu.
 *
 * @details
 *
 * Flushes the log and then exit's the program. The screen isn't cleaned up as
 * we are not printing to stdout so atexit() will clean up the screen for us.
 *
 * ************************************************************************** */

void
menu_exit_atomix (void)
{
  logfile_flush();
  exit (EXIT_SUCCESS);
}

/* ************************************************************************** */
/**
 * @brief  Trim the whitespace from the end of a string.
 *
 * @param[in]  str  The string to trim
 *
 * @return     str  The trimmed string
 *
 * ************************************************************************** */

char *
trim_whitespaces (char *str)
{
  char *end;

  while (isspace (*str))
    str++;

  if (*str == 0)
    return str;

  end = str + strlen(str) - 1;

  while (end > str && isspace (*end))
    end--;

  *(end + 1) = '\0';

  return str;
}

/* ************************************************************************** */
/**
 * @brief  Print a number with a separator up and below.
 *
 * @param[in]  ndash  The number of dashes for the separator
 * @param[in]  count  The number of items
 *
 * @details
 *
 * ************************************************************************** */

void
count (int ndash, int count)
{
  add_sep_display (ndash);
  display_add (" %i entries", count);
  add_sep_display (ndash);
}

/* ************************************************************************** */
/**
 * @brief  Create a formatted string.
 *
 * @param[out]  str  The created string
 * @param[in]   fmt  The formatted messaged
 * @param[in]   ...  The formatted message arguments
 *
 * @return    The length of the string written is returned
 *
 * @details
 *
 * Uses a clever "hack" to write 0 characters to a NULL pointer to determine
 * the length of the formatted string so the correct number of bytes can be
 * allocated for the string.
 *
 * The final char of the string is set to \0, because I'm not always sure/trust
 * that vsprintf null terminates...
 *
 * TODO: check if I'm being over cautious with manually adding \0
 *
 * ************************************************************************** */

int
create_string (char *str, char *fmt, ...)
{
  int len;
  va_list va, va_c;

  va_start (va, fmt);
  va_copy (va_c, va);

  len = vsnprintf (NULL, 0, fmt, va);
  str = malloc (len * sizeof (char) + 1);
  len = vsprintf (str, fmt, va_c);
  str[len] = '\0';

  va_end (va);
  va_end (va_c);

  return len;
}
