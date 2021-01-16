/* ************************************************************************** */
/**
 * @file     menu.h
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Cold storage for the MenuItem_t variables for all of the menus in Atomix.
 *
 * ************************************************************************** */

#include "atomix.h"

// const
MenuItem_t MAIN_MENU_CHOICES[] = {
  {&elements_main_menu, 0, "Elements", "Query the elements in the atomic data"},
  {&levels_main_menu, 1, "Atomic Levels", "Query the atomic configurations"},
  {&ions_main_menu, 2, "Ions", "Query the ions in the atomic data"},
  {&bound_bound_main_menu, 3, "Bound-Bound", "Query possible bound-bound transitions"},
  {&bound_free_main_menu, 4, "Bound-Free", "Query the photionization edges"},
  {&inner_shell_main_menu, 5, "Inner-Shell", "Query inner shell ionization edges"},
  {&view_atomic_summary, 6, "Atomic Summary", "View the atomic summary output"},
  {&switch_atomic_data, 7, "Switch Atomic Data", "Switch atomic data data sets"},
  {&menu_exit_atomix, MENU_QUIT, "Exit", "Exit Atomix"},
};

// const
MenuItem_t BOUND_MENU_CHOICES[] = {
  {&all_bound_bound, 0, "All", "Print all of the possible transitions"},
  {&bound_bound_wavelength_range, 1, "By wavelength range", "Print the transitions over a given wavelength range"},
  {&bound_bound_element, 2, "By element", "Print all the transitions for a given element"},
  {&bound_bound_ion, 3, "By ion number", "Print all the transitions for a given ion"},
  {NULL, MENU_QUIT, "Return to main menu", ""}
};

// const
MenuItem_t BOUND_FREE_MENU_CHOICES[] = {
  {&all_bound_free, 0, "All", "Print all of the possible transitions"},
  {&bound_free_wavelength_range, 1, "By wavelength range", "Print the transitions over a given wavelength range"},
  {&bound_free_element, 2, "By element", "Print all the transitions for a given element"},
  {&bound_free_ion, 3, "By ion number", "Print all the transitions for a given ion"},
  {NULL, MENU_QUIT, "Return to main menu", ""}
};

// const
MenuItem_t INNER_SHELL_MENU_CHOICES[] = {
  {&all_inner_shell, 0, "All", "Print all of the possible transitions"},
  {&inner_shell_wavelength_range, 1, "By wavelength range", "Print the transitions over a given wavelength range"},
  {&inner_shell_element, 2, "By element", "Print all the transitions for a given element"},
  {&inner_shell_ion, 3, "By ion number", "Print all the transitions for a given ion"},
  {NULL, MENU_QUIT, "Return to main menu", ""}
};

// const
MenuItem_t ELEMENTS_MENU_CHOICES[] = {
  {&all_elements, 0, "All elements", "Query all elements in the atomic data"},
  {&single_element, 1, "Single element", "Query a single element"},
  {NULL, MENU_QUIT, "Return to main menu", ""},
};

// const
MenuItem_t IONS_MENU_CHOICES[] = {
  {&all_ions, 0, "All", "Print all the ions in the atomic data"},
  {&ions_for_element, 1, "By element", "Print all the ions for an element"},
  {&single_ion_atomic_z, 2, "By atomic and ionisation state",
   "Detailed output for a single ion by atomic number and ionisation state"},
  {&single_ion_nion, 3, "By ion number", "Detailed output for a single ion by ion number"},
  {NULL, MENU_QUIT, "Return to main menu", ""},
};

// const
MenuItem_t LEVELS_MENU_CHOICES[] = {
  {&all_level_configurations, 0, "All", "Print all of the atomic configurations"},
  {NULL, 1, "By element", "Print all the atomic configurations for an element"},
  {NULL, 2, "By atomic number and ionisation state", ""},
  {NULL, 3, "By ion number", ""},
  {NULL, 4, "By level density", ""},
  {NULL, MENU_QUIT, "Return to main menu", ""}
};
