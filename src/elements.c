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

#include <math.h>

#include "atomix.h"

static const
int ndash = 40;

const
MenuItem_t ELEMENTS_MENU_CHOICES[] = {
  {&get_elements      , 0        , "All elements"       , "Query all elements in the atomic data"},
  {&get_single_element, 1        , "Single element"     , "Query a single element"},
  {NULL               , MENU_QUIT, "Return to main menu", ""},
};

/* ************************************************************************** */
/**
 * @brief The main menu for element queries.
 *
 * @details
 *
 * This function is basically a wrapper around create_menu to allow a user to
 * choose the different querying options.
 *
 * ************************************************************************** */

void
elements_main_menu (void)
{
  static int menu_index = 0;

  if (ele == NULL)
  {
    error_atomix ("No elements have been read in. Unable to query!");
    return;
  }

  while (TRUE)
  {
    menu_index = create_menu (CONTENT_WINDOW, "Elements", ELEMENTS_MENU_CHOICES, ARRAY_SIZE (ELEMENTS_MENU_CHOICES), 
                              menu_index, CONTROL_MENU);
    if (ELEMENTS_MENU_CHOICES[menu_index].index == MENU_QUIT || menu_index == MENU_QUIT)
      return;
  }
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
add_element_to_display (struct elements e, int detailed)
{
  add_to_display (" Element: %s", e.name);
  add_separator_to_display (ndash);
  add_to_display (" Z                        : %i", e.z);
  add_to_display (" Abundance relative to H  : %3.2f", log10 (e.abun) + 12);
  add_to_display (" Number of Ions           : %i", e.nions);
  add_to_display (" First Ion Index          : %i", e.firstion);
  add_to_display (" Last Ion Index           : %i", e.firstion + e.nions - 1);
  add_to_display (" Highest Ionisation state : %i", e.istate_max);
  add_separator_to_display (ndash);

  if (!detailed)
    return;

  // TODO: add all bb and bf transitions
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
get_single_element (void)
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

  add_separator_to_display (ndash);
  add_element_to_display (ele[i], TRUE);
  display (CONTENT_WINDOW, SCROLL_OK);
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
 get_elements (void)
 {
  int i;

  add_separator_to_display (ndash);

  for (i = 0; i < nelements; ++i)
    add_element_to_display (ele[i], FALSE);

   display (CONTENT_WINDOW, SCROLL_OK);
 }
