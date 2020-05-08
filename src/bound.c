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

static const
int ndash = 103;  

/* ************************************************************************** */
/**
 * @brief  Add a header for a bound_bound_line table.
 * 
 * @details
 *
 * ************************************************************************** */

void
bound_bound_header (void)
{
 display_add (" %-12s %-12s %-12s %-12s %-12s %-12s %-12s %-12s",
              "Wavelength", "Element", "Z", "istate", "levu", "levl", "nion", "macro info");
 add_sep_display (ndash);
}

/* ************************************************************************** */
/**
 * @brief  Standard layout for a bound bound transition line.
 * 
 * @details
 *
 * The function bound_bound_header will create an appropriate header for one
 * of these lines.
 *
 * ************************************************************************** */

void
bound_bound_line (int n)
{
  double wl;
  char element[LINELEN];

  get_element_name (lin_ptr[n]->z, element);
  wl = C_SI / lin_ptr[n]->freq / ANGSTROM / 1e-2;
  display_add (" %-12.2f %-12s %-12i %-12i %-12i %-12i %-12i %-12i", wl, element, lin_ptr[n]->z,
               lin_ptr[n]->istate, lin_ptr[n]->levu, lin_ptr[n]->levl, lin_ptr[n]->nion, lin_ptr[n]->macro_info);
}

/* ************************************************************************** */
/**
 * @brief  Print all the bound-bound transitions in the data set.
 * 
 * @details
 *
 * Iterates over the lin_ptr array, which is ordered by frequency. For atomic
 * data sets, there can be some lines with very large wavelengths.
 *
 * ************************************************************************** */

void
all_bound_bound (void)
{
  int i;
  double wmin, wmax;

  wmin = C_SI / lin_ptr[nlines - 1]->freq / ANGSTROM / 1e-2;
  wmax = C_SI / lin_ptr[0]->freq / ANGSTROM / 1e-2;

  display_add ("Wavelength range: %.2f - %.2f Angstroms", wmin, wmax);
  add_sep_display (ndash);

  bound_bound_header ();

  for (i = 0; i < nlines; ++i)
    bound_bound_line (i);

  count (ndash, nlines);

  display_show (SCROLL_ENABLE);
}

/* ************************************************************************** */
/**
 * @brief Retrieve all the bound bound transitions over a given wavelength
 *        range.
 *
 * @details
 *
 * This function simply loops over the lin_ptr struct between the limits
 * nline_min and nline_max set by the limit_lines() function. The wavelength
 * limits are queried within the function.
 * 
 * ************************************************************************** */

void
bound_bound_wavelength_range (void)
{
  int n, nline;
  double wmin, wmax;

  if (query_wavelength_range (&wmin, &wmax) == FORM_QUIT)
    return;

  limit_lines (C / (wmax * ANGSTROM), C / (wmin * ANGSTROM));
  n = nline_max - nline_min - 1;

  display_add ("Wavelength range: %.2f - %.2f Angstroms", wmin, wmax);
  add_sep_display (ndash);
  bound_bound_header ();

  for (nline = nline_min + 1; nline < nline_max; ++nline)
    bound_bound_line (nline);

  count (ndash, n);

  display_show (SCROLL_ENABLE);
}

/* ************************************************************************** */
/**
 * @brief  Print all bound bound transitions for a given element.
 * 
 * @details
 *
 * ************************************************************************** */

void
bound_bound_element (void)
{
  int n, z;
  int nline;
  char element[LINELEN];

  if (query_atomic_number (&z) == FORM_QUIT)
    return;

  if (find_element (z) == ELEMENT_NO_FOUND)
    return;

  get_element_name (z, element);
  display_add ("Bound-bound transitions for %s", element);
  add_sep_display (ndash);
  bound_bound_header ();

  n = 0;
  for (nline = 0; nline < nlines; ++nline)
  {
    if (lin_ptr[nline]->z == z)
    {
      bound_bound_line (nline);
      n++;
    }
  }

  count (ndash, n);

  display_show (SCROLL_ENABLE);
}

/* ************************************************************************** */
/**
 * @brief  Print all bound bound transitions for a given ion.
 * 
 * @details
 *
 * The ion is specified by the ion number. It would be possibleto add a small
 * menu to select to select an ion by ion number or atomic number and ionisation
 * state with the exeisting machinery.
 *
 * ************************************************************************** */

void
bound_bound_ion (void)
{
  int z, istate;
  int n, nion, nline;
  char element[LINELEN];

  if (query_ion_input (TRUE, NULL, NULL, &nion) == FORM_QUIT)
    return;

  if (nion < 0)
    nion *= -1;

  if (nion > nions - 1)
  {
    error_atomix ("Invaild ion number %i > nions %i", nion, nions);
    return;
  }

  z = ions[nion].z;
  istate = ions[nion].istate;
  get_element_name (z, element);

  display_add ("Bound-bound transitions for %s %i", element, istate);
  add_sep_display (ndash);
  bound_bound_header ();

  n = 0;
  for (nline = 0; nline < nlines; ++nline)
  {
    if (lin_ptr[nline]->z == z && lin_ptr[nline]->istate == istate)
    {
      bound_bound_line (nline);
      n++;
    }
  }

  count (ndash, n);

  display_show (SCROLL_ENABLE);
}
