
#ifndef CLIB_PREPROC_H
#define CLIB_PREPROC_H

#include <stdlib.h>
#include <string.h>

#define _CONCAT( a, b ) a ## b
#define CONCAT( a, b ) _CONCAT( a, b )

#define new( x ) ({                                             \
      void* tmp = malloc( sizeof( y ) );                        \
      memcpy( tmp, &y, sizeof( y ) );                           \
      tmp2;                                                     \
    })

#define delete( x ) ({                                          \
      x->destroy( x );                                          \
      free( x );                                                \
      x = NULL;                                                 \
    })

#endif

