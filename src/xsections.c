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
  Line_t sb = LINE_INIT;

  WINDOW *win;

  query_wavelength_range (&wmin, &wmax);

  create_sub_window (&win);
  get_photoionization_cross_sections (&sb, wmin, wmax);
  display_text_buffer (&sb, win, 0, 0);

  wgetch (win);
  delwin (win);
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
get_photoionization_cross_sections (Line_t *sb, double wmin, double wmax)
{
  int i;
  int z, istate, phot_info;
  double wavelength;
  double fmin, fmax, fthreshold;
  char element[LINELEN];
  const int ndashes = 64;

  fmax = C / (wmin * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);

  append_to_buffer (sb, "Photoionization Cross Sections\n");
  append_separator_to_buffer (sb, ndashes);
  append_to_buffer (sb, " %-12s %-12s %-12s %-12s %-12s\n", "Wavelength", "Element", "Z", "istate", "PhotInfo");
  append_separator_to_buffer (sb, ndashes);

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
      append_to_buffer (sb, " %-12f %-12s %-12i %-12i %-12i\n", wavelength, element, z, istate, phot_info);
    }
  }

  append_separator_to_buffer (sb, ndashes);
}
