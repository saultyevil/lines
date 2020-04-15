/** ************************************************************************* */
/**
 * @file
 * @author   Edward Parkinson
 * @date
 *
 * @brief
 *
 * ************************************************************************** */

#include <stdio.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief
 *
 * @return
 *
 * @details
 *
 *
 * ************************************************************************** */

void
photoionization_home_menu (void)
{
  double wmin, wmax;
  WINDOW *win = CONTENT_WINDOW.win;

  query_wavelength_range (&wmin, &wmax);

  get_photoionization_cross_sections (wmin, wmax);

  display_text_buffer (win, 1, 1);
}


/* ************************************************************************** */
/**
 * @brief
 *
 * @param[in] wmin
 * @param[in] wmax
 *
 * @return void
 *
 * @details
 *
 * ************************************************************************** */

void
get_photoionization_cross_sections (double wmin, double wmax)
{
  int i;
  int z, istate, phot_info;
  double wavelength;
  double fmin, fmax, fthreshold;
  char element[LINELEN];
  const int ndashes = 64;

  fmax = C / (wmin * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);

  add_to_display_buffer ("Photoionization Cross Sections\n");
  append_separator_to_buffer (ndashes);
  add_to_display_buffer (" %-12s %-12s %-12s %-12s %-12s\n", "Wavelength", "Element", "Z", "istate", "PhotInfo");
  append_separator_to_buffer (ndashes);

  for (i = 0; i < NLEVELS; ++i)
  {
    fthreshold = phot_top[i].freq[0];
    if (fthreshold > fmin && fthreshold < fmax)
    {
      z = phot_top[i].z;
      istate = phot_top[i].istate;
      phot_info = ion[phot_top[i].nion].phot_info;
      wavelength = const_C_SI / phot_top[i].freq[0] / ANGSTROM / 1e-2;
      get_element_name (z, element);
      add_to_display_buffer (" %-12f %-12s %-12i %-12i %-12i\n", wavelength, element, z, istate, phot_info);
    }
  }

  append_separator_to_buffer (ndashes);
}
