#ifndef __CLIB_ITER_H__
#define __CLIB_ITER_H__

#define iter_t(x) CAT(iter_, x)

#include "preproc.h"
#include "vtable.h"

#endif // END OF HEADER GUARD

// we must know the type we're iterating over
#ifndef COLLECTION_TYPE
  #error "COLLECTION_TYPE must be specified prior to inclusion"
  #define COLLECTION_TYPE list_int_t
  #include "primitive_lists.h"
#endif

#ifndef ITEM_TYPE
  #error "ITEM_TYPE must be specified prior to inclusion"
  #define ITEM_TYPE int
#endif

#define I_P PREFIX(iter, COLLECTION_TYPE)
#define I_T STRUCT_TYPE(iter, COLLECTION_TYPE) 
#define I_VT CAT(__vtable_, I_P)

typedef struct I_T I_T;

DEFINE_VTABLE(
  I_T,
  METHOD(void, I_P, init, I_T* this, const COLLECTION_TYPE* src),
  METHOD(void, I_P, destroy, I_T* this),

  METHOD(ITEM_TYPE, I_P, next, I_T* this),
  METHOD(ITEM_TYPE, I_P, peek, I_T* this)
)

struct I_T
{
  const COLLECTION_TYPE* src;
  unsigned int index;

  vtable_t(I_T)* fun;
};

vtable_t(I_T)* I_VT = NULL;

DEF_METHOD(void, I_T, init, I_T* this, const COLLECTION_TYPE* src)
{
  if ( I_VT == NULL )
  {
    I_VT = calloc( sizeof( *I_VT ), 1 );
    I_VT->init = &METHOD_NAME( I_P, init );
    I_VT->destroy = &METHOD_NAME( I_P, destroy );
    I_VT->next = &METHOD_NAME( I_P, next );
    I_VT->peek = &METHOD_NAME( I_P, peek );
  }

  this->src = src;
  this->index = 0;
}

DEF_METHOD(void, I_T, destroy, I_T* this)
{
  this->src = NULL;
  this->index = 0;
}

DEF_METHOD(ITEM_TYPE, I_T, next, I_T* this)
{
  ITEM_TYPE item = this->fun->peek(this);
  this->index += 1;
  return item; 
}

DEF_METHOD(ITEM_TYPE, I_T, peek, I_T* this)
{
  return this->src->fun->get( this->src, this->index );
}

// don't leak preprocessor types
#undef I_VT
#undef I_T
#undef ITEM_TYPE
#undef COLLECTION_TYPE

