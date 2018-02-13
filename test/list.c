#include <stdio.h>
#include <assert.h>
#include "primitive_lists.h"

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
}


