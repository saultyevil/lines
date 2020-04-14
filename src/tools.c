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

#include "atomix.h"

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

  // Probably not the best thing to do
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
 * @return
 *
 * @details
 *
 * ************************************************************************** */

int
check_command_line (int argc, char **argv)
{
  int atomic_data_error;
  int provided = FALSE;
  char atomic_data_name[LINELEN];
  char help[] =
    "atomix is a utility program used to inspect the atomic data used in Python.\n\n"
    "Usage:\n"
    "   atomix [-h] [atomic_data]\n\n"
    "   atomic_data [optional]  the name of the atomic data to explore\n"
    "   h           [optional]  print this help message\n";

  if (argc == 2 && !strncmp (argv[1], "-h", 2))
  {
    printf ("%s", help);
    exit (0);
  }
  else if (argc == 2)
  {
    strcpy (atomic_data_name, argv[1]);
    if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
    {
      strcat (atomic_data_name, ".dat");
    }
    atomic_data_error = get_atomic_data (atomic_data_name);
    if (atomic_data_error)
    {
      // TODO more verbose error reporting - generic function required
      printf ("!! Invalid atomic data provided, try again.\n");
      printf ( "!! Atomic data error %i\n", atomic_data_error);
      exit (1);
    }
    else
    {
      provided = TRUE;
    }
  }
  else if (argc > 2)
  {
    printf ("Invalid number of arguments passed.\n");
    printf ("\n%s", help);
    exit (1);
  }

  return provided;
}
