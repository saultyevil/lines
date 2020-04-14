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
 * TODO add more bound-bound features
 *
 * ************************************************************************** */

void
bound_bound_home_menu (void)
{
  double wmin, wmax;
  ScreenBuffer_t sb = SB_INIT;

  WINDOW *win;

  query_wavelength_range (&wmin, &wmax);

  create_sub_window (&win);
  get_bound_bound_lines (&sb, wmin, wmax);
  display_text_buffer (&sb, win, 0, 0);

  wgetch (win);

  delwin (win);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @param[in, out] sb
 * @param[in]      wmin
 * @param[in]      wmax
 *
 * @return void
 *
 * @details
 *
 * ************************************************************************** */

void
get_bound_bound_lines (ScreenBuffer_t *sb, double wmin, double wmax)
{
  int i;
  int len;
  int z, istate, levu, levl, nion;
  double wl;
  double fmin, fmax;
  char element[LINELEN];
  char buffer_line[LINELEN];

  fmax = C / (wmin  * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);
  limit_lines (fmin, fmax);

  len = sprintf (buffer_line, "Bound-Bound Lines\n\n");
  append_to_buffer (sb, buffer_line, len);
  append_separator (sb);
  len = sprintf (buffer_line, " %-12s %-12s %-12s %-12s %-12s %-12s %-12s\n", "Wavelength", "element", "z",
                 "istate", "levu", "levl", "nion");
  append_to_buffer (sb, buffer_line, len);
  append_separator (sb);

  for (i = nline_min; i < nline_max; ++i)
  {
    z = lin_ptr[i]->z;
    istate = lin_ptr[i]->istate;
    levu = lin_ptr[i]->levu;
    levl = lin_ptr[i]->levl;
    nion = lin_ptr[i]->nion;
    wl = const_C_SI / lin_ptr[i]->freq / ANGSTROM / 1e-2;
    get_element_name (z, element);

    len = sprintf (buffer_line, " %-12f %-12s %-12i %-12i %-12i %-12i %-12i\n", wl, element, z, istate, levu,
                   levl, nion);
    append_to_buffer (sb, buffer_line, len);
  }

  append_separator (sb);
}