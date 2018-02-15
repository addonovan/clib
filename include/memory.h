#ifndef __CLIB_MEMORY_H__
#define __CLIB_MEMORY_H__

#define delete(x)                                                              \
  do                                                                           \
  {                                                                            \
    x->fun->destroy( x );                                                      \
    free( x );                                                                 \
    x = NULL;                                                                  \
  }                                                                            \
  while ( 0 );                                                                 \

#endif

