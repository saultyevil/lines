/* ************************************************************************** */
/**
 * @file     ions.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * ************************************************************************** */

#include "atomix.h"

const
MenuItem_t IONS_MENU_CHOICES[] = {
  {&get_all_ions   , 0        , "All ions"          , "Query all ions in the atomic data"},
  {&get_ion_z      , 1        , "Single ion by Z"   , "Query a single ion by atomic number and ionisation state"},
  {&get_ion_nion   , 2        , "Single ion by nion", "Query a single ion by ion number"},
  {&get_ion_element, 3        , "Element ions"      , "Query all the ions for an element"},
  {NULL            , MENU_NULL, NULL                , NULL}
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

	menu_index = create_menu (CONTENT_WINDOW, "What do you want to do?", IONS_MENU_CHOICES, 
		                      ARRAY_SIZE (IONS_MENU_CHOICES), menu_index, CONTROL_MENU);

}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
get_ion_z (void)
{
  int nion;
	int z, istate;
  int form_return;
  int found = FALSE;
  char element[LINELEN];

	const int ndash = 40;
	struct ions mion;

  form_return = query_ion_input (FALSE, &z, &istate, NULL);
  if (form_return == FORM_QUIT)
    return;

  for (nion = 0; nion < nions; ++nion)
  {
    mion = ion[nion];
    if (mion.z == z && mion.istate == istate)
    {
      found = TRUE;
      break;
    }
  }

  if (found == FALSE)
  {
    error_atomix ("Unable to find configuration Z = %i Ionisation State = %i", z, istate);
    return;
  }

  get_element_name (mion.z, element);

  add_separator_to_display (ndash);
  add_to_display (" Ion number       : %i", nion);
  add_to_display (" Element          : %s", element);
  add_to_display (" Atomic number    : %i", mion.z);
  add_to_display (" Number of ions   : %i", ele[mion.nelem].nions);
  add_to_display (" Ionisation state : %i", mion.istate);
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
get_ion_nion (void)
{
  int nion;
  char element[LINELEN];
  int form_return;

  const int ndash = 40;
  struct ions mion;
  
  form_return = query_ion_input (TRUE, NULL, NULL, &nion);
  if (form_return == FORM_QUIT)
    return;

  if (nion < 0)
    nion *= -1;

  if(nion > nions)
  {
    error_atomix ("Invalid ion choice %i when there are only %i ions", nion, nions);
    return;
  }

  mion = ion[nion];
  get_element_name (mion.z, element);

  add_separator_to_display (ndash);
  add_to_display (" Ion number       : %i", nion);
  add_to_display (" Element          : %s", element);
  add_to_display (" Atomic number    : %i", mion.z);
  add_to_display (" Number of ions   : %i", ele[mion.nelem].nions);
  add_to_display (" Ionisation state : %i", mion.istate);
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
  const int ndash = 40;
  struct ions mion;

  add_separator_to_display (ndash);

  for (nion = 0; nion < nions; ++nion)
  {
    mion = ion[nion];
    add_to_display (" Ion number       : %i", nion);
    add_to_display (" Atomic number    : %i", mion.z);
    add_to_display (" Number of ions   : %i", ele[mion.nelem].nions);
    add_to_display (" Ionisation state : %i", mion.istate);
    add_separator_to_display (ndash);
  }

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

}