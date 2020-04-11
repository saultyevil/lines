
#define LINELEN 256
#define const_C_SI 299792458

typedef struct
{
  size_t len;
  char *buffer;
} ScreenBuffer_t;

#define SB_INIT {0, NULL}

#include "functions.h"
