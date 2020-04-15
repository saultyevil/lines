/** ************************************************************************* */
/**
 * @file     menu.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for creating and navigating menus. Requires the menu library which
 * is typically part of a standard ncurses install.
 *
 * ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <menu.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief      Clean up the memory for the menu.
 *
 * @param[in]  menu      The menu to be cleaned up
 * @param[in]  items     The items to be cleaned up
 * @param[in]  nitems    The number of items to clean up
 * @param[in]  win_menu  The window for the menu to be cleaned up
 *
 * @details
 *
 * Simply a wrapper function to clean up the menu elements.
 *
 * ************************************************************************** */

void
clean_up_menu (MENU *menu, ITEM **items, int nitems)
{
  int i;

  unpost_menu (menu);
  for (i = 0; i < nitems; ++i)
    free_item (items[i]);
  free_menu (menu);
}

/* ************************************************************************** */
/**
 * @brief      Control the cursor for the provided menu.
 *
 * @param[in]  menu  The menu to control
 * @param[in]  c     An integer which represents the keypress
 *
 * @return     The index for the item menu if one is selected.
 *
 * @details
 *
 * ************************************************************************** */

int
control_menu (MENU *menu, int c)
{
  int the_choice;

  the_choice = MENU_QUIT;

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
 * @brief      Displays a menu given the current items.
 *
 * @param[in]  menu_message       A message to display for the menu
 * @param[in]  menu_items         The name of the menu items
 * @param[in]  nitems             The number of items in the menu
 * @param[in]  current_index      The index referring to the previously chosen
 *                                menu entry
 * @param[in]  user_control_menu  If TRUE, allow the user to control the menu
 *
 * @return     choice             An integer referring to the chosen menu item
 *
 * @details
 *
 * ************************************************************************** */

int
update_menu_window (char *menu_message, char **menu_items, int nitems, int current_index, int user_control_menu)
{
  int i, j, c;
  int len;
  int the_choice;

  MENU *menu = NULL;
  ITEM **items = NULL;
  WINDOW *win = MENU_WINDOW.win;

  wclear (win);

  // This creates a 1 column boundary between the menu and content window
  wattron (win, A_REVERSE);
  for (j = 0; j < MENU_WINDOW.rows; ++j)
    mvwprintw (win, j, MENU_WINDOW.cols - 1, " ");
  wattroff (win, A_REVERSE);

  if (!(items = calloc (nitems, sizeof (ITEM *))))
  {
    cleanup_ncurses_stdscr();
    printf("BIG ERROR: Unable to allocate memory for menu\n");
    exit (EXIT_FAILURE);
  }

  /*
   * Pad the menu message to the center of the window
   */

  len = (int) strlen (menu_message);
  mvwprintw (win, 1, (MENU_WINDOW.cols - 2) / 2 - (len / 2), menu_message);

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

  set_menu_win (menu, win);
  set_menu_sub (menu, derwin (win, MENU_WINDOW.rows - 3, MENU_WINDOW.cols, 3, 0));
  set_menu_format (menu, MENU_WINDOW.rows - 2, 1);
  set_menu_mark (menu, "* ");

  if (current_index < 0)
    current_index = 0;
  if (current_index > nitems - 1)
    current_index = 0;

  set_current_item (menu, items[current_index]);
  post_menu (menu);
  wrefresh (win);

  the_choice = MENU_QUIT;
  if (user_control_menu)
  {
    while ((c = wgetch (win)) != 'q')
    {
      the_choice = control_menu (menu, c);
      wrefresh (win);
      if (the_choice != MENU_QUIT)
        break;
    }
  }

  if (the_choice == nitems - 2)  // Quit is the 2nd last element
  {
    clean_up_menu (menu, items, nitems);
    cleanup_ncurses_stdscr();
    exit (0);
  }

  clean_up_menu (menu, items, nitems);

  return the_choice;
}
