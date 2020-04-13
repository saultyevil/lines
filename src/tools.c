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

#include <string.h>
#include <stdlib.h>

#include "atomic.h"
#include "atomix.h"

/* ************************************************************************* */
/**
 * @brief   Parse inputs from the command line.
 *
 * @param[in]        argc   The number of arguments provided.
 * @param[in, out]   argv   The arguments provided.
 * @param[in, out]   fname  The name of the Python data master file.
 * @param[in, out]   wmin   The lower wavelength boundary.
 * @param[in, out]   wmax   The upper wavelength boundary.
 *
 * @return          void
 *
 * @details
 *
 * Variables are returned from this function via pointers. If the incorrect
 * number of arguments are passed, then the program should exit.
 *
 * ### Programming Notes ###
 *
 * Nothing has been done to check the input or order of the arguments, so good
 * luck!
 *
 * ************************************************************************** */

void
parse_input (int argc, char *argv[], char *fname, double *wmin, double *wmax)
{
  char *help = "The purpose of this program is to query the line transitions between two provided wavelength "
               "boundaries.\n\n"
               " Usage:  lines [-h] data wmin wmax\n\n"
               "  [-h]: print this message and exit\n"
               "  data: the path to the Python data master file to use\n"
               "  wmin: the lower wavelength boundary\n"
               "  wmax: the upper wavelength boundary\n";

  double tmp;

  if (argc == 2 && strcmp (argv[1], "-h") == 0)
  {
    Log ("%s", help);
    exit (EXIT_SUCCESS);
  }
  else if (argc != 4)
  {
    Log ("Incorrect parameters provided: expected 3 parameters\n");
    Log ("\n%s", help);
    exit (EXIT_FAILURE);
  }

  strcpy (fname, argv[1]);
  *wmin = atof (argv[2]);  // TODO: one should probably error check this
  *wmax = atof (argv[3]);
}

/* ************************************************************************** */
/**
 * @brief   Get the name of an element given the atomic number
 *
 * @param[in]  z           The atomic number of the element
 * @param[out] element     The name of the element
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

  // Probably not the best to do
  if (element == NULL)
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
display_text_buffer (ScreenBuffer_t *sb)
{
  if (!sb->buffer)
  {
    Log ("Nothing in the screen buffer to print!\n");
    return;
  }

  Log ("%s", sb->buffer);
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
 * ************************************************************************** */

void
append_to_buffer (ScreenBuffer_t *sb, char *s, size_t len)
{
  char *new;
  new = realloc (sb->buffer, sb->len + len);

  if (!new)
  {
    Log ("Unable to add additional text to the output buffer :-(\n");
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
print_separator (ScreenBuffer_t *sb)
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
    append_to_buffer (sb, "\r\n", 2);
  }
  else
  {
    Log ("\n");
  }
}