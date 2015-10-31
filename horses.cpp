#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
  std::size_t count;
  ( std::cin >> count ).ignore();

  std::vector< int > horses( count );

  for ( std::size_t i( 0 ); i != count; ++i )
    ( std::cin >> horses[ i ] ).ignore();

  std::sort( horses.begin(), horses.end() );

  std::size_t best( horses[ 1 ] - horses[ 0 ] );

  for ( std::size_t i( 1 ); ( best != 0 ) && ( i + 1 != count ); ++i )
    {
      std::size_t d( horses[ i + 1 ] - horses [ i ] );
      if ( d < best )
        best = d;
    }
  
  std::cout << best << '\n';
  
  return 0;
}
