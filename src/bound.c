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
  int query_return;
  double fmin, fmax;
  double wmin, wmax;

  if (nlines == 0)
  {
    error_atomix ("No bound-bound transitions were read in. Unable to query!");
    return;
  }

  query_return = query_wavelength_range (&wmin, &wmax);
  if (query_return == MENU_QUIT)
    return;

  fmax = C / (wmin  * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);
  limit_lines (fmin, fmax);

  get_bound_bound_lines (wmin, wmax);

  display_text_buffer (CONTENT_WINDOW, SCROLL_OK);
}

/* ************************************************************************** */
/**
 * @brief Retrieve all the bound bound transitions over a given wavelength
 *        range.
 *
 * @param[in]  wmin  The smallest wavelength to consider
 * @param[in]  wmax  The largest wavelength to consider
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
get_bound_bound_lines (double wmin, double wmax)
{
  int i;
  const int ndashes = 86;
  double wl;
  char element[LINELEN];

  add_to_display_buffer ("Bound-Bound Transitions: Wavelength range %.2f - %.2f Angstroms", wmin, wmax);
  add_separator_to_buffer (ndashes);
  add_to_display_buffer (" %-12s %-12s %-12s %-12s %-12s %-12s %-12s", "Wavelength", "Element", "Z", "istate", "levu",
                         "levl", "nion");
  add_separator_to_buffer (ndashes);

  for (i = nline_min; i < nline_max - 1; ++i)
  {
    get_element_name (lin_ptr[i]->z, element);
    wl = const_C_SI / lin_ptr[i]->freq / ANGSTROM / 1e-2;
    add_to_display_buffer (" %-12f %-12s %-12i %-12i %-12i %-12i %-12i", wl, element, lin_ptr[i]->z, lin_ptr[i]->istate,
                           lin_ptr[i]->levu, lin_ptr[i]->levl, lin_ptr[i]->nion);
  }

  add_separator_to_buffer (ndashes);
}
