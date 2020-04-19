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
  {NULL, 0        , "standard80_test"           , ": Standard Test Simple-atom"},
  {NULL, INDX_OTHR, "Other"                     , ": Custom data, needs to be in $PYTHON/xdata"},
  {NULL, MENU_NULL, NULL                        , NULL}
};

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 *
 * ************************************************************************** */

void
clean_up_form (FORM *form, FIELD **fields, int nfields)
{
  int i;

  unpost_form (form);
  free_form (form);
  for (i = 0; i < nfields; ++i)
    free_field (fields[i]);
  free (fields);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * TODO: input validation, i.e. make sure both fields are filled in
 *
 * ************************************************************************** */

int
control_form (FORM *form, int ch)
{
  int input = FORM_CONTINUE;

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
      input = FORM_BREAK;
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

int
query_user (Window_t w, Query_t *q, int nfields, char *title_message, int default_field)
{
  int i;
  int ch;
  int form_return = FORM_CONTINUE;
  FORM *form;
  WINDOW *the_win = w.win;
  FIELD **fields;

  wclear (the_win);
  keypad (the_win, TRUE); // TODO: check if I need this

  bold_message (the_win, 1, 1, title_message);

  if (q[nfields].field != NULL)
    error_exit_atomix (EXIT_FAILURE, "query_user_for_input : final query is not NULL. Programming error!");
  if (default_field > nfields)
    error_exit_atomix (EXIT_FAILURE,
                       "query_user_for_input : the default field is larger than the number of fields. Programming error!");

  /*
   * Allocate memory for the fields array, which will be  pointers to the fields
   * in the Query_t structure
   */

  fields = calloc (nfields + 1, sizeof (FIELD *));
  if (fields == NULL)
    error_exit_atomix (EXIT_FAILURE, "query_user_for_input : unable to allocate memory for fields");

  /*
   * Initialise the fields with their respective values
   */

  fields[nfields] = NULL;

  for (i = 0; i < nfields; ++i)
  {
    fields[i] = q[i].field;
    set_field_buffer (q[i].field, q[i].buffer_number, q[i].buffer);
    if (q[i].background != FIELD_SKIP)  // TODO: find better SKIP constant to avoid warning
      set_field_back (q[i].field, q[i].background);
    if (q[i].opts_on != FIELD_SKIP)
      set_field_opts (q[i].field, q[i].opts_on);
    if (q[i].opts_off != FIELD_SKIP)
      field_opts_off (q[i].field, q[i].opts_off);
  }

  form = new_form (fields);
  set_form_win (form, the_win);
  set_form_sub (form, derwin (the_win, w.rows - 4, w.cols - 2, 3, 1));
  set_current_field (form, fields[default_field]);
  update_status_bar ("Press F1 to cancel input");
  post_form (form);
  wrefresh (the_win);

  /*
   * Control the menu. Should exit when the enter key is pressed, or when
   * ch is (somehow) NULL.
   */

  while ((ch = wgetch (the_win)) && form_return == FORM_CONTINUE)
  {
    if (ch == KEY_F(1))
    {
      form_return = FORM_QUIT;
      break;
    }

    form_return = control_form (form, ch);
    wrefresh (the_win);

    if (form_return == FORM_BREAK)
      break;
  }

  /*
   * We have to validate the current field to "save" the buffer - or something
   */

  form_driver (form, REQ_VALIDATION);

  /*
   * This copies the field buffer into the Query_t buffer
   */

  for (i = 0; i < nfields; ++i)
    strcpy (q[i].buffer, trim_whitespaces (field_buffer (fields[i], q[i].buffer_number)));

  clean_up_form (form, fields, nfields);

  return form_return;
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
init_wavelength_form (Query_t *q, char *default_wmin, char *default_wmax)
{
  q[0].buffer_number = 0;
  strcpy (q[0].buffer, "Minimum Wavelength :");
  q[0].field = new_field (1, strlen (q[0].buffer), 0, 0, 0, 0);
  q[0].opts_off = FIELD_SKIP;
  q[0].opts_on = O_VISIBLE | O_PUBLIC | O_AUTOSKIP;
  q[0].background = FIELD_SKIP;

  q[1].buffer_number = 0;
  strcpy (q[1].buffer, default_wmin);
  q[1].field = new_field (1, MAX_FIELD_INPUT, 0, strlen (q[0].buffer) + 2, 0, 0);
  q[1].opts_off = O_AUTOSKIP;
  q[1].opts_on = O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE;
  q[1].background = A_REVERSE;

  q[2].buffer_number = 0;
  strcpy (q[2].buffer, "Maximum Wavelength :");
  q[2].field = new_field (1, strlen (q[2].buffer), 2, 0, 0, 0);
  q[2].opts_off = FIELD_SKIP;
  q[2].opts_on = O_VISIBLE | O_PUBLIC | O_AUTOSKIP;
  q[2].background = FIELD_SKIP;

  q[3].buffer_number = 0;
  strcpy (q[3].buffer, default_wmax);
  q[3].field = new_field (1, MAX_FIELD_INPUT, 2, strlen (q[2].buffer) + 2, 0, 0);
  q[3].opts_off = O_AUTOSKIP;
  q[3].opts_on = O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE;
  q[3].background = A_REVERSE;

  q[4].field = NULL;
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

int
query_wavelength_range (double *wmin, double *wmax)
{
  int valid = FALSE;
  int form_return;
  static int init_values = FALSE;
  static char s_wmin[MAX_FIELD_INPUT];
  static char s_wmax[MAX_FIELD_INPUT];
  WINDOW *win = CONTENT_WINDOW.win;
  static Query_t wavelength_query[5];

  if (init_values == FALSE)
  {
    strcpy (s_wmin, "");
    strcpy (s_wmax, "");
    init_values = TRUE;
  }

  wclear (win);

  while (valid != TRUE)
  {
    init_wavelength_form (wavelength_query, s_wmin, s_wmax);
    form_return = query_user (CONTENT_WINDOW, wavelength_query, 4, "Please input the wavelength ranges", 1);

    if (form_return == FORM_QUIT)
      return form_return;

    *wmin = strtof (wavelength_query[1].buffer, NULL);
    *wmax = strtof (wavelength_query[3].buffer, NULL);

    if (*wmax > *wmin)
    {
      valid = TRUE;
    }
    else
    {
      update_status_bar ("Invalid input for wavelength range %f - %f (minimum - maximum)", *wmin, *wmax);
    }
  }

  return EXIT_SUCCESS;
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
init_atomic_data_form (Query_t *q, char *default_data)
{
  q[0].buffer_number = 0;
  strcpy (q[0].buffer, "Master file :");
  q[0].field = new_field (1, strlen (q[0].buffer), 0, 0, 0, 0);
  q[0].opts_off = FIELD_SKIP;
  q[0].opts_on = O_VISIBLE | O_PUBLIC | O_AUTOSKIP;
  q[0].background = FIELD_SKIP;

  q[1].buffer_number = 0;
  strcpy (q[1].buffer, default_data);
  q[1].field = new_field (1, MAX_FIELD_INPUT, 0, strlen (q[0].buffer) + 2, 0, 0);
  q[1].opts_off = O_AUTOSKIP;
  q[1].opts_on = O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE;
  q[1].background = A_REVERSE;

  q[2].field = NULL;
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

void
query_atomic_data (void)
{
  int valid = FALSE;
  int atomic_data_error;
  static int menu_index = 8;
  static int init_name = FALSE;
  char atomic_data_name[MAX_FIELD_INPUT];
  static WINDOW *error_win = NULL;
  WINDOW *win = CONTENT_WINDOW.win;
  static Query_t atomic_data_query[3];

  if (init_name == FALSE)
  {
    strcpy (atomic_data_name, "");
    init_name = TRUE;
  }

  /*
   * Continue to loop until valid atomic data has been loaded
   */

  while (valid != TRUE)
  {
    menu_index = create_menu (CONTENT_WINDOW, "Please select the atomic data to use", ATOMIC_DATA_CHOICES,
                              ARRAY_SIZE (ATOMIC_DATA_CHOICES), menu_index, CONTROL_MENU);

    if (menu_index == MENU_QUIT)
    {
      update_status_bar ("Loading atomic data aborted... :-(");
      display_text_buffer (CONTENT_WINDOW, 1, 1);
      break;
    }
    else if (menu_index > MENU_QUIT)
    {
      if (ATOMIC_DATA_CHOICES[menu_index].index != INDX_OTHR)
      {
        strcpy (atomic_data_name, ATOMIC_DATA_CHOICES[menu_index].name);
        strcat (atomic_data_name, ".dat");
      }
      else
      {
        init_atomic_data_form (atomic_data_query, atomic_data_name);
        query_user (CONTENT_WINDOW, atomic_data_query, 2, "Please input the name of the atomic data master file", 1);
        strcpy (atomic_data_name, atomic_data_query[1].buffer);
      }
    }

    atomic_data_error = get_atomic_data (atomic_data_name);

    if (atomic_data_error)
    {
      update_status_bar ("Problem reading atomic data %s : errno = %i", atomic_data_name, atomic_data_error);
    }
    else
    {
      valid = TRUE;
    }

    wrefresh (win);
  }

  delwin (error_win);

  display_text_buffer (CONTENT_WINDOW, 1, 1);
  log_flush ();
}
