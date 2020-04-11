/** ************************************************************************* */
/**
 * @file     tools.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Various utility functions.
 *
 * ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "atomic.h"
#include "pyAtomic.h"

/* ************************************************************************** */
/**
 * @brief   Get the name of an element given the atomic number
 *
 * @param[in]  z           The atomic number of the element
 * @param[out] element     The name of the element
 *
 * @return  void
 *
 * @details
 *
 * This simply loops through the element structure to find the name of the
 * element given the atomic number z.
 *
 * ************************************************************************** */

void
get_element_name (int z, char *element)
{
  int i;

  // Probably not the best to do
  if (element == NULL)
    return;

  for (i = 0; i < nelements; ++i)
  {
    if (ele[i].z == z)
    {
      strcpy (element, ele[i].name);
      break;
    }
  }
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @param[in] sb
 *
 * @return void
 *
 * @details
 *
 * ************************************************************************** */

void
display_text_buffer (ScreenBuffer_t *sb)
{
  if (!sb->buffer)
  {
    Log ("Nothing in the screen buffer to print!\n");
    return;
  }

  Log ("%s", sb->buffer);
  free (sb->buffer);
}

/* ************************************************************************** */
/**
 * @brief
 *
 * @param[in,out] sb
 * @param[in]     s
 * @param[in]     len
 *
 * @return void
 *
 * @details
 *
 * ************************************************************************** */

void
append_to_buffer (ScreenBuffer_t *sb, char *s, size_t len)
{
  char *new;
  new = realloc (sb->buffer, sb->len + len);

  if (!new)
  {
    Log ("Unable to add additional text to the output buffer :-(\n");
    exit (1);
  }

  memcpy (&new[sb->len], s, len);
  sb->buffer = new;
  sb->len += len;
}

/* ************************************************************************* */
/**
 * @brief   Print a line of dashes to the screen.
 *
 * @param[in,out]  sb   The screen buffer to append to, can be NULL
 *
 * @details
 *
 * The number of dashes is controlled by the constant ndash.
 *
 * ************************************************************************** */

void
print_separator (ScreenBuffer_t *sb)
{
  int i;
  const int ndash = 84;

  for (i = 0; i < ndash; ++i)
  {
    if (sb)
    {
      append_to_buffer (sb, "-", 1);
    }
    else
    {
      Log ("-");
    }
  }

  if (sb)
  {
    append_to_buffer (sb, "\r\n", 2);
  }
  else
  {
    Log ("\n");
  }
}