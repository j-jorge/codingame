#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

struct defibrilator
{
  double longitude;
  double latitude;
  std::string name;
};

void jump_to_semicolon( std::istream& s )
{
  s.ignore( std::numeric_limits<std::streamsize>::max(), ';' );
}

double deg_to_rad( double deg )
{
  return deg / 180 * 3.14159;
}

double string_to_double( std::string s )
{
  std::replace( s.begin(), s.end(), ',', '.' );
  std::istringstream iss( s );
  double result;
  iss >> result;
  return result;
}

double distance( const defibrilator& d, double longitude, double latitude )
{
  const double x
    ( ( longitude - d.longitude ) * std::cos( ( d.latitude + latitude ) / 2 ) );
  const double y( latitude - d.latitude );

  return std::sqrt( x * x + y * y ) * 6371;
}

std::vector< defibrilator >::const_iterator
find_nearest
( const std::vector< defibrilator >& defibrilators, double longitude,
  double latitude )
{
  const auto eit( defibrilators.end() );
  std::vector< defibrilator >::const_iterator result( eit );
  double nearest( std::numeric_limits< double >::max() );

  for ( auto it( defibrilators.begin() ); it != eit; ++it )
    {
      const double d( distance( *it, longitude, latitude ) );

      if ( d < nearest )
      {
        nearest = d;
        result = it;
      }
    }

  return result;
}

int main()
{
  std::string LON;
  std::cin >> LON; std::cin.ignore();
  std::string LAT;
  std::cin >> LAT; std::cin.ignore();
  
  std::size_t N;
  std::cin >> N; std::cin.ignore();

  std::vector< defibrilator > defibrilators;
  defibrilators.reserve( N );
  
  for ( std::size_t i( 0 ); i != N; i++ )
    {
      std::string DEFIB;
      std::getline(std::cin, DEFIB);

      std::istringstream iss( DEFIB );
      defibrilator d;
      jump_to_semicolon( iss );
      std::getline( iss, d.name, ';' );
      jump_to_semicolon( iss );
      jump_to_semicolon( iss );

      std::string s;
      std::getline( iss, s, ';' );
      d.longitude = deg_to_rad( string_to_double( s ) );
      
      std::getline( iss, s );
      d.latitude = deg_to_rad( string_to_double( s ) );

      defibrilators.push_back( d );
    }

  std::cout << find_nearest
    ( defibrilators, deg_to_rad( string_to_double( LON ) ),
      deg_to_rad( string_to_double( LAT ) ) )->name << '\n';

  return 0;
}
