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

#include "atomic.h"
#include "pyAtomic.h"

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
query_line_wavelength_range (ScreenBuffer_t *sb, double wmin, double wmax)
{
  int i;
  int len;
  double fmin, fmax;
  int z, istate, levu, levl, nion;
  double wl;
  char element[LINELEN];
  char buffer_line[LINELEN];

  fmax = C / (wmin  * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);
  limit_lines (fmin, fmax);

  print_separator (sb);
  len = sprintf (buffer_line, "Bound-Bound Lines\r\n");
  append_to_buffer (sb, buffer_line, len);
  len = sprintf (buffer_line, " %-12s %-12s %-12s %-12s %-12s %-12s %-12s\r\n", "Wavelength", "element", "z",
                 "istate", "levu", "levl", "nion");
  append_to_buffer (sb, buffer_line, len);

  for (i = nline_min; i < nline_max; ++i)
  {
    z = lin_ptr[i]->z;
    istate = lin_ptr[i]->istate;
    levu = lin_ptr[i]->levu;
    levl = lin_ptr[i]->levl;
    nion = lin_ptr[i]->nion;
    wl = const_C_SI / lin_ptr[i]->freq / ANGSTROM / 1e-2;
    get_element_name (z, element);

    len = sprintf (buffer_line, " %-12f %-12s %-12i %-12i %-12i %-12i %-12i\r\n", wl, element, z, istate, levu,
                   levl, nion);
    append_to_buffer (sb, buffer_line, len);
  }
}