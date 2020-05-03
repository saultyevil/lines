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

const
MenuItem_t BOUND_MENU_CHOICES[] = {
  {&get_all_bound_bound     , 0        , "All"                , "Print all of the possible transitions"},
  {&get_bound_bound_wl_range, 1        , "By wavelength range", "Print the transitions over a given wavelength range"},
  {&get_bound_bound_element , 2        , "By element"         , "Print all the transitions for a given element"},
  {&get_bound_bound_ion     , 3        , "By ion number"      , "Print all the transitions for a given ion"},
  {NULL                     , MENU_QUIT, "Return to main menu", ""}  
};

/* ************************************************************************** */
/**
 * @brief  The main menu for bound bound transition queries.
 * 
 * @details
 *
 * ************************************************************************** */

void
bound_bound_main_menu (void)
{
  static int menu_index = 0;

  if (nlines == 0)
  {
    error_atomix ("No bound-bound transitions were read in. Unable to query!");
    return;
  }

  while (TRUE)
  {
    menu_index = create_menu (CONTENT_WINDOW, "Bound-bound transitions", BOUND_MENU_CHOICES,
                              ARRAY_SIZE (BOUND_MENU_CHOICES), menu_index, CONTROL_MENU);
    if (BOUND_MENU_CHOICES[menu_index].index == MENU_QUIT || menu_index == MENU_QUIT)
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
 add_bound_bound_header_to_display (void)
 {
    add_to_display (" %-12s %-12s %-12s %-12s %-12s %-12s %-12s %-12s", 
                    "Wavelength", "Element", "Z", "istate", "levu", "levl", "nion", "macro info");
    add_separator_to_display (ndash);
 }

/* ************************************************************************** */
/**
 * @brief 
 * 
 * @details
 *
 * ************************************************************************** */

void
add_bound_line_to_display (int n, int hide)
{
  double wl;
  char element[LINELEN];

  get_element_name (lin_ptr[n]->z, element);
  wl = const_C_SI / lin_ptr[n]->freq / ANGSTROM / 1e-2;
  add_to_display (" %-12.2f %-12s %-12i %-12i %-12i %-12i %-12i %-12i", wl, element, lin_ptr[n]->z, 
                  lin_ptr[n]->istate, lin_ptr[n]->levu, lin_ptr[n]->levl, lin_ptr[n]->nion, lin_ptr[n]->macro_info);
}

/* ************************************************************************** */
/**
 * @brief 
 * 
 * @details
 *
 * ************************************************************************** */

void
add_number_of_entries_to_display (int count)
{
  add_separator_to_display (ndash);
  add_to_display ("%i entries", count);
  add_separator_to_display (ndash);
}

/* ************************************************************************** */
/**
 * @brief 
 * 
 * @details
 *
 * ************************************************************************** */

void
get_all_bound_bound (void)
{
  int i;
  double wmin, wmax;

  wmin = const_C_SI / lin_ptr[nlines - 1]->freq / ANGSTROM / 1e-2;
  wmax = const_C_SI / lin_ptr[0]->freq / ANGSTROM / 1e-2;

  add_to_display ("Wavelength range %.2f - %.2f Angstroms", wmin, wmax);
  add_separator_to_display (ndash);

  add_bound_bound_header_to_display ();

  for (i = 0; i < nlines; ++i)
    add_bound_line_to_display (i, FALSE);

  add_number_of_entries_to_display (nlines);

  display (CONTENT_WINDOW, SCROLL_OK);
}

/* ************************************************************************** */
/**
 * @brief Retrieve all the bound bound transitions over a given wavelength
 *        range.
 *
 * @details
 *
 * This function simply loops over the lin_ptr struct between the limits
 * nline_min and nline_max set by the limit_lines() function.
 * 
 * ************************************************************************** */

void
get_bound_bound_wl_range (void)
{
  int n, nline;
  double wmin, wmax;

  if (query_wavelength_range (&wmin, &wmax) == FORM_QUIT)
    return;

  limit_lines (C / (wmax * ANGSTROM), C / (wmin * ANGSTROM));
  n = nline_max - nline_min - 1;

  add_to_display ("Wavelength range %.2f - %.2f Angstroms", wmin, wmax);
  add_separator_to_display (ndash);
  add_bound_bound_header_to_display ();

  for (nline = nline_min + 1; nline < nline_max; ++nline)
    add_bound_line_to_display (nline, FALSE);

  add_number_of_entries_to_display (n);

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
get_bound_bound_element (void)
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
get_bound_bound_ion (void)
{

}
