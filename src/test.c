
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "preproc.h"
#include "list.h"

int main()
{
  list_t(int) list = list(int);

  int i;
  for ( i = 0; i < 10; i++ )
  {
    list.push_back( &list, 5 * i );
  }

  for ( i = 0; i < 10; i++ )
  {
    printf( "%d\n", list.pop_back( &list ) );
  }

  list_t(double)* dubs = list_u(double);

  for ( i = 0; i < 10; i++ )
  {
    dubs->push_back( dubs, 5.0 * ( i + 1.5 ) );
  }

  for ( i = 0; i < 10; i++ )
  {
    printf( "%lf\n", dubs->pop_back( dubs ) );
  }

  printf( "sizeof list == %zi\n", sizeof( list ) );
  printf( "sizeof dubs == %zi\n", sizeof( dubs ) );

  delete( dubs );
  list.destroy( &list );

  return 0;
}

