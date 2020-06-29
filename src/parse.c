/* ************************************************************************** */
/**
 * @file     parse.c
 * @author   Edward Parkinson
 * @date     May 2020
 *
 * @brief
 *
 * Functions concerned with parsing the command line.
 *
 * ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "atomix.h"

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
check_command_line(int argc, char **argv)
{
  int provided = false;
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

  if (argc == 2 && strncmp(argv[1], "-h", 2) == 0)
  {
    printf("%s", help);
    exit(EXIT_SUCCESS);
  }
  else if (argc == 2)
  {
    strcpy(atomic_data_name, argv[1]);
    if (strcmp(&atomic_data_name[strlen(atomic_data_name) - 4], ".dat") != 0)
      strcat(atomic_data_name, ".dat");

    atomic_data_error = get_atomic_data(atomic_data_name, false);

    if (atomic_data_error)
    {
      logfile_close();
      printf("Fatal error: error when reading atomic data : errno = %i\n", atomic_data_error);
      exit(EXIT_FAILURE);
    }

    provided = true;
    strcpy(AtomixConfiguration.atomic_data, atomic_data_name);
  }
  else if (argc > 2)
  {
    printf("Unknown arguments. Seek help!\n");
    printf("\n%s", help);
    exit(EXIT_FAILURE);
  }

  return provided;
}
