#include <stdio.h>
#include "primitive_lists.h"

int main()
{
  list_t(int) list = list(int);

  for ( int i = 0; i < 10; i++ )
  {
    list.push_back( &list, i );
  }

  while ( list.size > 0 )
  {
    printf( "%d %d\n", list.size, list.pop_front( &list ) );
  }
  
  return 0;
}


