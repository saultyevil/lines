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
  {&all_bound_bound             , 0        , "All"                , "Print all of the possible transitions"},
  {&bound_bound_wavelength_range, 1        , "By wavelength range", "Print the transitions over a given wavelength range"},
  {&bound_bound_element         , 2        , "By element"         , "Print all the transitions for a given element"},
  {&bound_bound_ion             , 3        , "By ion number"      , "Print all the transitions for a given ion"},
  {NULL                         , MENU_QUIT, "Return to main menu", ""}
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
bound_bound_header (void)
{
 display_add (" %-12s %-12s %-12s %-12s %-12s %-12s %-12s %-12s",
              "Wavelength", "Element", "Z", "istate", "levu", "levl", "nion", "macro info");
 add_sep_display (ndash);
}

/* ************************************************************************** */
/**
 * @brief 
 * 
 * @details
 *
 * ************************************************************************** */

void
bound_bound_line (int n, int hide)
{
  double wl;
  char element[LINELEN];

  get_element_name (lin_ptr[n]->z, element);
  wl = C_SI / lin_ptr[n]->freq / ANGSTROM / 1e-2;
  display_add (" %-12.2f %-12s %-12i %-12i %-12i %-12i %-12i %-12i", wl, element, lin_ptr[n]->z,
               lin_ptr[n]->istate, lin_ptr[n]->levu, lin_ptr[n]->levl, lin_ptr[n]->nion, lin_ptr[n]->macro_info);

  if (hide)
    return;
}

/* ************************************************************************** */
/**
 * @brief 
 * 
 * @details
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
    bound_bound_line (i, FALSE);

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
 * nline_min and nline_max set by the limit_lines() function.
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
    bound_bound_line (nline, FALSE);

  count (ndash, n);

  display_show (SCROLL_ENABLE);
}

/* ************************************************************************** */
/**
 * @brief 
 * 
 * @details
 *
 * ************************************************************************** */

void
bound_bound_element (void)
{
  error_atomix ("NOT IMPLEMENTED YET");
}

/* ************************************************************************** */
/**
 * @brief 
 * 
 * @details
 *
 * ************************************************************************** */

void
bound_bound_ion (void)
{
  error_atomix ("NOT IMPLEMENTED YET");
}
