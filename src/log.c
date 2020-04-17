/***********************************************************/
/** @file  xlog.c
 * @author ksl
 * @date   January, 2018
 *
 * @brief  These are a series of routines designed to store comments and errors
 * in a diagnostic file or files.  The routines also provide a mechanism for tracking the numbers of erorrs
 * of each type
 *
 * Instead of using printf and fprintf statements throughout varius subroutines, the set of routines
 * here are intended to provide a standard interface to various diagnostic files, and to manage the inteaction of
 * logging with the various mpi threads that can exist when running Python in multiprocessor mode.  The routines
 * also contain a verbosity mechanism to allow one to control how much information is written to the screen, and a
 * mechanism to keep track of the number of times a particular error message has been generated.  The overall goal
 * is to keep the log files produced by Python a manageable size.
 *
 * Messages in Python runs are sent to the screen and to diagnostic files.  During multiprocessing runs, a diagnostic
 * file is opened for each thread.  With some exceptions, messages to the screen arise from thread 0.
 *
 *  The routines that control what files are open and closed for logging are as follows.
 * 	- Log_init(filename)			Open a logfile
 * 	- Log_append(filename)			Repoane an existing log file (so that one can continue to log to it)
 * 	- Log_close()					Close the current logfile
 *
 *   The routines that write to the log files (and optionally the screen) for informational reasons are:
 *  - Log ( char *format, ...)			Send a message to the screen and logfile
 * 	- Log_silent ( char *format, ...)		Send a message to the logfile
 *
 * 	The routines that are designed to report errors are as follows:
 * 	- Shout( char *format, ...)	   Send a message to the screen and logfile that cannot be suppressed.
 *  - Error ( char *format, ...)			Send a message prefaced by the word "Error:" to the screen and to the logfile.
 * 	- Error_silent ( char *format, ...)		Send a message prefaced by the word "Error:" to
 * 										the logfile
 *
 *  One can control how much information is printed to the screen and how many times a specific error message is logged
 *  to a file with several routines
 *  - Log_set_verbosity(vlevel)			Set the verbosity of the what is printed to the screen and the log file
 * 	- Log_print_max(print_max)			Set the number of times a single error will be
 * 	    output to the screen and the log file
 *
 *  In most cases, it is sufficient to log to the screen only from thread 0.  there are a few times, one might want to
 *  send a message to the screen from any thread. For this purpose there is a specific command:
 *  - Log_parallel(message)			Log statement for parallel reporting
 *
 *
 *  There are several specific commands that have been included for debugging problems:
 *  - Log_flush()					simply flushes the logfile to disk (before the program crashes).
 *	- Debug( char *format, ...) 			Log an statement to the screen and to a file.  This is essentially a
 *								intended to replace a printf statement in situations where
 *							one is debugging code.  The use of Debug instead of log
 *							means that a future developer should be free to remove the
 *							Debug statement from the code.  Because the command prepends the word Debug to
 *							the write staatement, it should be easy to grep debug statements out of the log files
 *
 *
 *  For errors, the logging routines keep track of how many times a particular error has been reported, using the format
 *  statement as a proxy for the error.  After a certin number of times a particular error has been roported the error is
 *  no longer written out to the diag file, but the routens still keep trank of the nubmer of times the error is reported.
 *  If this number becomes too large then, the progam exits, and when it does it indicates why. There are a nubmer of
 *  routines associated with this.
 *
 *
 *  - error_summary(char *format)			Summarize all of the erors that have been
 * 								logged to this point in time
 *
 *
 *  In addition there are several routines that largely internal
 *
 *  - Log_set_mpi_rank(rank, n_mpi)		Tells  the rank of the parallel process in parallel mode,
 * 								and divides max errors by n_mpi
 *
 *
 *
 *
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
add_to_log (char *format, ...)
{
  va_list ap, ap2;
  int result;

  if (init_log == 0)
    log_init ("logfile");

  va_start (ap, format);
  va_copy (ap2, ap);            /* ap is not necessarily preserved by vprintf */
  result = vfprintf (diagptr, format, ap2);
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
 **********************************************************/

int
add_error_to_log (char *format, ...)
{
  va_list ap, ap2;
  int result;

  if (init_log == 0)
    log_init ("logfile");

  va_start (ap, format);
  va_copy (ap2, ap);
  fprintf (diagptr, "Error: ");
  result = vfprintf (diagptr, format, ap2);
  va_end (ap);
  return (result);
}
