#define NDEBUG

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

struct job
{
  std::size_t s;
  std::size_t d;
};

bool starts_before( const job& a, const job& b )
{
  if ( a.s == b.s )
    return a.d < b.d;
  
  return a.s < b.s;
}

int main()
{
  std::size_t count;
  std::cin >> count;

  std::vector< job > jobs( count );
  std::size_t max_end( 0 );
  
  for( job& j : jobs )
    {
      std::cin >> j.s >> j.d;
      max_end = std::max( max_end, j.s + j.d );
    }

  std::sort( jobs.begin(), jobs.end(), &starts_before );

  std::map< std::size_t, std::size_t > max_at;
  max_at[ max_end ] = 0;
  std::size_t result( 0 );
  
  for ( std::size_t i( 0 ); i != count; ++i )
    {
      const std::size_t s( count - i - 1 );
      const std::size_t start( jobs[ s ].s );
      const std::size_t end( start + jobs[ s ].d );
      const auto eit( max_at.end() );

      const auto start_it( max_at.lower_bound( start ) );
      const auto end_it( max_at.lower_bound( end ) );
      
      std::size_t start_max( start_it->second );
      std::size_t end_max( end_it->second + 1 );
      std::size_t max( std::max( start_max, end_max ) );
      result = std::max( result, max );
      max_at[ start ] = max;
    }

  std::cout << result << '\n';

  return 0;
}
