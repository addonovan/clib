#ifndef __LIST_H__
#define __LIST_H__

#define list_t(x) list_##x##_t
#define list(x) ({                                              \
      list_t(x) tmp;                                            \
      list_##x##_init( &tmp );                                  \
      tmp;                                                      \
    })

#define list_u(x) ({                                            \
      list_t(x)* tmp = malloc( sizeof( list_t(x) ) );           \
      list_##x##_init( tmp );                                   \
      tmp;                                                      \
    })

#define TYPE int
#define TYPED_NAME(x) x##_int
#define COPY_VALUE 
#include "list_impl.h"

#define TYPE double
#define TYPED_NAME(x) x##_double
#define COPY_VALUE
#include "list_impl.h"

#endif

