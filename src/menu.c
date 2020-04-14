/** ************************************************************************* */
/**
 * @file
 * @author   Edward Parkinson
 * @date
 *
 * @brief
 *
 * ************************************************************************** */

#include <stdlib.h>
#include <curses.h>
#include <menu.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 *  @brief      Clean up the memory for the menu.
 *
 *  @param[in]  menu      The menu to be cleaned up
 *  @param[in]  items     The items to be cleaned up
 *  @param[in]  nitems    The number of items to clean up
 *  @param[in]  win_menu  The window for the menu to be cleaned up
 *
 *  @details
 *
 *  Simply a wrapper function to clean up the menu elements.
 *
 * ************************************************************************** */

void
clean_up_menu (MENU *menu, ITEM **items, int nitems, WINDOW *win_menu)
{
  int i;

  unpost_menu (menu);
  for (i = 0; i < nitems; ++i)
    free_item (items[i]);
  free_menu (menu);

  wclear (win_menu);
  delwin (win_menu);
  wrefresh (win_menu);
}

/* ************************************************************************** */
/**
 *  @brief      Control the cursor for the provided menu.
 *
 *  @param[in]  menu  The menu to control
 *  @param[in]  c     An integer which represents the keypress
 *
 *  @return     The index for the item menu
 *
 *  @details
 *
 * ************************************************************************** */

int
control_menu (MENU *menu, int c)
{
  int the_choice;

  the_choice = QUIT;

  switch (c)
  {
    case KEY_DOWN:
      menu_driver (menu, REQ_DOWN_ITEM);
      break;
    case KEY_UP:
      menu_driver (menu, REQ_UP_ITEM);
      break;
    case KEY_LEFT:
      menu_driver (menu, REQ_LEFT_ITEM);
      break;
    case KEY_RIGHT:
      menu_driver (menu, REQ_RIGHT_ITEM);
      break;
    case KEY_NPAGE:
      menu_driver (menu, REQ_SCR_DPAGE);
      break;
    case KEY_PPAGE:
      menu_driver (menu, REQ_SCR_UPAGE);
      break;
    case 10:  // Enter has been pressed - not sure what the constant for this is
      the_choice =  item_index (current_item (menu));
      break;
    default:
      break;
  }

  return the_choice;
}

/* ************************************************************************** */
/**
 *  @brief      Displays a menu given the current items.
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
create_menu (char *menu_message, char **menu_items, int nitems, int current_index)
{
  int i, c;
  int the_choice;

  MENU *menu = NULL;
  ITEM **items = NULL;
  WINDOW *win_menu = NULL;

  create_sub_window (&win_menu);

  if (!(items = calloc (nitems, sizeof (ITEM *))))
  {
    clean_ncurses_screen ();
    printf("BIG ERROR: Unable to allocate memory for menu\n");
    exit (EXIT_FAILURE);
  }

  wattron (win_menu, A_BOLD);
  mvwprintw (win_menu, 0, 0, "%s", menu_message);
  wattroff (win_menu, A_BOLD);

  /*
   * Populate the menu with items from MENU_CHOICES, set the description as the
   * name of the label because we are going to hide descriptions anyway (for now)
   */

  for (i = 0; i < nitems; i++)
    items[i] = new_item (menu_items[i], menu_items[i]);
  menu = new_menu (items);
  menu_opts_off (menu, O_SHOWDESC);

  /*
   * Set the menu formatting and place the cursor to the last chosen menu choice
   * using set_current_item
   */

  set_menu_win (menu, win_menu);
  set_menu_sub (menu, derwin (win_menu, MAX_ROWS - 2, MAX_COLS - 2, 2, 0));
  set_menu_format (menu, MAX_ROWS - 2, 2);
  set_menu_mark (menu, "* ");

  if (current_index < 0)
    current_index = 0;
  if (current_index > nitems - 1)
    current_index = 0;

  set_current_item (menu, items[current_index]);

  post_menu (menu);
  wrefresh (win_menu);

  the_choice = QUIT;
  while ((c = wgetch (win_menu)) != 'q')
  {
    the_choice = control_menu (menu, c);
    wrefresh (win_menu);
    if (the_choice != QUIT)
      break;
  }

  if (the_choice == nitems - 2)  // Quit is the 2nd last element
  {
    clean_up_menu (menu, items, nitems, win_menu);
    clean_ncurses_screen ();
    exit (0);
  }

  clean_up_menu (menu, items, nitems, win_menu);

  return the_choice;
}
