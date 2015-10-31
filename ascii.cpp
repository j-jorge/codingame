#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>

using namespace std;

typedef std::vector< std::vector< char > > ascii_char;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int L;
    cin >> L; cin.ignore();
    int H;
    cin >> H; cin.ignore();
    string T;
    getline(cin, T);

    std::transform( T.begin(), T.end(), T.begin(), &::toupper );
    
    std::vector< ascii_char > chars
      ( 27, ascii_char( H, ascii_char::value_type( L, ' ' ) ) );

    for (int i = 0; i < H; i++) {
      std::string ROW;
      getline(std::cin, ROW);

      for ( std::size_t j( 0 ); j < ROW.size(); j += L )
        std::copy
          ( ROW.begin() + j, ROW.begin() + j + L,
            chars[ j / L ][ i ].begin() );
    }

    std::vector< std::size_t > index;
    index.reserve( T.size() );
    
    for ( const char& c : T )
      if ( ( c >= 'A' ) && ( c <= 'Z' ) )
        index.push_back( c - 'A' );
      else
        index.push_back( chars.size() - 1 );


    for ( int line( 0 ); line != H; ++line )
      {
        for ( const std::size_t& i : index )
          std::copy( chars[ i ][ line ].begin(), chars[ i ][ line ].end(),
                     std::ostream_iterator<char>( std::cout ) );
        std::cout << '\n';
      }

    return 0;
}
