
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
#include "atomic.h"
#include "templates.h"

#define LINELEN 256
#define C_SI 299792458

/* ************************************************************************* */
/**
 * @brief   Print a line of dashes to the screen.
 *
 * @details
 *
 * The number of dashes is controlled by the constant ndash.
 *
 * ************************************************************************** */

void
print_separator (void)
{
  int i;
  const int ndash = 80;
  for (i = 0; i < ndash - 1; ++i)
    Log ("-");
  Log ("-\n");
}

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
  *wmin = atof (argv[2]);
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
  int i, z, istate;
  double wmin, wmax;
  double fmin, fmax;
  char fname[LINELEN];

  Log_init ("lines.out");
  parse_input (argc, argv, fname, &wmin, &wmax);

  print_separator ();
  get_atomic_data (fname);
  print_separator ();

  fmax = C / (wmin  * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);
  limit_lines (fmin, fmax);

  for (i = nline_min; i < nline_max; ++i)
  {
    z = lin_ptr[i]->z;
    istate = lin_ptr[i]->istate;
    double freq = lin_ptr[i]->freq;
    double wl = C_SI / freq;
    Log ("z %i istate %i freq %e wl %f\n", z, istate, freq, wl / ANGSTROM / 1e-2);
  }

  Log_close ();

  return EXIT_SUCCESS;
}
