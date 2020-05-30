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

static const
int ndash = 40;

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
element_line (struct elements e, int detailed)
{
  display_add (" Element: %s", e.name);
  add_sep_display (ndash);
  display_add (" Z                        : %i", e.z);
  display_add (" Abundance relative to H  : %3.2f", log10 (e.abun) + 12);
  display_add (" Number of Ions           : %i", e.nions);
  display_add (" First Ion Index          : %i", e.firstion);
  display_add (" Last Ion Index           : %i", e.firstion + e.nions - 1);
  display_add (" Highest Ionisation state : %i", e.istate_max);
  add_sep_display (ndash);

  if (!detailed)
    return;

  // TODO: add all bb and bf transitions
}

/* ************************************************************************** */
/**
 * @brief Prints details of all the elements in the atomic data.
 *
 * @details
 *
 * Loops over the entire ele structre to and prints each one.
 *
 * ************************************************************************** */

void
all_elements (void)
{
  int i;

  add_sep_display (ndash);

  for (i = 0; i < nelements; ++i)
    element_line (ele[i], FALSE);

  count (ndash, nelements);

  display_show (SCROLL_ENABLE, false, 0);
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
single_element (void)
{
  int i;
  int atomic_z;
  int found = FALSE;

  if (query_atomic_number (&atomic_z) == FORM_QUIT)
    return;

  for (i = 0; i < nelements; ++i)
  {
    if (ele[i].z == atomic_z)
    {
      found = TRUE;
      break;
    }
  }

  if (!found)
  {
    error_atomix ("Element Z = %i is not in the atomic data", atomic_z);
    return;
  }

  add_sep_display (ndash);
  element_line (ele[i], TRUE);
  display_show (SCROLL_ENABLE, false, 0);
}
