/* ************************************************************************** */
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
#include <signal.h>

#include "atomix.h"

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
main(int argc, char *argv[])
{
  /*
   * Start by checking everything is as it should be, and initialise the global
   * variables, logfile and check the command lines for input
   */

  if(getenv("PYTHON") == NULL)
    exit_atomix(EXIT_FAILURE, "main : unable to find the required $PYTHON environment variable");

  atexit(cleanup_ncurses_stdscr);

  AtomixConfiguration.current_screen = sc_unassigned;
  AtomixConfiguration.rows = AtomixConfiguration.cols = 0;
  AtomixConfiguration.current_line = AtomixConfiguration.current_col = 0;
  AtomixConfiguration.atomic_data_loaded = FALSE;
  AtomixConfiguration.atomic_data[0] = '\0';
  AtomixConfiguration.status_message[0] = '\0';

  DISPLAY_BUFFER.nlines = 0;
  DISPLAY_BUFFER.maxlen = 0;
  DISPLAY_BUFFER.lines = NULL;
  strcpy(DISPLAY_BUFFER.name, "display");

  ATOMIC_BUFFER.nlines = 0;
  ATOMIC_BUFFER.maxlen = 0;
  ATOMIC_BUFFER.lines = NULL;
  strcpy(ATOMIC_BUFFER.name, "atomic");

  logfile_init("atomix.log.txt");
  check_command_line(argc, argv);

  /*
   * Initialise ncurses, the window panels and draw the window borders
   */

  initialise_ncurses_stdscr();
  initialise_main_windows();
  draw_window_boundaries();

  /*
   * Query the user for the atomic data file name, and read in that atomic
   * data. If there is an error with reading the atomic data, then the error
   * is handle in this function. The main_menu (MENU_DRAW) is used here to draw
   * the main menu to complete the look of the UI
   */

  main_menu(MENU_DRAW);

  if(!AtomixConfiguration.atomic_data_loaded)
    switch_atomic_data();

  main_menu(MENU_CONTROL);

  return EXIT_SUCCESS;          // Don't need to clean up at exit due to atexit()
}
