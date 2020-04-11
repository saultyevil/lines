/** ************************************************************************* */
/**
 * @file     main.c
 * @author   Edward Parkinson
 * @date     September 2019
 *
 * @brief    The main functions related to running this program.
 *
 * ************************************************************************** */

#include <string.h>
#include <stdlib.h>

#include "pyAtomic.h"
#include "atomic.h"

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

/* ************************************************************************* */
/**
 * @brief   The main function of the program.
 *
 * @param[in]   argc    The number of arguments provided.
 * @param[in]   argv    The arguments provided.
 *
 * @return      EXIT_SUCCESS
 *
 * @details
 *
 * ************************************************************************** */

int
main (int argc, char *argv[])
{
  double wmin, wmax;
  char atomic_data_name[LINELEN];
  ScreenBuffer_t sb = SB_INIT;

  /*
   * Initialise the output file and read in the atomic data files
   */

  Log_init ("pyAtomic.output.txt");
  parse_input (argc, argv, atomic_data_name, &wmin, &wmax);
  if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
    strcat (atomic_data_name, ".dat");
  print_separator (NULL);
  get_atomic_data (atomic_data_name);

  /*
   * Handle the different queries
   */

  query_line_wavelength_range (&sb, wmin, wmax);
  query_photoionization_cross_sections (&sb, wmin, wmax);

  /*
   * Final output to the screen and close the output file
   */

  display_text_buffer(&sb);
  print_separator (NULL);
  Log_close ();

  return EXIT_SUCCESS;
}
