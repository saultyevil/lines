/* ************************************************************************** */
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

#include <stdbool.h>

#include "atomix.h"

static const int ndash = 99;

/* ************************************************************************** */
/**
 * @brief  Add a header for the bound free table.
 *
 * @details
 *
 * ************************************************************************** */

void
bound_free_header(void)
{
  display_add(" %-12s %-12s %-12s %-12s %-12s %-12s %-12s %-12s", "Wavelength", "Element", "Z", "istate", "n", "l",
              "PhotInfo", "nres");
  add_sep_display(ndash);
}

/* ************************************************************************** */
/**
 * @brief  Standard line for a bound free line.
 *
 * @details
 *
 * The function bound_free_header will create an approprate header for these
 * lines.
 *
 * ************************************************************************** */

void
bound_free_line(int nphot)
{
  double wavelength;
  char element[LINELEN];

  get_element_name(phot_top[nphot].z, element);
  wavelength = C_SI / phot_top[nphot].freq[0] / ANGSTROM / 1e-2;
  display_add(" %-12.2f %-12s %-12i %-12i %-12i %-12i %-12i %-12i", wavelength, element, phot_top[nphot].z,
              phot_top[nphot].istate, phot_top[nphot].n, phot_top[nphot].l,
              ions[phot_top[nphot].nion].phot_info, 1 + NLINES + nphot);
}

/* ************************************************************************** */
/**
 * @brief  Print all the bound free edges in the atomic data.
 *
 * @details
 *
 * ************************************************************************** */

void
all_bound_free(void)
{
  int n, nphot;
  double wmin, wmax;

  wmin = C_SI / phot_top_ptr[0]->freq[0] / ANGSTROM / 1e-2;
  wmax = C_SI / phot_top_ptr[nphot_total - 1]->freq[0] / ANGSTROM / 1e-2;

  display_add("Wavelength range: %.2f - %.2f Angstroms", wmin, wmax);
  add_sep_display(ndash);

  bound_free_header();

  n = 0;

  for(nphot = 0; nphot < nphot_total; ++nphot)
  {
    n++;
    bound_free_line(nphot);
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
bound_free_wavelength_range(void)
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
  bound_free_header();

  n = 0;

  for(nphot = 0; nphot < nphot_total; ++nphot)
  {
    fthreshold = phot_top[nphot].freq[0];
    if(fthreshold > fmin && fthreshold < fmax)
    {
      bound_free_line(nphot);
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
bound_free_element(void)
{
  int n, z;
  int nphot;
  char element[LINELEN];

  if(query_atomic_number(&z) == FORM_QUIT)
    return;

  if(find_element(z) == ELEMENT_NO_FOUND)
    return;

  get_element_name(z, element);
  display_add("Bound-free edges for %s", element);
  add_sep_display(ndash);
  bound_free_header();

  n = 0;
  for(nphot = 0; nphot < nphot_total; ++nphot)
  {
    if(phot_top[nphot].z == z)
    {
      bound_free_line(nphot);
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
bound_free_ion(void)
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

  display_add("Bound-free transitions for %s %i", element, istate);
  add_sep_display(ndash);
  bound_free_header();

  n = 0;
  for(nphot = 0; nphot < nphot_total; ++nphot)
  {
    if(phot_top[nphot].z == z && phot_top[nphot].istate == istate)
    {
      bound_free_line(nphot);
      n++;
    }
  }

  count(ndash, n);

  display_show(SCROLL_ENABLE, true, 4);

}
