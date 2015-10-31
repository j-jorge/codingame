#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include <algorithm>

typedef std::uint64_t value_type;

typedef std::vector<char> digit;

std::array< digit, 20 > g_numerals;

value_type digit_to_base_10( const digit& d )
{
  auto bit( g_numerals.begin() );
  auto eit( g_numerals.end() );
  auto it( std::find( bit, eit, d ) );
  assert( it != eit );
  return it - bit;
}

const digit& base_10_to_digit( value_type v )
{
  assert( v < g_numerals.size() );
  return g_numerals[ v ];
}

value_type build_number_base_10( value_type v, const digit& d )
{
  return v * 20 + digit_to_base_10( d );
}

std::vector< digit > build_number_base_20( value_type v )
{
  std::vector< digit > result;

  do
    {
      result.push_back( g_numerals[ v % 20 ] );
      v /= 20;
    }
  while( v != 0 );

  return result;
}

digit read_digit( std::istream& s, std::size_t w, std::size_t h )
{
  digit result;
  result.reserve( w * h );

  std::string line;
  
  for ( ; h != 0; --h )
    {
      std::getline( s, line );
      result.insert( result.end(), line.begin(), line.end() );
    }
  
  return result;
}

void write_digit( std::ostream& s, const digit& d, std::size_t w )
{
  for ( std::size_t i( 0 ); i != d.size(); ++i )
    {
      s << d[ i ];
      if ( ( i + 1 ) % w == 0 )
        s << '\n';
    }
}

value_type read_to_base_10( std::istream& s, std::size_t w, std::size_t h )
{
  std::size_t n;
  ( s >> n ).ignore();

  n /= h;
  
  value_type result( 0 );
  for ( ; n != 0; --n )
    result = build_number_base_10( result, read_digit( s, w, h ) );
  
  return result;
}

int main()
{
  std::size_t w;
  std::size_t h;

  ( std::cin >> w >> h ).ignore();

  std::string line;
  
  for( std::size_t i( 0 ); i != h; ++i )
    {
      std::getline( std::cin, line );

      assert( line.size() % w == 0 );
      
      for ( std::size_t j( 0 ); j != g_numerals.size(); ++j )
        g_numerals[ j ].insert
          ( g_numerals[ j ].end(),
            line.begin() + j * w, line.begin() + ( j + 1 ) * w );
    }

  const value_type a( read_to_base_10( std::cin, w, h ) );
  const value_type b( read_to_base_10( std::cin, w, h ) );

  value_type result;
  char op;
  std::cin >> op;

  switch( op )
    {
    case '+':
      result = a + b;
      break;
    case '-':
      result = a - b;
      break;
    case '*':
      result = a * b;
      break;
    case '/':
      result = a / b;
      break;
    default:
      {
        assert( false );
      }
    }

  const std::vector< digit > result_20( build_number_base_20( result ) );
  const std::size_t length( result_20.size() );
  
  for ( std::size_t i( 0 ); i != length; ++i )
    write_digit( std::cout, result_20[ length - i - 1 ], w );

  return 0;
}
