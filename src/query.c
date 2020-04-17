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
 * ************************************************************************** */

#include <form.h>
#include <string.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 *
 * ************************************************************************** */

int
control_form (FORM *form, int ch)
{
  int input = MENU_NULL;

  switch (ch)
  {
    case KEY_DOWN:
      form_driver (form, REQ_NEXT_FIELD);
      form_driver (form, REQ_END_LINE);
      break;
    case KEY_UP:
      form_driver (form, REQ_PREV_FIELD);
      form_driver (form, REQ_END_LINE);
      break;
    case KEY_LEFT:
      form_driver (form, REQ_PREV_CHAR);
      break;
    case KEY_RIGHT:
      form_driver (form, REQ_NEXT_CHAR);
      break;
    case KEY_BACKSPACE:
    case 127:
      form_driver (form, REQ_DEL_PREV);
      break;
    case KEY_DC:
      form_driver (form, REQ_DEL_CHAR);
      break;
    case 10:
      input = MENU_QUIT;
      break;
    default:
      form_driver (form, ch);
      break;
  }

  return input;
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
query_user_for_input (Window_t win, char *title_message, char *question, char *answer)
{
  int ch, control = MENU_NULL;
  static FORM *form;
  static FIELD *fields[3];
  WINDOW *the_win = win.win;

  wclear (the_win);
  keypad (the_win, TRUE);

  bold_message (the_win, 1, 1, title_message);

  fields[0] = new_field (1, strlen (question), 0, 0, 0, 0);
  fields[1] = new_field (1, MAX_FIELD_INPUT, 0, strlen (question) + 2, 0, 0);
  fields[2] = NULL;

  set_field_buffer (fields[0], 0, question);
  set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);

  set_field_buffer (fields[1], 0, answer);
  set_field_back (fields[1], A_REVERSE);
  field_opts_off (fields[1], O_AUTOSKIP);
  set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

  form = new_form (fields);
  set_form_win (form, the_win);
  set_form_sub (form, derwin (the_win, 10, win.cols - 2, 3, 1));
  set_current_field (form, fields[1]);
  post_form (form);
  wrefresh (the_win);

  /*
   * Control the menu, exits when the enter key has been pressed
   * or when ch is (somehow) NULL
   */

  while ((ch = wgetch (the_win)) && control == MENU_NULL)
  {
    control = control_form (form, ch);
    wrefresh (the_win);
    if (control == MENU_QUIT)
      break;
  }

  form_driver (form, REQ_VALIDATION);
  strcpy (answer, trim_whitespaces (field_buffer (fields[1], 0)));

  unpost_form (form);
  free_form (form);
  free_field (fields[0]);
  free_field (fields[1]);
  free_field (fields[2]);

  keypad (the_win, FALSE);
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
  {NULL, MENU_OTHR, "Other"                     , ": Custom data"},
  {NULL, MENU_NULL, NULL                        , NULL}
};

void
query_atomic_data (void)
{
  int valid = FALSE;
  int atomic_data_error;
  static int index = 8;
  static int init_name = FALSE;
  char atomic_data_name[MAX_FIELD_INPUT];
  WINDOW *win = CONTENT_WINDOW.win;

  if (init_name == FALSE)
  {
    strcpy (atomic_data_name, "");
    init_name = TRUE;
  }

  while (valid != TRUE)
  {
    index = create_menu (CONTENT_WINDOW, "Please select the atomic data to use", ATOMIC_DATA_CHOICES,
                         ARRAY_SIZE (ATOMIC_DATA_CHOICES), index, CONTROL_MENU);

    if (index > MENU_QUIT)
    {
      if (ATOMIC_DATA_CHOICES[index].index != MENU_OTHR)
      {
        strcpy (atomic_data_name, ATOMIC_DATA_CHOICES[index].name);
        strcat (atomic_data_name, ".dat");
      }
      else
      {
        query_user_for_input (CONTENT_WINDOW, "Please input the path to the atomic data masterfile",
                              "File path: ", atomic_data_name);
      }
    }

    atomic_data_error = get_atomic_data (atomic_data_name);

    if (atomic_data_error)
    {
      update_status_bar ("Error: error when reading atomic data : errno = %i", atomic_data_error);
    }
    else
    {
      valid = TRUE;
    }

    wrefresh (win);
  }

  display_text_buffer (win, 1, 1);
  log_flush ();
}
