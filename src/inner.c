/* ************************************************************************** */
/**
 * @file     inner.c
 * @author   Edward Parkinson
 * @date     May 2020
 *
 * @brief
 *
 * Functions for querying the inner shell ionization data.
 *
 * ************************************************************************** */

#include <stdbool.h>

#include "atomix.h"

static const int ndash = 88;

/* ************************************************************************** */
/**
 * @brief  Add a header for the bound free table.
 *
 * @details
 *
 * ************************************************************************** */

void
inner_shell_header(void)
{
  display_add(" %-12s %-12s %-12s %-12s %-12s %-12s %-12s", "Wavelength", "Element", "Z", "istate", "n", "l",
              "PhotInfo");
  add_sep_display(ndash);
}

/* ************************************************************************** */
/**
 * @brief  Standard line for a bound free line.
 *
 * @details
 *
 * The function inner_shell_header will create an appropriate header for these
 * lines.
 *
 * ************************************************************************** */

void
inner_shell_line(int nphot)
{
  double wavelength;
  char element[LINELEN];

  get_element_name(inner_cross_ptr[nphot]->z, element);
  wavelength = C_SI / inner_cross_ptr[nphot]->freq[0] / ANGSTROM / 1e-2;
  display_add(" %-12.2f %-12s %-12i %-12i %-12i %-12i %-12i", wavelength, element, inner_cross_ptr[nphot]->z,
              inner_cross_ptr[nphot]->istate, inner_cross_ptr[nphot]->n, inner_cross_ptr[nphot]->l,
              ions[inner_cross_ptr[nphot]->nion].phot_info);
}

/* ************************************************************************** */
/**
 * @brief  Print all the bound free edges in the atomic data.
 *
 * @details
 *
 * ************************************************************************** */

void
all_inner_shell(void)
{
  int n, nphot;
  double wmin, wmax;

  wmin = C_SI / inner_cross_ptr[0]->freq[0] / ANGSTROM / 1e-2;
  wmax = C_SI / inner_cross_ptr[n_inner_tot - 1]->freq[0] / ANGSTROM / 1e-2;

  display_add(" Wavelength range: %.2f - %.2f Angstroms", wmin, wmax);
  add_sep_display(ndash);

  inner_shell_header();

  n = 0;

  for(nphot = 0; nphot < n_inner_tot; ++nphot)
  {
    n++;
    inner_shell_line(nphot);
  }

  count(ndash, n);

  display_show(SCROLL_ENABLE, true, 4);
}

/* ************************************************************************** */
/**
 * @brief  Retrieve all of the photoionization edges over a given wavelength
 *         range.
 *
 * @details
 *
 * This function simply loops over the phot_top_prt (Topbase) and writes
 * to the screen whenever a threshold frequency for an edge falls
 * within the provided wavelength range.
 *
 * The wavelength range is queried within the function.
 *
 * ************************************************************************** */

void
inner_shell_wavelength_range(void)
{
  int n, nphot;
  double fmin, fmax, fthreshold;
  double wmin, wmax;

  if(query_wavelength_range(&wmin, &wmax) == FORM_QUIT)
    return;

  fmax = C / (wmin * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);

  display_add(" Wavelength range: %.2f - %.2f Angstroms", wmin, wmax);
  add_sep_display(ndash);
  inner_shell_header();

  n = 0;

  for(nphot = 0; nphot < n_inner_tot; ++nphot)
  {
    fthreshold = inner_cross_ptr[nphot]->freq[0];
    if(fthreshold > fmin && fthreshold < fmax)
    {
      inner_shell_line(nphot);
      n++;
    }
  }

  count(ndash, n);

  display_show(SCROLL_ENABLE, true, 4);
}

/* ************************************************************************** */
/**
 * @brief  Print all the bound free edges for an element.
 *
 * @details
 *
 * ************************************************************************** */

void
inner_shell_element(void)
{
  int n, z;
  int nphot;
  char element[LINELEN];

  if(query_atomic_number(&z) == FORM_QUIT)
    return;

  if(find_element(z) == ELEMENT_NO_FOUND)
    return;

  get_element_name(z, element);
  display_add("Inner shell ionization edges for %s", element);
  add_sep_display(ndash);
  inner_shell_header();

  n = 0;
  for(nphot = 0; nphot < n_inner_tot; ++nphot)
  {
    if(inner_cross_ptr[nphot]->z == z)
    {
      inner_shell_line(nphot);
      n++;
    }
  }

  count(ndash, n);

  display_show(SCROLL_ENABLE, true, 4);
}

/* ************************************************************************** */
/**
 * @brief  Print all the bound free edges for an ion.
 *
 * @details
 *
 * ************************************************************************** */

void
inner_shell_ion(void)
{
  int z, istate;
  int n, nion, nphot;
  char element[LINELEN];

  if(query_ion_input(TRUE, NULL, NULL, &nion) == FORM_QUIT)
    return;

  if(nion < 0)
    nion *= -1;

  if(nion > nions - 1)
  {
    error_atomix("Invaild ion number %i > nions %i", nion, nions);
    return;
  }

  z = ions[nion].z;
  istate = ions[nion].istate;
  get_element_name(z, element);

  display_add("Inner shell ionization edges for %s %i", element, istate);
  add_sep_display(ndash);
  inner_shell_header();

  n = 0;
  for(nphot = 0; nphot < n_inner_tot; ++nphot)
  {
    if(inner_cross_ptr[nphot]->z == z && inner_cross_ptr[nphot]->istate == istate)
    {
      inner_shell_line(nphot);
      n++;
    }
  }

  count(ndash, n);

  display_show(SCROLL_ENABLE, true, 4);

}
