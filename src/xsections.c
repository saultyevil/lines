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
query_photoionization_cross_sections (ScreenBuffer_t *sb, double wmin, double wmax)
{
  int i;
  int len;
  int z, istate, phot_info;
  double wavelength;
  double fmin, fmax, fthreshold;
  char element[LINELEN];
  char buffer_line[LINELEN];

  fmax = C / (wmin  * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);

  append_separator (sb);
  len = sprintf (buffer_line, "Photoionization Cross Sections\r\n");
  append_to_buffer (sb, buffer_line, len);
  len = sprintf (buffer_line, " %-12s %-12s %-12s %-12s %-12s\r\n", "Wavelength", "element", "z", "istate",  "PhotInfo");
  append_to_buffer (sb, buffer_line, len);

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
      len = sprintf (buffer_line, " %-12f %-12s %-12i %-12i %-12i\r\n", wavelength, element, z, istate, phot_info);
      append_to_buffer (sb, buffer_line, len);
    }
  }
}