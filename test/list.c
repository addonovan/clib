#include <stdio.h>
#include <assert.h>
#include "primitive_lists.h"

#include "basic_struct.h"
#define TYPE basic_struct_t
#include "list.h"

void test_primitive_list();
void test_struct_list();

int main()
{
  test_primitive_list();
  test_struct_list();
  return 0;
}

void test_primitive_list()
{
  list_t(int) list = list(int);

  for ( int i = 0; i < 10; i++ )
  {
    list.fun->push_back( &list, i );
  }

  while ( list.size > 0 )
  {
    int size = ( int ) list.size - 1;
    int item = list.fun->pop_back( &list );
    if ( item != size )
    {
      printf( "Expected: %d, Actual: %d\n", size, item );
    }
  }

  list.fun->destroy( &list );
}

void test_struct_list()
{
  list_t(basic_struct_t) list = list(basic_struct_t);

  for ( int i = 0; i < 10; i++ )
  {
    basic_struct_t* item = malloc( sizeof( basic_struct_t ) );
    basic_struct_init( item, "hello world", i, 5.34 * i );
    list.fun->push_back( &list, item );
  }

  while ( list.size > 0 )
  {
    basic_struct_t* item = list.fun->pop_back( &list );
    printf( 
        "%d = { %s, %d, %lf }\n", 
        list.size, 
        item->string,
        *item->int_ptr,
        item->number
    );
    item->fun->destroy( item );
    free( item );
  }

  list.fun->destroy( &list );
}


