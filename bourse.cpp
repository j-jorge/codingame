#include <iostream>
#include <vector>

int main()
{
  std::size_t n;
  std::cin >> n;
  std::vector< std::size_t > v( n );

  for ( std::size_t& value : v )
    std::cin >> value;

  std::size_t highest( v[ 0 ] );
  std::size_t gap( 0 );
  
  for ( std::size_t i( 1 ); i != n; ++i )
    {
      const std::size_t value( v[ i ] );

      if ( value > highest )
        highest = value;
      else
        {
          const std::size_t diff( highest - value );
          if ( diff > gap )
            gap = diff;
        }
    }

  if ( gap == 0 )
    std::cout << "0\n";
  else
    std::cout << '-' << gap << '\n';
  
  return 0;
}
