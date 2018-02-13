#ifndef __CLIB_LIST_H__
#define __CLIB_LIST_H__

#define list_t( x ) list_##x##_t
#define list( x ) ({                                            \
      list_t( x ) tmp;                                          \
      list_##x##_init( &tmp );                                  \
      tmp;                                                      \
    })

#define list_u( x ) ({                                          \
      list_t( x )* tmp = malloc( sizeof( list_t( x ) ) );       \
      list_##x##_init( tmp );                                   \
      tmp;                                                      \
    })

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "preproc.h"

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
#else
  #define REF_TYPE TYPE*
#endif

#define TYPED_NAME( x ) CONCAT( x, CONCAT( _, TYPE ) )

#if !defined( DESTRUCTOR ) && !defined( COPY_VALUE )
  #error "Non-copy values must have specified a DESTRUCTOR!"
  #define DESTRUCTOR( x ) ( void )( x );
#endif

/** 
 * The generates the structure's name (by appending a _t to
 * typed name
 */
#define STRUCT_NAME( x ) CONCAT( TYPED_NAME( x ), _t )

/**
 * A method name is just the method's name appended to the
 * typed name
 */
#define METHOD_NAME( x, method ) CONCAT( TYPED_NAME( x ), _##method )

#define LIST_METHOD_NAME( method )                              \
  CONCAT( TYPED_NAME( list ), _##method )
#define LIST_NODE_METHOD_NAME( method )                         \
  CONCAT( TYPED_NAME( list_node ), _##method )

/** convenience for STRUCT_NAME( list_node ) */
#define LIST_NODE_T STRUCT_NAME( list_node )
/** convenience for STRUCT_NAME( list ) */
#define LIST_T      STRUCT_NAME( list )

#define LIST_NODE_VTABLE_T STRUCT_NAME( list_node_vtable_t )
#define LIST_VTABLE_T STRUCT_NAME( list_vtable_t )

/** 
 * convenient way to call/declare/define a method for
 * a list
 */
#define LIST_METHOD( name, ... )                                \
  METHOD_NAME( list, name ) ( LIST_T* this, ##__VA_ARGS__ )
#define LIST_INVOKE( name, ... )                                \
  METHOD_NAME( list, name ) ( __VA_ARGS__ )

/** 
 * convenient way to call/declare/define a method for
 * a list
 */
#define LIST_NODE_METHOD( name, ... )                           \
  METHOD_NAME( list_node, name ) ( LIST_NODE_T* this, ##__VA_ARGS__ )
#define LIST_NODE_INVOKE( name, ... )                           \
  METHOD_NAME( list_node, name ) ( __VA_ARGS__ )

//
// Forward declare all of the typedefs
//

typedef struct LIST_NODE_T LIST_NODE_T; 
typedef struct LIST_T      LIST_T; 

typedef struct LIST_NODE_VTABLE_T LIST_NODE_VTABLE_T;
typedef struct LIST_VTABLE_T LIST_VTABLE_T;

//
// Define all necessary structs
//

struct LIST_NODE_T
{
  TYPE data;
  LIST_NODE_T* prev;
  LIST_NODE_T* next;

  const LIST_NODE_VTABLE_T* fun;
};

void LIST_NODE_METHOD( init, TYPE data );
void LIST_NODE_METHOD( destroy );



struct LIST_T
{
  LIST_NODE_T* head;
  LIST_NODE_T* tail;

  unsigned int size;

  const LIST_VTABLE_T* fun;
};

void LIST_METHOD( init );
void LIST_METHOD( destroy );

void LIST_METHOD( push_front, TYPE item );
void LIST_METHOD( push_back, TYPE item );

TYPE LIST_METHOD( pop_front);
TYPE LIST_METHOD( pop_back);

TYPE LIST_METHOD( remove, unsigned int index );
REF_TYPE LIST_METHOD( get, unsigned int index );



struct LIST_NODE_VTABLE_T
{
  void ( *destroy )( LIST_NODE_T* this );
};

struct LIST_VTABLE_T
{
  void ( *destroy )( LIST_T* );
  
  void ( *push_front )( LIST_T*, TYPE );
  void ( *push_back )( LIST_T*, TYPE );

  TYPE ( *pop_front )( LIST_T* );
  TYPE ( *pop_back )( LIST_T* );

  TYPE ( *remove )( LIST_T*, unsigned int );
  REF_TYPE ( *get )( LIST_T*, unsigned int );
};


//
// list_node_t implementation
//

void LIST_NODE_METHOD( init, TYPE data )
{
  static LIST_NODE_VTABLE_T* vtable = NULL;

  if ( vtable == NULL )
  {
    vtable = calloc( 1, sizeof( LIST_NODE_VTABLE_T ) );
    vtable->destroy = &LIST_NODE_METHOD_NAME( destroy );
  }

  this->fun = vtable;
  this->data = data;
  this->prev = NULL;
  this->next = NULL;
}

void LIST_NODE_METHOD( destroy )
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
  if ( this->data != ( TYPE ) NULL )
  {
    DESTRUCTOR( this->data );
  }
#endif
  memset( this, 0, sizeof( *this ) );
}

//
// list_node implementation
//

void LIST_METHOD( init )
{
  static LIST_VTABLE_T* vtable = NULL;

  if ( vtable == NULL )
  {
    vtable = calloc( 1, sizeof( LIST_VTABLE_T ) );
    vtable->destroy = &LIST_METHOD_NAME( destroy );
    vtable->push_front = &LIST_METHOD_NAME( push_front );
    vtable->push_back = &LIST_METHOD_NAME( push_back );
    vtable->pop_front = &LIST_METHOD_NAME( pop_front );
    vtable->pop_back = &LIST_METHOD_NAME( pop_back );
    vtable->remove = &LIST_METHOD_NAME( remove );
    vtable->get = &LIST_METHOD_NAME( get );
  }

  this->fun = vtable;
  this->head = NULL;
  this->tail = NULL;
  this->size = 0;
}

void LIST_METHOD( destroy )
{
  // if we have no elements, there's nothing to free
  if ( this->size == 0 ) return;

  // free all of our nodes
  // (n.b. list_node_*_destroy will fix linkage
  LIST_NODE_T* current = this->head;
  while ( current->next != NULL )
  {
    LIST_NODE_T* next = current->next;
    LIST_NODE_INVOKE( destroy, next );
    free( next );
  }
  LIST_NODE_INVOKE( destroy, current );
  free( current );

  // zero ourselves out to indicate that we're dead
  memset( this, 0, sizeof( *this ) );
}



void LIST_METHOD( push_front, TYPE item )
{
  LIST_NODE_T* node = malloc( sizeof( LIST_NODE_T ) );
  LIST_NODE_INVOKE( init, node, item );

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

void LIST_METHOD( push_back, TYPE item )
{
  LIST_NODE_T* node = malloc( sizeof( LIST_NODE_T ) );
  LIST_NODE_INVOKE( init, node, item );

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

TYPE LIST_METHOD( pop_front )
{
  return LIST_INVOKE( remove, this, 0 );
}

TYPE LIST_METHOD( pop_back )
{
  return LIST_INVOKE( remove, this, this->size - 1 );
}

TYPE LIST_METHOD( remove, unsigned int index )
{
  assert( this->size > index );

  // TODO start from back if index >= size/2
 
  LIST_NODE_T* current = this->head;
  unsigned int i;
  for ( i = 0; i < index; i++ )
  {
    current = current->next;
  }

  TYPE item = current->data;

  // if it's not a copied value, then we're going to
  // need to prevent the destructor from deleting it
#ifndef COPY_VALUE
  current->data = ( TYPE ) NULL;
#endif

  if ( current->next == NULL )
  {
    this->tail = current->prev;
  }
  if ( current->prev == NULL )
  {
    this->head = current->next;
  }

  LIST_NODE_INVOKE( destroy, current );
  free( current );

  this->size -= 1;

  return item; 
}

REF_TYPE LIST_METHOD( get, unsigned int index )
{
  // TODO start from back if index >= size/2
 
  LIST_NODE_T* current = this->head;
  unsigned int i;
  for ( i = 0; i < index; i++ )
  {
    current = current->next;
  }

#ifdef COPY_VALUE
  return current->data;
#else
  return &current->data;
#endif
}

// don't leak any of our preprocessor symbols
#undef LIST_INVOKE
#undef LIST_NODE_VTABLE_T
#undef LIST_VTABLE_T
#undef LIST_METHOD
#undef LIST_NODE_INVOKE
#undef LIST_NODE_METHOD
#undef LIST_T
#undef LIST_NODE_T
#undef STRUCT_NAME
#undef METHOD_NAME
#undef TYPED_NAME
#undef REF_TYPE
#undef COPY_VALUE
#undef TYPE

