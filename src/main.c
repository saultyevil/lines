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

const
MenuItem_t MAIN_MENU_CHOICES[] = {
  {&bound_bound_main_menu    , 0        , "Bound Lines"    , "Query bound-bound transitions"},
  {&photoionization_main_menu, 1        , "Photoionization", "Query photionization edges"},
  {&query_atomic_data        , 2        , "Atomic Data"    , "Switch the atomic data"},
  {&menu_exit_atomix         , MENU_QUIT, "Exit"           , "Exit atomix"},
  {NULL                      , MENU_NULL, NULL             , NULL}
};

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
  int menu_index = 0;
  int print_atomic;

  atexit (cleanup_ncurses_stdscr);

  /*
   * Initialise the log file, this should put AT LEAST the atomic data
   * diagnostics into the logfile
   */

  log_init ("atomix.log.txt");
  print_atomic = check_command_line (argc, argv);

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

  main_menu ("Main Menu", MAIN_MENU_CHOICES, ARRAY_SIZE (MAIN_MENU_CHOICES), menu_index, REDRAW_MENU);

  if (print_atomic)
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
    menu_index = main_menu ("Main Menu", MAIN_MENU_CHOICES, ARRAY_SIZE (MAIN_MENU_CHOICES), menu_index, CONTROL_MENU);
    if (menu_index == MENU_QUIT || MAIN_MENU_CHOICES[menu_index].index == MENU_QUIT)  // Safety really
      break;
  }

  return EXIT_SUCCESS;  // Don't need to clean up at exit due to atexit()
}
