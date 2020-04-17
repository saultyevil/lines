/* ************************************************************************** */
/**
 * @file     bound.c
 * @author   Edward Parkinson
 * @date     September 2019
 *
 * @brief
 *
 * Functions for querying the bound-bound transitions within the atomic data.
 *
 * ************************************************************************** */

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief  The main menu for bound bound transition queries.
 * 
 * @details
 *
 * This function uses limit_lines() in read_atomic_data.c to update the values
 * of nline_min and nline_max. It iterates over the lin_ptr array.
 *
 * limit_lines() is not perfect and usually causes the program to return lines
 * which are just a little out of the provided wavelength range.
 *
 * TODO: add additional options, i.e. lines for a given elements or istate
 * TODO: add a mdenu :^)
 *
 * ************************************************************************** */

void
bound_bound_main_menu (void)
{
  double fmin, fmax;
  double wmin, wmax;
  WINDOW *win = CONTENT_WINDOW.win;

  query_wavelength_range (&wmin, &wmax);
  fmax = C / (wmin  * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);
  limit_lines (fmin, fmax);

  get_bound_bound_lines ();

  display_text_buffer (win, 1, 1);
}

/* ************************************************************************** */
/**
 * @brief Retrieve all the bound bound transitions over a given wavelength
 *        range.
 *
 * @details
 *
 * This function simply loops over the lin_ptr struct between the limits
 * nline_min and nline_max set by the limit_lines() function.
 * 
 * Added to the display buffer for each transition is,
 * 
 *  Wavelength, Element, Z, istat, levu, levl and nion
 * 
 * ************************************************************************** */

void
get_bound_bound_lines (void)
{
  int i;
  const int ndashes = 86;
  int z, istate, nion;
  int levu, levl;
  double wl;
  char element[LINELEN];

  add_to_display_buffer ("Bound-Bound Transitions");
  add_separator_to_buffer (ndashes);
  add_to_display_buffer (" %-12s %-12s %-12s %-12s %-12s %-12s %-12s", "Wavelength", "Element", "Z", "istate", "levu",
                         "levl", "nion");
  add_separator_to_buffer (ndashes);

  for (i = nline_min; i < nline_max; ++i)
  {
    z = lin_ptr[i]->z;
    istate = lin_ptr[i]->istate;
    levu = lin_ptr[i]->levu;
    levl = lin_ptr[i]->levl;
    nion = lin_ptr[i]->nion;
    wl = const_C_SI / lin_ptr[i]->freq / ANGSTROM / 1e-2;
    get_element_name (z, element);
    add_to_display_buffer (" %-12f %-12s %-12i %-12i %-12i %-12i %-12i", wl, element, z, istate, levu, levl, nion);
  }

  add_separator_to_buffer (ndashes);
}
