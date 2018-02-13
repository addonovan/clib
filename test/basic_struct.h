#ifndef __LIBC_TEST_BASIC_STRUCT_C__
#define __LIBC_TEST_BASIC_STRUCT_C__

#include <stdlib.h>
#include <string.h>

typedef struct basic_struct_t basic_struct_t;
typedef struct basic_struct_vtable_t basic_struct_vtable_t;

struct basic_struct_t
{
  char* string;
  int* int_ptr;
  double number;

  basic_struct_vtable_t* fun;
};

struct basic_struct_vtable_t
{
  void ( *destroy )( basic_struct_t* );
};

void basic_struct_init( basic_struct_t*, char*, int, double );
void basic_struct_destroy( basic_struct_t* );

void basic_struct_init( 
    basic_struct_t* this,
    char* string,
    int value,
    double number
)
{
  static basic_struct_vtable_t* vtable = NULL;

  if ( vtable == NULL )
  {
    vtable = calloc( 1, sizeof( basic_struct_vtable_t ) );
    vtable->destroy = &basic_struct_destroy;
  }

  this->fun = vtable;
  this->string = strdup( string );
  this->int_ptr = malloc( sizeof( int ) );
  *this->int_ptr = value;
  this->number = number;
}

void basic_struct_destroy( basic_struct_t* this )
{
  free( this->int_ptr );
  free( this->string );
}

#endif

