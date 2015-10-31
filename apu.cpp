#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
  std::size_t width;
  std::size_t height;
  ( std::cin >> width >> height ).ignore();

  std::vector< std::vector< bool > > rows
    ( height, std::vector< bool >( width ) );
  std::vector< std::vector< bool > > columns
    ( width, std::vector< bool >( height ) );

  for ( std::size_t y( 0 ); y != height; ++y )
    {
      std::string line;
      std::getline( std::cin, line );

      for ( std::size_t x( 0 ); x != width; ++x )
        {
          rows[ y ][ x ] = ( line[ x ] == '0' );
          columns[ x ][ y ] = ( line[ x ] == '0' );
        }
    }
  
  for ( std::size_t y( 0 ); y != height; ++y )
    for ( std::size_t x( 0 ); x != width; ++x )
      if ( rows[ y ][ x ] )
        {
          std::cout << x << ' ' << y << ' ';

          const auto it_row( rows[ y ].begin() + x + 1 );
          const auto row_end( rows[ y ].end() );

          auto it( std::find( it_row, row_end, true ) );
          
          if ( it != row_end )
            std::cout << it - rows[ y ].begin() << ' ' << y << ' ';
          else
            std::cout << "-1 -1 ";

          const auto it_column( columns[ x ].begin() + y + 1 );
          const auto column_end( columns[ x ].end() );

          it = std::find( it_column, column_end, true );
  
          if ( it != column_end )
            std::cout << x << ' ' << it - columns[ x ].begin();
          else
            std::cout << "-1 -1";

          std::cout << '\n';
        }
  
  return 0;
}
