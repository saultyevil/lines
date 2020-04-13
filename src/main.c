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

  init_ncurses_screen ();
  write_banner ();

  /*
   * Initialise the output file and read in the atomic data files
   */

  Log_init ("atomix.out.txt");
  parse_input (argc, argv, atomic_data_name, &wmin, &wmax);
  if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
    strcat (atomic_data_name, ".dat");
  get_atomic_data (atomic_data_name);

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
