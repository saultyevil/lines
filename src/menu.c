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

#include "menu.h"

/* ************************************************************************** */
/**
 * @brief  Clean up the memory for the menu.
 *
 * @param[in]  menu      The menu to be cleaned up
 * @param[in]  items     The items to be cleaned up
 * @param[in]  nitems    The number of items to clean up
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
 * Relies heavily on menu_driver(). When enter is pressed on an option, that
 * item has a "userptr" associated with it, which is a pointer to a function.
 * Thus, when enter is pressed, a function will be called (as long as
 * usrptr isn't null).
 *
 * ************************************************************************** */

int
control_menu (MENU *menu, int c)
{
  int current_index = MENU_CONTINUE;
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
      current_index = item_index (item);
      item_usrptr = item_userptr (item);
      if (item_usrptr != NULL)
        item_usrptr ();
      pos_menu_cursor (menu);
      break;
    default:
      break;
  }

  return current_index;
}

/* ************************************************************************** */
/**
 * @brief  Displays and controls the main menu.
 *
 * @param[in]  menu_message       The menu display message at the top
 * @param[in]  menu_items         The items for the menu
 * @param[in]  nitems             The number of items
 * @param[in]  current_index      The default menu index when drawn
 * @param[in]  control_this_menu  If TRUE, allow the user to control the menu
 *                                otherwise the menu is only drawn
 *
 * @return     index             An integer referring to the chosen menu item
 *
 * @details
 *
 * This is the main menu for atomix. The function is very similar to
 * create_menu() - note that it is not a generic function. Whilst in the main
 * menu, it is possible to press "q" to exit - something you can't do in
 * create_menu().
 *
 * ************************************************************************** */

int
create_main_menu (char *menu_message, const MenuItem_t *menu_items, int nitems, int current_index,
                  int control_this_menu)
{
  int i, j, c;
  int len;
  int index = MENU_CONTINUE;
  MENU *menu = NULL;
  ITEM **items = NULL;

  wclear (MENU_WINDOW.win);

  items = calloc (nitems + 1, sizeof (ITEM *));
  if (items == NULL)
    exit_atomix (EXIT_FAILURE, "create_menu : unable to allocate memory for menu items");

  /*
   * Create a 1 column boundary between the menu and content windows
   */

  wattron (MENU_WINDOW.win, A_REVERSE);
  for (j = 0; j < MENU_WINDOW.rows; ++j)
    mvwprintw (MENU_WINDOW.win, j, MENU_WINDOW.cols - 1, " ");
  wattroff (MENU_WINDOW.win, A_REVERSE);

  len = (int) strlen (menu_message);
  mvwprintw (MENU_WINDOW.win, 1, (MENU_WINDOW.cols - len) / 2 - 1, menu_message);

  for (i = 0; i < nitems; i++)
  {
    items[i] = new_item (menu_items[i].name, menu_items[i].desc);
    set_item_userptr (items[i], menu_items[i].func);
  }

  items[nitems] = NULL;

  menu = new_menu (items);
  menu_opts_off (menu, O_SHOWDESC);  // Don't want to show desc in small menu window
  set_menu_win (menu, MENU_WINDOW.win);
  set_menu_sub (menu, derwin (MENU_WINDOW.win, MENU_WINDOW.rows - 3, MENU_WINDOW.cols, 3, 0));
  set_menu_format (menu, MENU_WINDOW.rows - 2, 1);
  set_menu_mark (menu, "* ");

  if (current_index < 0)
    current_index = 0;
  if (current_index > nitems - 1)
    current_index = nitems - 1;

  set_current_item (menu, items[current_index]);
  update_status_bar ("Press q or F1 to exit Atomix");
  post_menu (menu);
  wrefresh (MENU_WINDOW.win);

  if (control_this_menu == MENU_CONTROL)
  {
    while ((c = wgetch (MENU_WINDOW.win)))
    {
      if (c == 'q' || c == KEY_F(1))
      {
        index = MENU_QUIT;
        break;
      }

      index = control_menu (menu, c);
      
      if (index != MENU_CONTINUE)
        break;
    }
  }

  clean_up_menu (menu, items, nitems);

  return index;
}

/* ************************************************************************** */
/**
 * @brief  Displays an control a genric menu for the given Window_t window.
 *
 * @param[in]  win                The Window_t to contain the menu
 * @param[in]  menu_message       The menu display message at the top
 * @param[in]  menu_items         The items for the menu
 * @param[in]  nitems             The number of items
 * @param[in]  current_index      The default menu index when drawn
 * @param[in]  control_this_menu  If TRUE, allow the user to control the menu
 *                                otherwise the menu is only drawn
 *
 * @return     index             An integer referring to the chosen menu item
 *
 * @details
 *
 * This can be used to create a generic menu, with the same formatting as
 * other menus. This should create a consistent menu system.
 *
 * TODO: configuration struct in case finer control is required.
 *
 * ************************************************************************** */

int
create_menu (Window_t win, char *menu_message, const MenuItem_t *menu_items, int nitems, int current_index,
             int control_this_menu)
{
  int i, c;
  int index = MENU_CONTINUE;
  MENU *menu = NULL;
  ITEM **items = NULL;
  WINDOW *window = win.win;

  wclear (window);

  items = calloc (nitems + 1, sizeof (ITEM *));
  if (items == NULL)
    exit_atomix (EXIT_FAILURE, "create_menu : unable to allocate memory for menu items");

  for (i = 0; i < nitems; i++)
  {
    items[i] = new_item (menu_items[i].name, menu_items[i].desc);
    set_item_userptr (items[i], menu_items[i].func);
  }

  items[nitems] = NULL;

  menu = new_menu (items);
  menu_opts_on (menu, O_SHOWDESC);
  set_menu_spacing (menu, 3, 0, 0);
  set_menu_win (menu, window);
  set_menu_sub (menu, derwin (window, win.rows - 6, win.cols - 2, 3, 1));  // TODO better constants
  set_menu_format (menu, win.rows - 6, 1);
  set_menu_mark (menu, "* ");

  if (current_index < 0)
    current_index = 0;
  if (current_index > nitems - 1)
    current_index = nitems - 1;

  set_current_item (menu, items[current_index]);
  update_status_bar ("Press q or F1 to exit Atomix");
  bold_message (CONTENT_WINDOW, 1, 1, menu_message);
  post_menu (menu);
  wrefresh (window);

  if (control_this_menu)
  {
    while ((c = wgetch (window)))
    {
      if (c == 'q' || c == (KEY_F(1)))
      {
        index = MENU_QUIT;
        break;
      }

      index = control_menu (menu, c);
      wrefresh (window);

      if (index != MENU_CONTINUE)
        break;
    }
  }

  clean_up_menu (menu, items, nitems);

  return index;
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 *  ************************************************************************* */

void
main_menu (int control)
{
  static int menu_index = 0;

  if (control == MENU_DRAW)
  {
    menu_index = create_main_menu ("Main Menu", MAIN_MENU_CHOICES, ARRAY_SIZE (MAIN_MENU_CHOICES), menu_index,
                                   MENU_DRAW);
  }
  else
  {
    while (TRUE)
    {
      atomic_summary_show (SCROLL_DISBALE);
      menu_index = create_main_menu ("Main Menu", MAIN_MENU_CHOICES, ARRAY_SIZE (MAIN_MENU_CHOICES), menu_index,
                                     MENU_CONTROL);

      if (menu_index == MENU_QUIT || MAIN_MENU_CHOICES[menu_index].index == MENU_QUIT)  // Safety really
        break;
    }
  }
}

/* ************************************************************************** */
/**
 * @brief  The main menu for bound bound transition queries.
 *
 * @details
 *
 * The previous menu index is remembered.
 *
 * ************************************************************************** */

void
bound_bound_main_menu (void)
{
  static int menu_index = 0;

  if (nlines == 0)
  {
    error_atomix ("No bound-bound transitions were read in");
    return;
  }

  while (TRUE)
  {
    menu_index = create_menu (CONTENT_WINDOW, "Bound-bound transitions", BOUND_MENU_CHOICES,
                              ARRAY_SIZE (BOUND_MENU_CHOICES), menu_index, MENU_CONTROL);
    if (BOUND_MENU_CHOICES[menu_index].index == MENU_QUIT || menu_index == MENU_QUIT)
      return;
  }
}

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
 * @brief The main menu for element queries.
 *
 * @details
 *
 * This function is basically a wrapper around create_menu to allow a user to
 * choose the different querying options.
 *
 * ************************************************************************** */

void
elements_main_menu (void)
{
  static int menu_index = 0;

  if (ele == NULL)
  {
    error_atomix ("No elements have been read in. Unable to query!");
    return;
  }

  while (TRUE)
  {
    menu_index = create_menu (CONTENT_WINDOW, "Elements", ELEMENTS_MENU_CHOICES, ARRAY_SIZE (ELEMENTS_MENU_CHOICES),
                              menu_index, MENU_CONTROL);
    if (ELEMENTS_MENU_CHOICES[menu_index].index == MENU_QUIT || menu_index == MENU_QUIT)
      return;
  }
}

/* ************************************************************************** */
/**
 * @brief  The main menu for ion queries.
 *
 * @details
 *
 * The previous menu index is remembered.
 *
 * ************************************************************************** */

void
ions_main_menu (void)
{
  static int menu_index;

  if (nions == 0)
  {
    error_atomix ("No ions have been read in. Unable to query!");
    return;
  }

  while (TRUE)
  {
    menu_index = create_menu (CONTENT_WINDOW, "Ions", IONS_MENU_CHOICES, ARRAY_SIZE (IONS_MENU_CHOICES), menu_index,
                              MENU_CONTROL);
    if (IONS_MENU_CHOICES[menu_index].index == MENU_QUIT || menu_index == MENU_QUIT)
      return;
  }
}
