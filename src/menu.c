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
 * @brief  Clean up the memory for the menu.
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
 * @brief  Control the cursor for the provided menu.
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
  int current_item_index = MENU_NULL;
  void (*item_usrptr) (void);
  ITEM *item;

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
      item = current_item (menu);
      current_item_index = item_index (item);
      item_usrptr = item_userptr (item);
      if (item_usrptr)
        item_usrptr ();
      pos_menu_cursor (menu);  // TODO: not sure if required
      break;
    default:
      break;
  }

  return current_item_index;
}

/* ************************************************************************** */
/**
 * @brief  Displays and controls the main menu.
 *
 * @param[in]  menu_message       A message to display for the menu
 * @param[in]  menu_items         The name of the menu items
 * @param[in]  nitems             The number of items in the menu
 * @param[in]  current_index      The index referring to the previously chosen
 *                                menu entry
 * @param[in]  control_menu  If TRUE, allow the user to control the menu
 *                                otherwise just the menu is printed
 *
 * @return     choice             An integer referring to the chosen menu item
 *
 * @details
 *
 * This function assumes that the last element of the menu is QUIT and that the
 * last element in menu_items is NULL. Hence, the index for QUIT is nitems - 2.  
 * 
 * ************************************************************************** */

int
main_menu (char *menu_message, const MenuItem_t *menu_items, int nitems, int current_index, int control_this_menu)
{
  int i, j, c;
  int len;
  int index = MENU_NULL;
  MENU *menu;
  ITEM **items;

  wclear (MENU_WINDOW.win);

  if (menu_items[nitems - 1].name != NULL)
  {
    cleanup_ncurses_stdscr ();
    printf ("Error : main_menu : programming error - final element of menu_items is not NULL.\n");
    printf ("%s\n", menu_items[nitems].name);
    exit (EXIT_FAILURE);
  }

  /*
   * This creates a 1 column "boundary" between the menu and content window
   */

  wattron (MENU_WINDOW.win, A_REVERSE);
  for (j = 0; j < MENU_WINDOW.rows; ++j)
    mvwprintw (MENU_WINDOW.win, j, MENU_WINDOW.cols - 1, " ");
  wattroff (MENU_WINDOW.win, A_REVERSE);

  len = (int) strlen (menu_message);
  mvwprintw (MENU_WINDOW.win, 1, (MENU_WINDOW.cols - len) / 2 - 1, menu_message);

  if (!(items = calloc (nitems, sizeof (ITEM *))))
  {
    cleanup_ncurses_stdscr();
    printf("Error: unable to allocate memory to construct a menu\n");
    exit (EXIT_FAILURE);
  }

  for (i = 0; i < nitems; i++)
  {
    items[i] = new_item (menu_items[i].name, menu_items[i].desc);
    set_item_userptr (items[i], menu_items[i].usrptr);
  }

  menu = new_menu (items);
  menu_opts_off (menu, O_SHOWDESC);  // Don't want to show desc in small menu window

  /*
   * Set the menu formatting. Note that this is hard coded at the moment, though
   * I do not think it will ever need to be done more dynamically.
   */

  set_menu_win (menu, MENU_WINDOW.win);
  set_menu_sub (menu, derwin (MENU_WINDOW.win, MENU_WINDOW.rows - 3, MENU_WINDOW.cols, 3, 0));
  set_menu_format (menu, MENU_WINDOW.rows - 2, 1);
  set_menu_mark (menu, "* ");

  if (current_index < 0)
    current_index = 0;
  if (current_index > nitems - 1)
    current_index = nitems - 1;

  set_current_item (menu, items[current_index]);
  post_menu (menu);
  wrefresh (MENU_WINDOW.win);

  if (control_this_menu)
  {
    while ((c = wgetch (MENU_WINDOW.win)))
    {
      if (c == 'q')
      {
        index = MENU_QUIT;
        break;
      }

      index = control_menu (menu, c);
      if (index != MENU_NULL)
        break;

      wrefresh (MENU_WINDOW.win);
    }
  }

  clean_up_menu (menu, items, nitems);

  return index;
}

/* ************************************************************************** */
/**
 * @brief  Displays a menu given the current items.
 *
 * @param[in]  menu_message       A message to display for the menu
 * @param[in]  menu_items         The name of the menu items
 * @param[in]  nitems             The number of items in the menu
 * @param[in]  current_index      The index referring to the previously chosen
 *                                menu entry
 * @param[in]  control_menu  If TRUE, allow the user to control the menu
 *                                otherwise just the menu is printed
 *
 * @return     choice             An integer referring to the chosen menu item
 *
 * @details
 *
 * This function assumes that the last element of the menu is QUIT and that the
 * last element in menu_items is NULL. Hence, the index for QUIT is nitems - 2.
 *
 * ************************************************************************** */

int
create_menu (Window_t win, char *menu_message, const MenuItem_t *menu_items, int nitems, int current_index,
             int control_this_menu)
{
  int i, c;
  int index = MENU_NULL;
  MENU *menu;
  ITEM **items;
  WINDOW *the_win = win.win;

  wclear (the_win);
  keypad (the_win, TRUE);

  if (menu_items[nitems - 1].name != NULL)
  {
    cleanup_ncurses_stdscr ();
    printf ("Error: create_menu : programming error - final element of menu_items is not NULL.\n");
    printf ("%s\n", menu_items[nitems].name);
    exit (EXIT_FAILURE);
  }

  if (!(items = calloc (nitems, sizeof (ITEM *))))
  {
    cleanup_ncurses_stdscr();
    printf("Error: unable to allocate memory to construct a menu\n");
    exit (EXIT_FAILURE);
  }

  for (i = 0; i < nitems; i++)
  {
    items[i] = new_item (menu_items[i].name, menu_items[i].desc);
    set_item_userptr (items[i], menu_items[i].usrptr);
  }

  menu = new_menu (items);
  menu_opts_on (menu, O_SHOWDESC);
  set_menu_spacing (menu, 3, 0, 0);

  /*
   * Set the menu formatting. Note that this is hard coded at the moment, though
   * I do not think it will ever need to be done more dynamically.
   */

  set_menu_win (menu, the_win);
  set_menu_sub (menu, derwin (the_win, win.rows - 4, win.cols - 2, 3, 1));
  set_menu_format (menu, win.rows - 2, 1);
  set_menu_mark (menu, "* ");

  if (current_index < 0)
    current_index = 0;
  if (current_index > nitems - 1)
    current_index = nitems - 1;

  set_current_item (menu, items[current_index]);
  post_menu (menu);
  bold_message (the_win, 1, 1, menu_message);

  wrefresh (the_win);

  if (control_this_menu)
  {
    while ((c = wgetch (the_win)))
    {
      index = control_menu (menu, c);
      if (index != MENU_NULL)
        break;
      wrefresh (the_win);
    }
  }

  clean_up_menu (menu, items, nitems);
  keypad (the_win, FALSE);

  return index;
}
