/** ************************************************************************* */
/**
 * @file     bound_bound.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for querying the bound-bound transitions within the atomic data.
 *
 * ************************************************************************** */

#include <stdio.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief  The main menu for bound bound transition queries.
 *
 * TODO add more bound-bound features
 * @details
 *
 * This function uses limit_lines() in read_atomic_data.c to update the values
 * of nline_min and nline_max. It iterates over the lin_ptr array.
 *
 * limit_lines() is not perfect and usually causes the program to return lines
 * which are just a little out of the provided wavelength range.
 *
 * ************************************************************************** */

void
bound_bound_main_menu (void)
{
  double fmin, fmax;
  double wmin, wmax;
  Line_t sb = LINE_INIT;

  WINDOW *win;
  win = CONTENT_WINDOW.win;

  query_wavelength_range (&wmin, &wmax);
  fmax = C / (wmin  * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);
  limit_lines (fmin, fmax);

  get_bound_bound_lines (&sb);
  display_text_buffer (&sb, win, 1, 0);

  wgetch (win);
}

/* ************************************************************************** */
/**
 * @brief          Retrieve the bound bound transitions in a given wavelength
 *                 range.
 *
 * @param[in, out] sb     The screen buffer to write to.
 * @param[in]      wmin   The smallest wavelength transition to find.
 * @param[in]      wmax   The largest wavelength transition to find.
 *
 * @return void
 *
 * @details
 *
 * ************************************************************************** */

void
get_bound_bound_lines (Line_t *sb)
{
  int i;
  const int ndashes = 86;
  int z, istate, nion;
  int levu, levl;
  double wl;
  char element[LINELEN];

  append_to_buffer (sb, "Bound-Bound Lines\n");
  append_separator_to_buffer (sb, ndashes);
  append_to_buffer (sb, " %-12s %-12s %-12s %-12s %-12s %-12s %-12s\n", "Wavelength", "Element", "Z", "istate", "levu",
                    "levl", "nion");
  append_separator_to_buffer (sb, ndashes);

  for (i = nline_min; i < nline_max; ++i)
  {
    z = lin_ptr[i]->z;
    istate = lin_ptr[i]->istate;
    levu = lin_ptr[i]->levu;
    levl = lin_ptr[i]->levl;
    nion = lin_ptr[i]->nion;
    wl = const_C_SI / lin_ptr[i]->freq / ANGSTROM / 1e-2;
    get_element_name (z, element);
    append_to_buffer (sb, " %-12f %-12s %-12i %-12i %-12i %-12i %-12i\n", wl, element, z, istate, levu, levl, nion);
  }

  append_separator_to_buffer (sb, ndashes);
}