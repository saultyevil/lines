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

#include "atomix.h"

static const
int ndash = 43;

const
MenuItem_t IONS_MENU_CHOICES[] = {
  {&get_all_ions   , 0        , "All ions"                   , "Print all the ions in the atomic data"},
  {&get_ion_element, 1        , "Ions for an element"        , "Print all the ions for an element"},
  {&get_ion_z      , 2        , "Single ion by atomic number", "Detailed output for a single ion by atomic number and ionisation state"},
  {&get_ion_nion   , 3        , "Single ion by ion number"   , "Detailed output for a single ion by ion number"},
  {NULL            , MENU_QUIT, "Return to main menu"        , ""},
};

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
ions_main_menu (void)
{
	static int menu_index;

	if (nions == 0)
	{
		error_atomix ("No ions have been read in. Unable to query!");
		return;
	}

	while (TRUE)
  {
    menu_index = create_menu (CONTENT_WINDOW, "Ions", IONS_MENU_CHOICES, ARRAY_SIZE (IONS_MENU_CHOICES), menu_index, 
                              CONTROL_MENU);
    if (IONS_MENU_CHOICES[menu_index].index == MENU_QUIT || menu_index == MENU_QUIT)
      return;
  }
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
add_ion_to_display (int nion, int detailed, int basic)
{
  int i;
  int count;
  double wl;
  char element[LINELEN];
  struct ions ion;

  ion = ions[nion];
  get_element_name (ion.z, element);

  if (basic)
    add_to_display (" Element                     : %s", element);
  add_separator_to_display (ndash);
  if (basic)
    add_to_display (" Atomic number               : %i", ion.z);
  add_to_display (" Ion number                  : %i", nion);
  add_to_display (" Ionisation state            : %i", ion.istate);
  add_to_display (" Photionization info         : %i", ion.phot_info);
  add_to_display (" Ionisation potential        : %.2e eV", ion.ip / EV2ERGS);
  if (basic)
  {
    add_to_display (" Number of ions for element  : %i", ele[ion.nelem].nions);
    add_separator_to_display (ndash);
  }

  if (!detailed)
    return;

  add_to_display (" Bound-bound transitions for this ion");
  add_separator_to_display (ndash);
  add_to_display (" %-12s %-12s %-12s", "Wavelength", "levu", "levl");

  count = 0;

  for (i = 0; i < nlines; ++i)
  {
    if (lin_ptr[i]->z == ion.z && lin_ptr[i]->istate == ion.istate)
    {
      count++;
      wl = const_C_SI / lin_ptr[i]->freq / ANGSTROM / 1e-2;
      add_to_display (" %-12.2f %-12i %-12i", wl, lin_ptr[i]->levu, lin_ptr[i]->levl);
    }
  }

  add_separator_to_display (ndash);
  add_to_display (" %i lines", count);
  add_separator_to_display (ndash);
  add_to_display (" Bound-free transitions for this ion: ");
  add_separator_to_display (ndash);
  add_to_display (" %-12s %-12s %-12s", "Wavelength", "n", "l");

  count = 0;

  for (i = 0; i < nphot_total; ++i)
  {
    if (phot_top[i].z == ion.z && phot_top[i].istate == ion.istate)
    {
      count++;
      wl = const_C_SI / phot_top[i].freq[0] / ANGSTROM / 1e-2;
      add_to_display (" %-12.2f %-12i %-12i", wl, phot_top[i].n, phot_top[i].l);
    }
  }

  add_separator_to_display (ndash);
  add_to_display (" %i edges", count);
  add_separator_to_display (ndash);
}


/* ************************************************************************** */
/**
 * @brief  Print detailed information about a single ion.
 *
 * @details
 *
 * ************************************************************************** */

void
get_ion_z (void)
{
  int nion;
	int z, istate;
  int found = FALSE;

  if (query_ion_input (FALSE, &z, &istate, NULL) == FORM_QUIT)
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
    error_atomix ("Unable to find configuration Z = %i : Ionisation State = %i", z, istate);
    return;
  }

  add_separator_to_display (ndash);
  add_ion_to_display (nion, TRUE, TRUE);
  display (CONTENT_WINDOW, SCROLL_OK);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
get_ion_nion (void)
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

  add_separator_to_display (ndash);
  add_ion_to_display (nion, TRUE, TRUE);
  display (CONTENT_WINDOW, SCROLL_OK);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
get_ion_element (void)
{
  int i, nion;
  int z, firston, lastion;
  int found = FALSE;

  if (query_atomic_number (&z) == FORM_QUIT)
    return;

  for (i = 0; i < nelements; ++i)
  {
    if (ele[i].z == z)
    {
      found = TRUE;
      firston = ele[i].firstion;
      lastion = firston + ele[i].nions - 1;
      break;
    }
  }

  if (!found)
  {
    error_atomix ("Element Z = %i is not in the atomic data", z);
    return;
  }

  add_separator_to_display (ndash);
  add_to_display (" There are %i ions for %s", lastion - firston, ele[i].name);

  for (nion = firston; nion < lastion; ++nion)
    add_ion_to_display (nion, FALSE, FALSE);

  add_separator_to_display (ndash);

  display (CONTENT_WINDOW, SCROLL_OK);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
get_all_ions (void)
{
  int nion;

  add_separator_to_display (ndash);

  for (nion = 0; nion < nions; ++nion)
    add_ion_to_display (nion, FALSE, TRUE);

  display (CONTENT_WINDOW, SCROLL_OK);
}
