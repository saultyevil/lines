/** ************************************************************************* */
/**
 * @file     main.c
 * @author   Edward Parkinson
 * @date     September 2019
 *
 * @brief    The main functions related to running this program.
 *
 * ************************************************************************** */

#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "atomix.h"
#include "atomic.h"

/* ************************************************************************** */
/**
 *
 * ************************************************************************** */

char *MAIN_MENU_CHOICES[] = {
  "Bound-Bound Lines",
  "Photoionization Cross Sections",
  "Exit",
  NULL
};

enum MAIN_MENU_CHOICES_ENUM
{
  BOUND_BOUND      = 0,
  PHOTOIONIZATION  = 1,
  EXIT             = 2,
  NCHOICES         = 3
};

const int MAIN_MENU_NCHOICES = ARRAY_SIZE (MAIN_MENU_CHOICES);


/* ************************************************************************** */
/**
 *  @brief      Displays the home menu containing all of the main programs.
 *
 *  @param[in]  int current_index   The index referring to the previously chosen
 *                                  menu entry
 *  @return     int choice          An integer referring to the chosen menu item
 *
 *  @details
 *
 *  This function initialises a window and menu to display the options which are
 *  available in Py Wind. The user can use arrow keys and page up and page down
 *  to navigate the list and enter to select a choice. An integer referring to
 *  the internal index of the choice is returned. If the user press the q key,
 *  then the program is exited.
 *
 * ************************************************************************** */

int
main_menu (int current_index)
{
  int choice;

  choice = goto_menu ("What do you want to inspect?:", MAIN_MENU_CHOICES, MAIN_MENU_NCHOICES,
                      current_index);

  return choice;
}

/* ************************************************************************** */
/**
 *  @brief      Process the menu input choices.
 *
 *  @param[in]  choice  The index for the menu choice
 *
 *  @details
 *
 *
 * ************************************************************************** */

void
process_main_menu_choices (int choice)
{
  switch (choice)
  {
    case BOUND_BOUND:
      bound_bound_home_menu ();
      break;
    case PHOTOIONIZATION:
      photoionization_home_menu ();
      break;
    case EXIT:
      clean_ncurses_screen ();
      exit (0);
    default:
      break;
  }
}

/* ************************************************************************* */
/**
 * @brief   The main function of the program.
 *
 * @param[in]   argc    The number of arguments provided.
 * @param[in]   argv    The arguments provided.
 *
 * @return      EXIT_SUCCESS
 *
 * @details
 *
 * ************************************************************************** */

int
main (int argc, char *argv[])
{
  int the_choice;
  double wmin, wmax;
  char atomic_data_name[LINELEN];
  ScreenBuffer_t sb = SB_INIT;

  /*
   * Initialise the output file and read in the atomic data files
   */

  Log_init ("atomix.out.txt");
  parse_input (argc, argv, atomic_data_name, &wmin, &wmax);
  if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
    strcat (atomic_data_name, ".dat");
  get_atomic_data (atomic_data_name);

  init_ncurses_screen ();
  write_banner_stdscr ();

  /*
   * Handle the different queries
   */

  the_choice = 0;
  while (TRUE)
  {
    if ((the_choice = main_menu (the_choice)) == QUIT)
      break;
    process_main_menu_choices (the_choice);
  }

  clean_ncurses_screen ();

  return EXIT_SUCCESS;
}
