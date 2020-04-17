/** ************************************************************************* */
/**
 * @file     query.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for querying information from the user.
 *
 * TODO switch to using the forms library
 *
 * ************************************************************************** */

#include <form.h>
#include <string.h>
#include <stdlib.h>

#include "atomix.h"

const
MenuItem_t ATOMIC_DATA_CHOICES[] ={
  {NULL, 0        , "CIIICIVCV_c10"             , ": Carbon III, IV and V Macro-atom"},
  {NULL, 0        , "CIIICIVCV_c10_CV1LVL"      , ": Carbon III, IV and V Macro-atom"},
  {NULL, 0        , "CIIICIV_c10"               , ": Carbon III and IV Macro-atom"},
  {NULL, 0        , "h10_hetop_lohe1_standard80", ": 10 Level H and He Macro-atom"},
  {NULL, 0        , "h10_hetop_standard80"      , ": 10 Level H and He Macro-atom"},
  {NULL, 0        , "h10_standard80"            , ": 10 Level H Macro-atom"},
  {NULL, 0        , "h20"                       , ": 20 Level H Macro-atom"},
  {NULL, 0        , "h20_hetop_standard80"      , ": 20 Level H and He Macro-atoms"},
  {NULL, 0        , "standard80"                , ": Standard Simple-atom"},
  {NULL, 0        , "standard80_reduced"        , ": Reduced Simple-atom"},
  {NULL, 0        , "standard80_sn_kurucz"      , ": Standard Supernova Simple-atom"},
  {NULL, 0        , "standard80_test"           , ": Standard TEst Simple-atom"},
  {NULL, 0        , "Other"                     , ": Custom data"},
  {NULL, MENU_NULL, NULL                        , NULL}
};

#define ATOMIC_DATA_WIDTH 26

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 *
 * ************************************************************************** */

//static char *
//trim_whitespaces(char *str)
//{
//  char *end;
//
//  while (isspace (*str))
//    str++;
//
//  if (*str == 0)
//    return str;
//
//  end = str + strnlen(str, 128) - 1;
//
//  while (end > str && isspace (*end))
//    end--;
//
//  *(end + 1) = '\0';
//
//  return str;
//}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 *
 * ************************************************************************** */

//void
//control_form (WINDOW *win, FORM *form)
//{
//  int ch;
//
//  while ((ch = wgetch (win)) != 'q' || ch == KEY_ENTER)
//  {
//    switch (ch)
//    {
//      case KEY_DOWN:form_driver (form, REQ_NEXT_FIELD);
//        form_driver (form, REQ_END_LINE);
//        break;
//      case KEY_UP:form_driver (form, REQ_PREV_FIELD);
//        form_driver (form, REQ_END_LINE);
//        break;
//      case KEY_LEFT:form_driver (form, REQ_PREV_CHAR);
//        break;
//      case KEY_RIGHT:form_driver (form, REQ_NEXT_CHAR);
//        break;
//      case KEY_BACKSPACE:
//      case 127:form_driver (form, REQ_DEL_PREV);
//        break;
//      case KEY_DC:form_driver (form, REQ_DEL_CHAR);
//        break;
//      default:form_driver (form, ch);
//        break;
//    }
//
//    wrefresh (win);
//  }
//}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

//void
//query_custom_atomic_data (void)
//{
//  FORM *form;
//  FIELD *fields[2];
//  WINDOW *win = CONTENT_WINDOW.win;
//  char *atomic_data_name;
//
//  fields[1] = NULL;
//  fields[0] = new_field (1, ATOMIC_DATA_WIDTH, 0, 0, 0, 0);
//
//  set_field_back (fields[0], A_UNDERLINE);
//  field_opts_off (fields[0], O_AUTOSKIP);
//
//  form = new_form (fields);
//  set_form_win (form, win);
//  set_form_sub (form, derwin (win, 1, ATOMIC_DATA_WIDTH, 3, 4));
//  post_form (form);
//  set_current_field (form, fields[0]);
//
//  wrefresh (win);
//  control_form (win, form);
//  atomic_data_name = trim_whitespaces (field_buffer (fields[0], 0));
//}

/* ************************************************************************** */
/**
 * @brief     Query the user for the atomic data name.
 *
 * @details
 *
 * Will keep asking the user for an atomic data name until the data can be read
 * in without error, or until the user quits the program.
 *
 * ************************************************************************** */

void
query_atomic_data (void)
{
  int valid = FALSE;
  int atomic_data_error;
  static int index = 8;
  char atomic_data_name[LINELEN];
  WINDOW *win = CONTENT_WINDOW.win;

  while (valid != TRUE)
  {

    index = create_menu (CONTENT_WINDOW, "Please select the atomic data to use", ATOMIC_DATA_CHOICES,
                         ARRAY_SIZE (ATOMIC_DATA_CHOICES), index, CONTROL_MENU);

    if (index > MENU_QUIT)
      strcpy (atomic_data_name, ATOMIC_DATA_CHOICES[index].name);

    if (strcmp (&atomic_data_name[strlen (atomic_data_name) - 4], ".dat") != 0)
      strcat (atomic_data_name, ".dat");

    atomic_data_error = get_atomic_data (atomic_data_name);

    if (atomic_data_error)
    {
      // TODO: increase verbosity of error messages, i.e. write out actual error
      mvwprintw (win, ARRAY_SIZE (ATOMIC_DATA_CHOICES), 2, "Invalid atomic data provided, try again.");
      mvwprintw (win, ARRAY_SIZE (ATOMIC_DATA_CHOICES) + 1, 2, "Error number: %i\n", atomic_data_error);
    }
    else
    {
      valid = TRUE;
    }

    wrefresh (win);
  }

  display_text_buffer (win, 1, 1);
}

/* ************************************************************************** */
/**
 * @brief      Query a wavelength value from the user.
 *
 * @param[in]  win  The window to place the query in.
 * @param[in]  msg  A message to prompt the user what to input.
 * @param[in]  y    The y location of the prompt message.
 * @param[in]  x    The x location of the prompt message.
 *
 * @return     The wavelength input by the user.
 *
 * @details
 *
 * Originally a function to avoid code duplication. The input value should be
 * echo'd back to the user, but once the value will be updated on the screen.
 *
 * ************************************************************************** */

double
get_wavelength (WINDOW *win, char *msg, int y, int x, int len)
{
  double wl;

  mvwprintw (win, y, x, "%s", msg);
  wrefresh (win);
  wscanw (win, "%lf", &wl);
  mvwprintw (win, y, len + 2, "%5.1f Angstroms", wl);
  wrefresh (win);

  return wl;
}

/* ************************************************************************** */
/**
 * @brief       Get the wavelength range to consider.
 *
 * @param[out]  wmin   The smallest wavelength transition to find.
 * @param[out]  wmax   The largest wavelength transition to find.
 *
 * @details
 *
 * This function will keep looping until either the user quits, or until the
 * input is correct; i.e., wmin < wmax. Echo mode is temporarily turned on for
 * this function, so the user can see what they are typing.
 *
 * ************************************************************************** */

void
query_wavelength_range (double *wmin, double *wmax)
{
  int valid = FALSE;
  WINDOW *win = CONTENT_WINDOW.win;

  wclear (win);
  echo ();

  bold_message (win, 1, 2, "Please input the wavelength range to query:");

  while (valid != TRUE)
  {
    *wmin = get_wavelength (win, "Minimum wavelength range: > ", 3, 2, 28);
    *wmax = get_wavelength (win, "Maximum wavelength range: > ", 5, 2, 28);

    if (*wmax > *wmin)
    {
      valid = TRUE;
    }
    else
    {
      bold_message (win, 7, 2, "Incorrect input. Try again...");
      wrefresh (win);
      wmove (win, 3, 0);
      wclrtoeol (win);
      wmove (win, 5, 0);
      wclrtoeol (win);
    }
  }

  noecho ();
}
