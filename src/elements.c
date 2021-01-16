/* ************************************************************************** */
/**
 * @file     elements.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for querying the elements in the atomic data.
 *
 * ************************************************************************** */

#include <stdbool.h>
#include <math.h>

#include "atomix.h"

static const int ndash = 45;
static const int ndash_line = 94;

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 *
 * ************************************************************************** */

void
elements_header(void)
{
  add_sep_display(ndash_line);
  display_add(" %-12s %-12s %-12s %-12s %-12s %-12s %-12s", "Element", "Z", "Abundance", "Ions", "First Ion",
              "Last Ion", "Max Ionisation");
  add_sep_display(ndash_line);
}

/* ************************************************************************** */
/**
 * @brief  Print standard information about an element to screen.
 *
 * @param[in]  e  The element to print to screen
 *
 * @details
 *
 * Prints an element to screen using a standardised output.
 *
 * ************************************************************************** */

void
element_line(struct elements e)
{
  display_add(" %-12s %-12i %-12.2f %-12i %-12i %-12i %-12i", e.name, e.z, log10(e.abun) + 12, e.nions, e.firstion,
              e.firstion + e.nions - 1, e.istate_max);
}

/* ************************************************************************** */
/**
 * @brief  Print standard information about an element to screen.
 *
 * @param[in]  e  The element to print to screen
 *
 * @details
 *
 * Prints an element to screen using a standardised output.
 *
 * ************************************************************************** */

void
single_element_info(struct elements e, int detailed)
{
  int i, n;
  double wavelength;

  display_add(" Element: %s", e.name);
  add_sep_display(ndash);
  display_add(" Z                        : %i", e.z);
  display_add(" Abundance relative to H  : %3.2f", log10(e.abun) + 12);
  display_add(" Number of Ions           : %i", e.nions);
  display_add(" First Ion Index          : %i", e.firstion);
  display_add(" Last Ion Index           : %i", e.firstion + e.nions - 1);
  display_add(" Highest Ionisation state : %i", e.istate_max);
  add_sep_display(ndash);

  if(!detailed)
    return;

  display_add(" Bound-bound transitions for this element");
  add_sep_display(ndash);
  display_add(" %-12s %-12s %-12s %-12s", "Ionisation", "Wavelength", "levu", "levl");

  n = 0;

  for(i = 0; i < nlines; ++i)
  {
    if(lin_ptr[i]->z == e.z)
    {
      n++;
      wavelength = C_SI / lin_ptr[i]->freq / ANGSTROM / 1e-2;
      display_add(" %-12i %-12.2f %-12i %-12i", lin_ptr[i]->istate, wavelength, lin_ptr[i]->levu, lin_ptr[i]->levl);
    }
  }

  add_sep_display(ndash);
  display_add(" %i lines", n);

  add_sep_display(ndash);
  display_add(" Bound-free transitions for this element ");
  add_sep_display(ndash);
  display_add(" %-12s %-12s %-12s %-12s", "Ionisation", "Wavelength", "n", "l");

  n = 0;

  for(i = 0; i < nphot_total; ++i)
  {
    if(phot_top_ptr[i]->z == e.z)
    {
      n++;
      wavelength = C_SI / phot_top_ptr[i]->freq[0] / ANGSTROM / 1e-2;
      display_add(" %-12i %-12.2f %-12i %-12i", phot_top_ptr[i]->istate, wavelength, phot_top_ptr[i]->n,
                  phot_top_ptr[i]->l);
    }
  }

  add_sep_display(ndash);
  display_add(" %i photoionization edges", n);
  add_sep_display(ndash);

  add_sep_display(ndash);
  display_add(" Inner Shell transitions for this element ");
  add_sep_display(ndash);
  display_add(" %-12s %-12s %-12s %-12s", "Ionisation", "Wavelength", "n", "l");

  n = 0;

  for(i = 0; i < n_inner_tot; ++i)
  {
    if(inner_cross_ptr[i]->z == e.z)
    {
      n++;
      wavelength = C_SI / inner_cross_ptr[i]->freq[0] / ANGSTROM / 1e-2;
      display_add(" %-12i %-12.2f %-12i %-12i", inner_cross_ptr[i]->istate, wavelength, inner_cross_ptr[i]->n,
                  inner_cross_ptr[i]->l);
    }
  }

  add_sep_display(ndash);
  display_add(" %i inner shell edges", n);
  add_sep_display(ndash);

}

/* ************************************************************************** */
/**
 * @brief Prints details of all the elements in the atomic data.
 *
 * @details
 *
 * Loops over the entire ele structure to and prints each one.
 *
 * ************************************************************************** */

void
all_elements(void)
{
  int i;

  elements_header();

  for(i = 0; i < nelements; ++i)
    element_line(ele[i]);

  count(ndash_line, nelements);

  display_show(SCROLL_ENABLE, true, 3);
}

/* ************************************************************************** */
/**
 * @brief  Prints details of a single element to screen.
 *
 * @details
 *
 * Queries the user for the atomic number of the element to be printed out. If
 * the element is not in the atomic data, then an error message is printed
 * instead.
 *
 * ************************************************************************** */

void
single_element(void)
{
  int i;
  int atomic_z;
  int found = false;

  if(query_atomic_number(&atomic_z) == FORM_QUIT)
    return;

  for(i = 0; i < nelements; ++i)
  {
    if(ele[i].z == atomic_z)
    {
      found = true;
      break;
    }
  }

  if(!found)
  {
    error_atomix("Element Z = %i is not in the atomic data", atomic_z);
    return;
  }

  add_sep_display(ndash);
  single_element_info(ele[i], true);
  display_show(SCROLL_ENABLE, false, 0);
}
