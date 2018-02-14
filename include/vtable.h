#ifndef __CLIB_VTABLE_H__
#define __CLIB_VTABLE_H__

#include "generic.h"
#include "preproc.h"

#define vtable_t(x)                                                            \
  CAT(__vtable_, x)

#define VTABLE_PTR_METHOD(x) CAT(_VTABLE_PTR_, x)
#define _VTABLE_PTR_METHOD(return, type, name, ...)                            \
  return ( *name ) ( __VA_ARGS__ );

#define VTABLE_DEC_METHOD(x) CAT(_VTABLE_DEC_, x)
#define _VTABLE_DEC_METHOD(return, type, name, ...)                            \
  return type##_##name( __VA_ARGS__ );

#define DEFINE_VTABLE(type, ...)                                               \
  typedef struct vtable_t(type) vtable_t(type);                                \
  struct vtable_t(type)                                                        \
  {                                                                            \
    EVAL(MAP(VTABLE_PTR_METHOD, __VA_ARGS__))                                  \
  };                                                                           \
  EVAL(MAP(VTABLE_DEC_METHOD, __VA_ARGS__))

#endif

