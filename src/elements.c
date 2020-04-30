/* ************************************************************************** */
/**
 * @file     elements.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * TODO: print all elements
 * 
 * ************************************************************************** */

#include <math.h>

#include "atomix.h"

const
MenuItem_t ELEMENTS_MENU_CHOICES[] = {
  {&get_elements      , 0        , "All elements"  , "Query all elements in the atomic data"},
  {&get_single_element, 1        , "Single element", "Query a single element"},
  {NULL               , MENU_NULL, NULL            , NULL}
};

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
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

  menu_index = create_menu (CONTENT_WINDOW, "What do you want to query?", ELEMENTS_MENU_CHOICES, 
                            ARRAY_SIZE (ELEMENTS_MENU_CHOICES), menu_index, CONTROL_MENU);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
get_single_element (void)
{
  int i;
  int z;
  const int ndash = 40;
  struct elements element;
  int found = FALSE;

  query_atomic_number (&z);

  for (i = 0; i < nelements; ++i)
  {
    element = ele[i];
    if (element.z == z)
    {
      found = TRUE;
      break;
    }
  }

  if (found == FALSE)
  {
    error_atomix ("Element Z = %i is not in the atomic data", z);
    return;
  }

  add_to_display_buffer (" Element: %s", element.name);
  add_separator_to_buffer (ndash);
  add_to_display_buffer (" Z                        : %i", element.z);
  add_to_display_buffer (" Abundance relative to H  : %3.2f", log10 (element.abun) + 12);
  add_to_display_buffer (" Number of Ions           : %i", element.nions);
  add_to_display_buffer (" First Ion Index          : %i", element.firstion);
  add_to_display_buffer (" Last Ion Index           : %i", element.firstion + element.nions - 1);
  add_to_display_buffer (" Highest Ionisation state : %i", element.istate_max);
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
 get_elements (void)
 {
  int i;
  const int ndash = 40;
  struct elements element;

  add_separator_to_buffer (ndash);

  for (i = 0; i < nelements; ++i)
  {
    element = ele[i];
    add_to_display_buffer (" Element: %s", element.name);
    add_separator_to_buffer (ndash);
    add_to_display_buffer (" Z                        : %i", element.z);
    add_to_display_buffer (" Abundance relative to H  : %3.2f", log10 (element.abun) + 12);
    add_to_display_buffer (" Number of Ions           : %i", element.nions);
    add_to_display_buffer (" First Ion Index          : %i", element.firstion);
    add_to_display_buffer (" Last Ion Index           : %i", element.firstion + element.nions - 1);
    add_to_display_buffer (" Highest Ionisation state : %i", element.istate_max);
    add_separator_to_buffer (ndash);
  }


  display_text_buffer (CONTENT_WINDOW, SCROLL_OK);
 }
