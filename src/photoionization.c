/** ************************************************************************* */
/**
 * @file     photoionization.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for querying the photoionization data.
 *
 * ************************************************************************** */

#include "atomix.h"

static const
int ndash = 88;

const
MenuItem_t BOUND_FREE_MENU_CHOICES[] = {
  {&all_bound_free             , 0        , "All"                , "Print all of the possible transitions"},
  {&bound_free_wavelength_range, 1        , "By wavelength range", "Print the transitions over a given wavelength range"},
  {&bound_free_element         , 2        , "By element"         , "Print all the transitions for a given element"},
  {&bound_free_ion             , 3        , "By ion number"      , "Print all the transitions for a given ion"},
  {NULL                        , MENU_QUIT, "Return to main menu", ""}
};

/* ************************************************************************** */
/**
 * @brief  The main menu for photoionization queries.
 *
 * @details
 *
 * The main menu for photoionization queries is controlled by this function.
 *
 * ************************************************************************** */

void
bound_free_main_menu (void)
{
  int menu_index = 0;

  while (TRUE)
  {    
    menu_index = create_menu (CONTENT_WINDOW, "Bound-free transitions", BOUND_FREE_MENU_CHOICES,
                              ARRAY_SIZE (BOUND_FREE_MENU_CHOICES), menu_index, MENU_CONTROL);
    if (BOUND_FREE_MENU_CHOICES[menu_index].index == MENU_QUIT || menu_index == MENU_QUIT)
      return;
  }
}

/* ************************************************************************** */
/**
 * @brief  Add a header for the bound free table.
 *
 * @details
 *
 * ************************************************************************** */

void
bound_free_header (void)
{
  display_add (" %-12s %-12s %-12s %-12s %-12s %-12s %-12s", "Wavelength", "Element", "Z", "istate", "n", "l",
               "PhotInfo");
  add_sep_display (ndash);
}

/* ************************************************************************** */
/**
 * @brief  Standard line for a bound free line.
 *
 * @details
 *
 * The function bound_free_header will create an approprate header for these
 * lines.
 *
 * ************************************************************************** */

void
bound_free_line (int nphot)
{
  double wavelength;
  char element[LINELEN];

  get_element_name (phot_top_ptr[nphot]->z, element);
  wavelength = C_SI / phot_top_ptr[nphot]->freq[0] / ANGSTROM / 1e-2;
  display_add (" %-12.2f %-12s %-12i %-12i %-12i %-12i %-12i", wavelength, element, phot_top_ptr[nphot]->z,
               phot_top_ptr[nphot]->istate, phot_top_ptr[nphot]->n, phot_top_ptr[nphot]->l,
               ions[phot_top_ptr[nphot]->nion].phot_info);
}

/* ************************************************************************** */
/**
 * @brief  Print all the bound free edges in the atomic data.
 *
 * @details
 *
 * ************************************************************************** */

void
all_bound_free (void)
{
  int n, nphot;
  double wmin, wmax;

  wmin = C_SI / phot_top_ptr[0]->freq[0] / ANGSTROM / 1e-2;
  wmax = C_SI / phot_top_ptr[nphot_total - 1]->freq[0] / ANGSTROM / 1e-2;

  display_add ("Wavelength range: %.2f - %.2f Angstroms", wmin, wmax);
  add_sep_display (ndash);

  bound_free_header ();

  n = 0;

  for (nphot = 0; nphot < nphot_total; ++nphot)
  {
    n++;
    bound_free_line (nphot);
  }

  count (ndash, n);

  display_show (SCROLL_ENABLE);
}

/* ************************************************************************** */
/**
 * @brief  Retrieve all of the photoionization edges over a given wavelength
 *         range.
 *
 * @details
 *
 * This function simply loops over the phot_top_prt (Topbase) and writes
 * to the screen whenever a threshold frequency for an edge falls
 * within the provided wavelength range.
 *
 * The wavelength range is queried within the function.
 *
 * ************************************************************************** */

void
bound_free_wavelength_range (void)
{ 
  int n, nphot;
  double fmin, fmax, fthreshold;
  double wmin, wmax;

  if (query_wavelength_range (&wmin, &wmax) == FORM_QUIT)
    return;

  fmax = C / (wmin * ANGSTROM);
  fmin = C / (wmax * ANGSTROM);

  display_add(" Wavelength range: %.2f - %.2f Angstroms", wmin, wmax);
  add_sep_display (ndash);
  bound_free_header ();

  n = 0;

  for (nphot = 0; nphot < nphot_total; ++nphot)
  {
    fthreshold = phot_top_ptr[nphot]->freq[0];
    if (fthreshold > fmin && fthreshold < fmax)
    {
      bound_free_line (nphot);
      n++;
    }
  }

  count (ndash, n);

  display_show (SCROLL_ENABLE);
}

/* ************************************************************************** */
/**
 * @brief  Print all the bound free edges for an element.
 *
 * @details
 *
 * ************************************************************************** */

void
bound_free_element (void)
{
  error_atomix ("NOT IMPLEMENTED YET");
}

/* ************************************************************************** */
/**
 * @brief  Print all the bound free edges for an ion.
 *
 * @details
 *
 * ************************************************************************** */

void
bound_free_ion (void)
{
  error_atomix ("NOT IMPLEMENTED YET");
}
