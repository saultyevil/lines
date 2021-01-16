/* ************************************************************************** */
/**
 * @file     levels.c
 * @author   Edward Parkinson
 * @date     April 2020
 *
 * @brief
 *
 * Functions for querying the atomic configuration levels data.
 * 
 * ************************************************************************** */

#include "atomix.h"

static const int ndash = 80;

/* ************************************************************************** */
/**
 * @brief
 * 
 * @details
 * 
 * ************************************************************************** */

void
atomic_level_header(void)
{
  add_sep_display(ndash);
  display_add(" %-12s %-12s %-12s %-12s %-12s", "Z", "istate", "nion", "nden", "ilv");
  add_sep_display(ndash);
}

/* ************************************************************************** */
/**
 * @brief
 * 
 * @details
 * 
 * ************************************************************************** */

void
atomic_level_line(int n)
{
  ConfigPtr c;
  c = &config[n];
  display_add(" %-12i %-12i %-12i %-12i %-12i", c->z, c->istate, c->nion, c->nden, c->ilv);
}

/* ************************************************************************** */
/**
 * @brief
 * 
 * @details
 * 
 * ************************************************************************** */

void
all_level_configurations(void)
{
  int i;

  atomic_level_header();

  for(i = 0; i < nlevels; ++i)
    atomic_level_line(i);

  count(ndash, nlevels);
  display_show(SCROLL_ENABLE, true, 3);
}
