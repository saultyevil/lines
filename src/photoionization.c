/** ************************************************************************* */
/**
 * @file     photoionization.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for querying the photoionization data.
 *
 * ************************************************************************** */

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief  The main menu for photoionization queries.
 *
 * @details
 *
 * The main menu for photoionization queries is controlled by this function.
 *
 * TODO: add additional options and a menu :^)
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
 * @brief  Retrieve all of the photoionization edges over a given wavelength
 *         range.
 *
 * @param[in]  wmin  The smallest wavelength to consider
 * @param[in]  wmax  The largest wavelength to consider
 *
 * @details
 *
 * This function simply loops over the phot_top pointer (Topbase) and writes
 * to the DISPLAY buffer whenever a threshold frequency for an edge falls
 * within the provided wavelength range.
 *
 * Added to the display buffer for each edge is,
 *
 *  Wavelength, Element, Z, istat and PhotInfo
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
