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
  {&get_ion        , 0        , "All ions"    , "Query all ions in the atomic data"},
  {&get_all_ions   , 1        , "Single Ion"  , "Query a single ion"},
  {&get_ion_element, 2        , "Element ions", "Query all the ions for an element"},
  {NULL            , MENU_NULL, NULL          , NULL}
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

	menu_index = create_menu (CONTENT_WINDOW, "What do you want to query?", IONS_MENU_CHOICES, 
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
get_ion (void)
{
	int nion = 20;

	const int ndash = 40;
	struct ions mion;
	mion = ion[nion];

	add_separator_to_buffer (ndash);
	add_to_display_buffer (" Ion number       : %i", nion);
	add_to_display_buffer (" Atomic number    : %i", mion.z);
	add_to_display_buffer (" Number of ions   : %i", ele[mion.nelem].nions);
	add_to_display_buffer (" Ionisation state : %i", mion.istate);
	add_separator_to_buffer (ndash);

	display_text_buffer (CONTENT_WINDOW, SCROLL_OK);
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