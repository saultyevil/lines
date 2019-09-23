
/** ************************************************************************* */
/**
 * @file
 * @author   Edward Parkinson
 * @date
 *
 * @brief
 *
 * @details
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
 * @brief
 *
 * @param[in]        argc
 * @param[in, out]   argv
 * @param[in, out]   fname
 * @param[in, out]   wmin
 * @param[in, out]   wmax
 *
 * @return          void
 *
 * @details
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
 * @brief
 *
 *
 * @param[in]   argc
 * @param[in]   argv
 *
 * @return          void
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
  get_atomic_data (fname);

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
