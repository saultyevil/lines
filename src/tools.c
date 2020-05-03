/** ************************************************************************* */
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

  // Probably not the best thing to do
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
 * @brief  Check the command line arguments to see if atomic data has been
 *         provided.
 *
 * @return  provided  TRUE if atomic data is provided, otherwise FALSE
 *
 * @details
 *
 * Quick and dirty method to parse the command line arguments. atomix either
 * expects ONE or NO arguments. If one argument is provided, this is assumed
 * to be the file name for the atomic data and it will subsequently be loaded
 * in. If we cannot read the atomic data, then atomix will exit.
 * 
 * This function is called before ncurses is initialised, thus printf should be
 * used instead when expanding it.
 *
 * TODO: better way to parse command line for multiple arguments
 *
 * ************************************************************************** */

int
check_command_line (int argc, char **argv)
{
  int provided = FALSE;
  int atomic_data_error;
  char atomic_data_name[LINELEN];

  char help[] =
    "atomix is a utility program used to inspect the atomic data used in Python.\n"
    "Python is required to be installed correctly for atomix to work.\n"
    "\nTo test atomix, one can load the standard80_test test data.\n\n"
    "Usage:\n"
    "   atomix [-h] [atomic_data]\n\n"
    "   atomic_data  [optional]  the name of the atomic data to explore\n"
    "   h            [optional]  print this help message\n";

  if (argc == 2 && strncmp (argv[1], "-h", 2) == 0)
  {
    printf ("%s", help);
    exit (EXIT_SUCCESS);
  }
  else if (argc == 2)
  {
    strcpy (atomic_data_name, argv[1]);
    if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
      strcat (atomic_data_name, ".dat");

    atomic_data_error = get_atomic_data (atomic_data_name, FALSE);

    if (atomic_data_error)
    {
      log_close ();
      printf ("Fatal error: error when reading atomic data : errno = %i\n", atomic_data_error);
      exit (EXIT_FAILURE);
    }
    provided = TRUE;
  }
  else if (argc > 2)
  {
    printf ("Unknown arguments. Seek help!\n");
    printf ("\n%s", help);
    exit (EXIT_FAILURE);
  }

  return provided;
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
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

  wclear (CONTENT_WINDOW.win);
  bold_message (CONTENT_WINDOW.win, 1, 1, str);
  update_status_bar ("Press q or F1 to continue");
  wrefresh (CONTENT_WINDOW.win);

  while ((ch = wgetch (CONTENT_WINDOW.win)))
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

  log_flush ();
  cleanup_ncurses_stdscr ();

  printf (":-(\n");
  printf ("Fatal Error: ");
  va_start (va, fmt);
  vprintf (fmt, va);
  va_end (va);
  printf ("\nerrno = %i\n", errno);

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
  log_flush ();
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
trim_whitespaces(char *str)
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
