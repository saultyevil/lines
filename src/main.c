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

#include "atomix.h"

/*
 * struct MenuItems_t
 * {
 *    char name[LINELEN];   // Name of menu item
 *    char desc[LINELEN];   // Description of menu item
 *    int menu_code;        // Internal menu code for this item, i.e. index
 *    void (*func) (void);  // ??? function pointer to menu item
 * }
 * 
 * Maybe a struct like this would work better? It would be neater..
 * 
 */

char *MAIN_MENU_CHOICES[] = {
  "Bound Lines",
  "Photoionization",
  "Atomic Data",
  "Exit",
  NULL
};

enum MAIN_MENU_CHOICES_ENUM
{
  BOUND_BOUND,
  PHOTOIONIZATION,
  SWITCH_ATOMIC_DATA,
  EXIT
};

/* ************************************************************************** */
/**
 * @brief  Displays the home menu and navigates.
 *
 * @param[in]  current_index  The index referring to the previously chosen
 *                            menu entry
 * @return     choice         An integer referring to the chosen menu item
 *
 * @details
 *
 * This function is the main menu in atomix. It essentially, as well as
 * process_main_menu_choices(), controls where the user will go next.
 *
 * ************************************************************************** */

int
main_menu (int current_index)
{
  int choice;

  choice = update_menu_window ("Main Menu", MAIN_MENU_CHOICES, ARRAY_SIZE (MAIN_MENU_CHOICES), current_index, TRUE);

  return choice;
}

/* ************************************************************************** */
/**
 * @brief  Process the menu input choices.
 *
 * @param[in]  choice  The index for the menu choice
 *
 * @details
 *
 * This function is used to take the user to other sections of atomix.
 *
 * ************************************************************************** */

void
process_main_menu_choices (int choice)
{
  switch (choice)
  {
    case BOUND_BOUND:
      bound_bound_main_menu ();
      break;
    case PHOTOIONIZATION:
      photoionization_home_menu ();
      break;
    case SWITCH_ATOMIC_DATA:
      query_atomic_data ();
      break;
    case EXIT:cleanup_ncurses_stdscr ();
      exit (0);
    default:
      break;
  }
}

/* ************************************************************************* */
/**
 * @brief  The main function of the program.
 *
 * @param[in] argc  The number of arguments provided.
 * @param[in] argv  The arguments provided.
 *
 * @return    EXIT_SUCCESS
 *
 * @details
 * 
 * The way atomic data is read in and the diagnostics printed at the beginning 
 * of the program is a bit spaghetti-ish.
 * 
 * ************************************************************************** */

int
main (int argc, char *argv[])
{
  int atomic_provided;
  int main_menu_choice = MENU_QUIT;

  /*
   * Initalise DISPLAY to zero lines and NULL otherwise realloc will crash
   */

  DISPLAY.nlines = 0;
  DISPLAY.lines = NULL;

  /*
   * Initialise the log file, this should put AT LEAST the atomic data
   * diagnostics into the logfile
   * TODO: add more to the log file
   */

  Log_init ("atomix.out.txt");
  atomic_provided = check_command_line (argc, argv);

  /*
   * Initialise ncurses, the window panels and draw the window borders
   */

  initialise_ncurses_stdscr ();
  initialise_main_windows ();
  draw_window_boundaries ();

  /*
   * Query the user for the atomic data file name, and read in that atomic
   * data. If there is an error with reading the atomic data, then the error
   * is handle in this function. The update_menu_window() call here is used
   * to just display the menu.
   */

  update_menu_window ("Main Menu", MAIN_MENU_CHOICES, ARRAY_SIZE (MAIN_MENU_CHOICES), 0, FALSE);

  if (atomic_provided)
  {
    display_text_buffer (CONTENT_WINDOW.win, 1, 1);
  }
  else
  {
    query_atomic_data ();
  }

  /*
   * Loops over the main menu until it's time to quit :^)
   */

  while (TRUE)
  {
    main_menu_choice = main_menu (main_menu_choice);
    if (main_menu_choice == MENU_QUIT)
      break;
    process_main_menu_choices (main_menu_choice);
  }

  cleanup_ncurses_stdscr ();

  return EXIT_SUCCESS;
}
