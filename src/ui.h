/** ************************************************************************* */
/**
 * @file
 * @author   Edward Parkinson
 * @date
 *
 * @brief
 *
 * ************************************************************************** */

#define VERSION "3.0a"
#define MAX_ROWS LINES - 4
#define MAX_COLS COLS - 2
#define QUIT -1
#define ARRAY_SIZE(x) (sizeof x / sizeof x[0])

enum
{
  ATOMIC_MEMORY_ISSUE_ERROR  = 0,
  ATOMIC_ENVRIONMENT_ERROR   = 1,
  ATOMIC_FILE_IO_ERROR       = 2,
  ATOMIC_FILE_FORMAT_ERROR   = 3,
  ATOMIC_MAX_NLTE_ERROR      = 4,
  ATOMIC_MAX_NIONS_ERROR     = 5,
  ATOMIC_MAX_LEVELS_ERROR    = 6,
  ATOMIC_MACRO_ERROR         = 7,
  ATOMIC_ERROR_TODO          = 8,
  ATOMIC_NERRORS
};
