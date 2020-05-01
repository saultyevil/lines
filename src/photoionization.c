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
bound_free_main_menu (void)
{
  int query_return;
  double wmin, wmax;

  query_return = query_wavelength_range (&wmin, &wmax);
  if (query_return == MENU_QUIT)
    return;

  get_photoionization_cross_sections (wmin, wmax);

  display (CONTENT_WINDOW, SCROLL_OK);
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
 *  TODO: add innershell
 *
 * ************************************************************************** */

void
get_photoionization_cross_sections (double wmin, double wmax)
{ 
  int i;
  int count = 0;
  double wavelength;
  double fmin, fmax, fthreshold;
  char element[LINELEN];
  const int ndash = 88;

  fmax = C / (wmin * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);

  add_to_display ("Photoionization Edges: Wavelength range %.2f - %.2f Angstroms", wmin, wmax);
  add_separator_to_display (ndash);
  add_to_display (" %-12s %-12s %-12s %-12s %-12s %-12s %-12s", "Wavelength", "Element", "Z", "istate", "n", "l",
                  "PhotInfo");
  add_separator_to_display (ndash);

  for (i = 0; i < nphot_total; ++i)
  {
    fthreshold = phot_top[i].freq[0];
    if (fthreshold > fmin && fthreshold < fmax)
    {
      count++;
      get_element_name (phot_top[i].z, element);
      wavelength = const_C_SI / phot_top[i].freq[0] / ANGSTROM / 1e-2;
      add_to_display (" %-12.2f %-12s %-12i %-12i %-12i %-12i %-12i", wavelength, element, phot_top[i].z, phot_top[i].istate,
                      phot_top[i].n, phot_top[i].l, ions[phot_top[i].nion].phot_info);
    }
  }

  add_separator_to_display (ndash);
  add_to_display (" %i entries", count);
  add_separator_to_display (ndash);
}
