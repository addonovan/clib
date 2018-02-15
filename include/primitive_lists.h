#ifndef __CLIB_PRIMITIVE_LISTS_H__
#define __CLIB_PRIMITIVE_LISTS_H__

#define TYPE int
#define COPY_VALUE 
#include "list.h"

#define TYPE double
#define COPY_VALUE
#include "list.h"

typedef char* string;
#define TYPE string
#define COPY_VALUE
#include "list.h"

#define TYPE char
#define COPY_VALUE
#include "list.h"

#endif

