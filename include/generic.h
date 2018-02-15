#ifndef __LIBC_GENERIC_H__
#define __LIBC_GENERIC_H__

#include "preproc.h"

#define METHOD(return, type, name, ...)                                        \
  METHOD(return, type, name, __VA_ARGS__)

#define DEF_METHOD(return, type, name, ...)                                    \
  return METHOD_NAME(type, name)( __VA_ARGS__ )

#define METHOD_NAME(type, name)                                                \
  CAT(type, _##name)

#define PREFIX(name, type)                                                     \
  CAT(name, CAT(_, type))

#define STRUCT_TYPE(name, type)                                                \
  CAT(PREFIX(name, type), _t)

#endif

