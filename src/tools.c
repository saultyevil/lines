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
 * ************************************************************************** */

int
check_command_line (int argc, char **argv)
{
  int provided = FALSE;
  int atomic_data_error;
  char atomic_data_name[LINELEN];

  char help[] =
    "atomix is a utility program used to inspect the atomic data used in Python.\n"
    "Python is required to be installed correctly for atomix to work.\n\n"
    "Usage:\n"
    "   atomix [-h] [atomic_data]\n\n"
    "   atomic_data [optional]  the name of the atomic data to explore\n"
    "   h           [optional]  print this help message\n";


  if ((argc == 2 || argc == 3) && !strcmp (argv[1], "-h"))
  {
    printf ("%s", help);
    exit (EXIT_SUCCESS);
  }
  else if (argc == 2)
  {
    strcpy (atomic_data_name, argv[1]);
    if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
      strcat (atomic_data_name, ".dat");

    atomic_data_error = get_atomic_data (atomic_data_name);

    if (atomic_data_error)
    {
      // TODO more verbose error reporting - generic function required
      printf ("Error: Invalid atomic data provided, try again.\n");
      printf ("Error: Atomic data error %i\n", atomic_data_error);
      exit (EXIT_FAILURE);
    }
    else
    {
      provided = TRUE;
    }
  }
  else if (argc > 2)
  {
    printf ("Unknown arguments.\n");
    printf ("\n%s", help);
    exit (EXIT_FAILURE);
  }

  return provided;
}
