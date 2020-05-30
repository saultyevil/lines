/* ************************************************************************** */
/**
 * @file     ions.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 *  Functions for querying
 *
 * ************************************************************************** */

#include <stdbool.h>

#include "atomix.h"

static const
int ndash = 43;

/* ************************************************************************** */
/**
 * @brief  Standard layout for printing information about an ion.
 *
 * @param[in]  nion      The ion number
 * @param[in]  detailed  If TRUE, the BB and BF information will be printed
 * @param[in]  basic     If TRUE, the element information will be printed
 *
 * @details
 *
 * Has a few more arguments than really required to allow the re-use of this
 * function for different sub-routines.
 *
 * ************************************************************************** */

void
ion_line (int nion, int detailed, int basic)
{
  int i, n;
  double wavelength;
  char element[LINELEN];
  struct ions ion;

  ion = ions[nion];
  get_element_name (ion.z, element);

  if (basic)
    display_add (" Element                     : %s", element);
  add_sep_display (ndash);
  if (basic)
    display_add (" Atomic number               : %i", ion.z);
  display_add (" Ion number                  : %i", nion);
  display_add (" Ionisation state            : %i", ion.istate);
  display_add (" Photionization info         : %i", ion.phot_info);
  display_add (" Ionisation potential        : %.2e eV", ion.ip / EV2ERGS);
  if (basic)
  {
    display_add (" Number of ions for element  : %i", ele[ion.nelem].nions);
    add_sep_display (ndash);
  }

  if (!detailed)
    return;

  display_add (" Bound-bound transitions for this ion");
  add_sep_display (ndash);
  display_add (" %-12s %-12s %-12s", "Wavelength", "levu", "levl");

  n = 0;

  for (i = 0; i < nlines; ++i)
  {
    if (lin_ptr[i]->z == ion.z && lin_ptr[i]->istate == ion.istate)
    {
      n++;
      wavelength = C_SI / lin_ptr[i]->freq / ANGSTROM / 1e-2;
      display_add (" %-12.2f %-12i %-12i", wavelength, lin_ptr[i]->levu, lin_ptr[i]->levl);
    }
  }

  add_sep_display (ndash);
  display_add (" %i lines", n);
  add_sep_display (ndash);
  display_add (" Bound-free transitions for this ion: ");
  add_sep_display (ndash);
  display_add (" %-12s %-12s %-12s", "Wavelength", "n", "l");

  n = 0;

  for (i = 0; i < nphot_total; ++i)
  {
    if (phot_top[i].z == ion.z && phot_top[i].istate == ion.istate)
    {
      n++;
      wavelength = C_SI / phot_top[i].freq[0] / ANGSTROM / 1e-2;
      display_add (" %-12.2f %-12i %-12i", wavelength, phot_top[i].n, phot_top[i].l);
    }
  }

  add_sep_display (ndash);
  display_add (" %i edges", n);
  add_sep_display (ndash);
}

/* ************************************************************************** */
/**
 * @brief  Print all of the ions in the atomic data.
 *
 * @details
 *
 * ************************************************************************** */

void
all_ions (void)
{
  int nion;

  add_sep_display (ndash);

  for (nion = 0; nion < nions; ++nion)
    ion_line (nion, FALSE, TRUE);

  count (ndash, nions);

  display_show (SCROLL_ENABLE, false, 0);
}

/* ************************************************************************** */
/**
 * @brief  Print detailed information about a single ion.
 *
 * @details
 *
 * The atomic number Z and ionization state are queried.
 *
 * ************************************************************************** */

void
single_ion_atomic_z (void)
{
  int nion;
	int z, istate;
  int found = FALSE;

  if (query_ion_input (FALSE, &z, &istate, NULL) == FORM_QUIT)
    return;

  if (find_element (z) == ELEMENT_NO_FOUND)
    return;

  for (nion = 0; nion < nions; ++nion)
  {
    if (ions[nion].z == z && ions[nion].istate == istate)
    { 
      found = TRUE; 
      break;
    }
  }

  if (!found)
  {
    error_atomix ("Unknown ion configuration");
    return;
  }

  add_sep_display (ndash);
  ion_line (nion, TRUE, TRUE);
  display_show (SCROLL_ENABLE, false, 0);
}

/* ************************************************************************** */
/**
 * @brief  Print detailed information about a single ion.
 *
 * @details
 *
 * The ion number of the ion is queried.
 *
 * ************************************************************************** */

void
single_ion_nion (void)
{
  int nion;

  if (query_ion_input (TRUE, NULL, NULL, &nion) == FORM_QUIT)
    return;

  if (nion < 0)
    nion *= -1;

  if(nion > nions - 1)
  {
    error_atomix ("Invalid ion index choice %i when there are only %i ion indices", nion, nions);
    return;
  }

  add_sep_display (ndash);
  ion_line (nion, TRUE, TRUE);
  display_show (SCROLL_ENABLE, false, 0);
}

/* ************************************************************************** */
/**
 * @brief  Print all the ions for a given element.
 *
 * @details
 *
 * ************************************************************************** */

void
ions_for_element (void)
{
  int nion, n;
  int z, firstion, lastion;

  if (query_atomic_number (&z) == FORM_QUIT)
    return;

  if ((n = find_element (z)) == ELEMENT_NO_FOUND)
    return;

  firstion = ele[n].firstion;
  lastion = ele[n].firstion + ele[n].nions - 1;

  add_sep_display (ndash);
  display_add (" There are %i ions for %s", lastion - firstion, ele[n].name);

  for (nion = firstion; nion < lastion; ++nion)
    ion_line (nion, FALSE, FALSE);

  add_sep_display (ndash);

  display_show (SCROLL_ENABLE, false, 0);
}
