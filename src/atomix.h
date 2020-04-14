/** ************************************************************************* */
/**
 * @file
 * @author   Edward Parkinson
 * @date
 *
 * @brief
 *
 * ************************************************************************** */

/* ************************************************************************** */
/**
 *
 * ************************************************************************** */

#define LINELEN 256
#define const_C_SI 299792458

#define VERSION "3.0a"
#define MAX_ROWS LINES - 4
#define MAX_COLS COLS - 2
#define QUIT -1
#define ARRAY_SIZE(x) (sizeof x / sizeof x[0])

/* ************************************************************************** */
/**
 *
 * ************************************************************************** */

typedef struct
{
  size_t len;
  char *buffer;
} ScreenBuffer_t;

#define SB_INIT {0, NULL}

/* ************************************************************************** */
/**
 *
 * ************************************************************************** */

// TODO clean up includes for final release

#include <menu.h>
#include <curses.h>

#include "atomic.h"

#include "log_functions.h"
#include "atomic_functions.h"
#include "functions.h"
