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

#include <string.h>

#include "atomix.h"

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

  // Probably not the best thing to do
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
