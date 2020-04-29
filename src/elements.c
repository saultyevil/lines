/* ************************************************************************** */
/**
 * @file     elements.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * ************************************************************************** */

#include <math.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 *
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
elements_main_menu (void)
{
	get_single_element (20);  // hardcoded to Calcium for now
	display_text_buffer (CONTENT_WINDOW, SCROLL_OK);
}

/* ************************************************************************** */
/**
 *
 * @brief
 *
 * @details
 *
 * ************************************************************************** */

void
get_single_element (int z)
{
	int i;
	const int ndash = 40;
	struct elements element;

	for (i = 0; i < nelements; ++i)
	{
		element = ele[i];
		if (element.z == z)
			break;
	}

	add_to_display_buffer (" Element: %s", element.name);
	add_separator_to_buffer (ndash);

	add_to_display_buffer (" Z                        : %i", element.z);
	add_to_display_buffer (" Abundance relative to H  : %3.2f", log10 (element.abun) + 12);
	add_to_display_buffer (" Number of Ions           : %i", element.nions);
	add_to_display_buffer (" First Ion Index          : %i", element.firstion);
	add_to_display_buffer (" Last Ion Index           : %i", element.firstion + element.nions - 1);
	add_to_display_buffer (" Highest Ionisation state : %i", element.istate_max);

	add_separator_to_buffer (ndash);
}
