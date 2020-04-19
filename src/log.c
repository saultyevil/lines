/***********************************************************/
/**
 * @file   log.c
 * @author ksl
 * @date   January, 2018
 *
 * @brief
 *
 * These are a series of routines designed to store comments and errors in a
 * diagnostic file or files.
 *
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "log.h"

#define LINELENGTH 256

FILE *diagptr;
int init_log = 0;

/**********************************************************/
/**
 * @brief      Open a log file
 *
 * @param [in] char *  filename   The name of the file where logging will occur
 * @return     Always returns 0
 *
 * ###Notes###
 *
 *
 **********************************************************/

int
log_init (filename)
     char *filename;
{
  if ((diagptr = fopen (filename, "w")) == NULL)
  {
    printf ("Yikes: could not even open log file %s\n", filename);
    exit (EXIT_FAILURE);
  }
  init_log = 1;

  return (0);
}

/**********************************************************/
/**
 * @brief      Close a log file
 *
 * @return     Always returns 0
 *
 * Close the current log file
 *
 * ###Notes###
 *
 *
 **********************************************************/

int
log_close ()
{
  fclose (diagptr);
  init_log = 0;           // Release the error summary structure
  return (0);
}

/**********************************************************/
/**
 * @brief      Print/write an informational message
 *
 * @param [in] char *  format   The format string for the message
 * @param [in]   ...   The various values which fill out the format screen
 * @return     The number of characters sucessfully written
 *
 * This is the standard way of printing a message to screen and to the diag file
 *
 * ###Notes###
 *
 * Printing to the screen can be suppressed by setting the verbosity level
 *
 **********************************************************/

int
logfile (char *format, ...)
{
  va_list ap, ap2;
  int result;

  if (init_log == 0)
    log_init ("logfile");

  va_start (ap, format);
  va_copy (ap2, ap);            /* ap is not necessarily preserved by vprintf */
  result = vfprintf (diagptr, format, ap2);
  va_end (ap);
  va_end (ap);
  return (result);
}


/**********************************************************/
/**
 * @brief      Print/write out an error message
 *
 * @param [in] char *  format   The format string for the message
 * @param [in]   ...   The various values which fill out the format statement
 * @return     The number of characters sucessfully written
 *
 *
 * This is the standard way of writing an error message to the screen and to
 * a file
 *
 * ###Notes###
 *
 * Writing to the screen can be suppressed depending on the level of verbosity
 *
 * TODO: seperate error log file
 *
 **********************************************************/

int
add_error_to_log (char *format, ...)
{
  va_list ap, ap2;
  int result = 0;

  if (init_log == 0)
    log_init ("logfile");

  va_start (ap, format);
  va_copy (ap2, ap);
//  fprintf (diagptr, "Error: ")
//  result = vfprintf (diagptr, format, ap2);
  va_end (ap);
  va_end (ap2);

  return (result);
}

/**********************************************************/
/**
 * @brief      Flush the diagnostic file to assure that one has an up-to-date version of the log file
 *
 * @return     Always returns 0
 *
 * This routine is intended to assure that the log file is complete before
 * a possilbe program crash
 *
 * ###Notes###
 *
 *
 *
 **********************************************************/

int
log_flush ()
{
  if (init_log == 0)
    log_init ("logfile");

  fflush (diagptr);
  return (0);
}
