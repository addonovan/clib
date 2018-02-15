#ifndef __CLIB_LIST_H__
#define __CLIB_LIST_H__

#define list_t( x ) list_##x##_t
#define list_node_t( x ) list_node_##x##_t

#define list_iter_t( x ) CAT(iter_, list_node_t(x))

#define list( x ) ({                                                           \
      list_t( x ) tmp;                                                         \
      list_##x##_init( &tmp );                                                 \
      tmp;                                                                     \
    })

#define list_u( x ) ({                                                         \
      list_t( x )* tmp = malloc( sizeof( list_t( x ) ) );                      \
      list_##x##_init( tmp );                                                  \
      tmp;                                                                     \
    })

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "preproc.h"
#include "vtable.h"

#endif // END OF INCLUDE GUARD

// we *must* know the type
#ifndef TYPE
  #error "TYPE must be defined before list.h is included"
  #define TYPE int
#endif

// if they didn't define COPY_VALUE, then we assume
// that we assume that the reference type should be
// a const pointer
#ifdef COPY_VALUE 
  #define REF_TYPE TYPE
  #define CONST_REF_TYPE TYPE
#else
  #define REF_TYPE TYPE*
  #define CONST_REF_TYPE const TYPE*
#endif

#define LN_P PREFIX(list_node, TYPE)
#define L_P  PREFIX(list, TYPE)

#define LN_T STRUCT_TYPE(list_node, TYPE)
#define L_T  STRUCT_TYPE(list, TYPE)

#define LN_VT CAT(__vtable, LN_P)
#define L_VT  CAT(__vtable, L_P)

#define LN_METHOD(x) METHOD_NAME(LN_P, x)
#define L_METHOD(x) METHOD_NAME(L_P, x)

#if defined(HEADER_ONLY)

//
// Forward declare all of the typedefs
//

typedef struct LN_T LN_T; 
typedef struct L_T L_T;

//
// Vtables
//

DEFINE_VTABLE(
  LN_T, 
  METHOD(void, LN_P, init, LN_T* this, REF_TYPE data),
  METHOD(void, LN_P, destroy, LN_T* this),
)

vtable_t(LN_T)* LN_VT = NULL;

DEFINE_VTABLE(
  L_T,
  METHOD(void, L_P, init,    L_T* this),
  METHOD(void, L_P, destroy, L_T* this),

  METHOD(void, L_P, push, L_T* this, REF_TYPE item),
  METHOD(void, L_P, enqueue,  L_T* this, REF_TYPE item),

  METHOD(REF_TYPE, L_P, pop, L_T* this),
  METHOD(REF_TYPE, L_P, pop_back,  L_T* this),

  METHOD(REF_TYPE, L_P, remove, L_T* this, unsigned int index),

  METHOD(CONST_REF_TYPE, L_P, get, const L_T* this, unsigned int index)
)

vtable_t(L_T)* L_VT = NULL;

//
// Struct definitions
//

struct LN_T
{
  REF_TYPE data;
  LN_T* prev;
  LN_T* next;

  const vtable_t(LN_T)* fun;
};

struct L_T
{
  LN_T* head;
  LN_T* tail;

  unsigned int size;

  const vtable_t(L_T)* fun;
};


#else

//
// list_node_t implementation
//

void METHOD_NAME(LN_P, init)( LN_T* this, REF_TYPE data )
{
  this->fun = LN_VT;
  this->data = data;
  this->prev = NULL;
  this->next = NULL;
}

void METHOD_NAME(LN_P, destroy)( LN_T* this )
{
  // maintain the surrounding nodes' linkage
  if ( this->prev != NULL )
  {
    this->prev->next = this->next;
  }
  if ( this->next != NULL )
  {
    this->next->prev = this->prev;
  }

#ifndef COPY_VALUE
  if ( this->data != ( REF_TYPE ) NULL )
  {
    // run destructor
  }
#endif
  memset( this, 0, sizeof( *this ) );
}

//
// list implementation
//

DEF_METHOD(void, L_P, init,    L_T* this)
{
  if ( L_VT == NULL )
  {
    L_VT = calloc( 1, sizeof( *L_VT ) );
    L_VT->destroy = &METHOD_NAME(L_P,  destroy );
    L_VT->push = &METHOD_NAME(L_P,  push );
    L_VT->enqueue = &METHOD_NAME(L_P,  enqueue );
    L_VT->pop = &METHOD_NAME(L_P,  pop );
    L_VT->pop_back = &METHOD_NAME(L_P,  pop_back );
    L_VT->remove = &METHOD_NAME(L_P,  remove );
    L_VT->get = &METHOD_NAME(L_P,  get );

    // while we're at it, let's create the vt for our nodes
    // so we don't have to worry about it in the future
    LN_VT = calloc( 1, sizeof( *LN_VT ) );
    LN_VT->init = &METHOD_NAME(LN_P, init);
    LN_VT->destroy = &METHOD_NAME(LN_P, destroy);
  } 

  this->fun = L_VT;
  this->head = NULL;
  this->tail = NULL;
  this->size = 0;
}

DEF_METHOD(void, L_P, destroy, L_T* this)
{
  // if we have no elements, there's nothing to free
  if ( this->size == 0 ) return;

  // free all of our nodes
  // (n.b. list_node_*_destroy will fix linkage
  LN_T* current = this->head;
  while ( current->next != NULL )
  {
    LN_T* next = current->next;
    LN_VT->destroy( next );
    free( next );
  }
  LN_VT->destroy( current );
  free( current );

  // zero ourselves out to indicate that we're dead
  memset( this, 0, sizeof( *this ) );
}

DEF_METHOD(void, L_P, push, L_T* this, REF_TYPE item)
{
  LN_T* node = malloc( sizeof( LN_T ) );
  LN_VT->init( node, item );

  node->next = this->head;
  if ( this->size == 0 )
  {
    this->tail = node;
  }
  else
  {
    this->head->prev = node;
  }
  this->head = node;

  this->size += 1;
}

DEF_METHOD(void, L_P, enqueue,  L_T* this, REF_TYPE item)
{
  LN_T* node = malloc( sizeof( LN_T ) );
  LN_VT->init( node, item );

  node->prev = this->tail;
  if ( this->size == 0 )
  {
    this->head = node;
  }
  else
  {
    this->tail->next = node;
  }
  this->tail = node;

  this->size += 1;
}

DEF_METHOD(REF_TYPE, L_P, pop, L_T* this)
{
  return L_VT->remove( this, 0 );
}

DEF_METHOD(REF_TYPE, L_P, pop_back,  L_T* this)
{
  return L_VT->remove( this, this->size - 1 );
}

DEF_METHOD(REF_TYPE, L_P, remove, L_T* this, unsigned int index)
{
  assert( this->size > index );

  // TODO start from back if index >= size/2
 
  LN_T* current = this->head;
  unsigned int i;
  for ( i = 0; i < index; i++ )
  {
    current = current->next;
  }

  REF_TYPE item = current->data;

  // if it's not a copied value, then we're going to
  // need to prevent the destructor from deleting it
#ifndef COPY_VALUE
  current->data = ( REF_TYPE ) NULL;
#endif

  if ( current->next == NULL )
  {
    this->tail = current->prev;
  }
  if ( current->prev == NULL )
  {
    this->head = current->next;
  }

        LN_VT->destroy( current );
  free( current );

  this->size -= 1;

  return item; 
}

DEF_METHOD(CONST_REF_TYPE, L_P, get, const L_T* this, unsigned int index)
{
  // TODO start from back if index >= size/2
 
  LN_T* current = this->head;
  unsigned int i;
  for ( i = 0; i < index; i++ )
  {
    current = current->next;
  }

#ifdef COPY_VALUE
  return current->data;
#else
  return current->data;
#endif
}

#endif // ifndef(HEADER_ONLY)

// don't leak any of our preprocessor symbols
#undef HEADER_ONLY
#undef L_METHOD
#undef LN_METHOD
#undef L_VT
#undef LN_VT
#undef L_T
#undef LN_T
#undef L_P
#undef LN_P
#undef CONST_REF_TYPE
#undef REF_TYPE
#undef COPY_VALUE
#undef TYPE

