#ifdef AT_HOME
#define _DEBUG
#define DBG(x) x
#else
#define NDEBUG
#define DBG(x) do {} while( false )
#endif

#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <cassert>

struct range
{
  std::int64_t first;
  std::int64_t last;
};

struct house
{
  std::int64_t coord;
  std::int64_t count;
};

typedef std::int64_t distance_type;

distance_type
sum_distances_before( distance_type limit, const std::vector< house >& houses )
{
  distance_type result( 0 );

  for ( const house& h : houses )
    if ( h.coord >= limit )
      return result;
    else
      result += ( limit - distance_type( h.coord ) ) * h.count;

  return result;
}

distance_type
sum_distances_after( distance_type limit, const std::vector< house >& houses )
{
  distance_type result( 0 );
  const auto eit( houses.rend() );
  
  for ( auto it( houses.rbegin() ); it != eit; ++it )
    if ( it->coord <= limit )
      return result;
    else
      result += ( distance_type( it->coord ) - limit ) * it->count;

  return result;
}

std::int64_t search_pivot
( range r, const std::vector< house >& houses )
{
  assert( r.first <= r.last );

  DBG( std::cerr << "search in " << r.first << ", " << r.last << '\n' );
  
  const distance_type pivot( distance_type( r.first + r.last ) / 2 );
  const distance_type before( sum_distances_before( pivot, houses ) );
  const distance_type after( sum_distances_after( pivot, houses ) );

  distance_type best( before + after );
  
  if ( before == after )
    return before + after;

  DBG( std::cerr << "INIT\npivot=" << std::int64_t( pivot )
       << ", before=" << std::int64_t( before )
       << ", after=" << std::int64_t( after )
       << ", best=" << std::int64_t( best ) << '\n' );

  while ( r.first < r.last )
    {
      DBG( const std::int64_t dist( r.last - r.first + 1 ) );
      DBG( std::cerr << "search " << r.first << ", " << r.last
           << ", dist=" << dist << '\n' );
      
      const distance_type pivot( distance_type( r.first + r.last ) / 2 );
      
      const distance_type d_left
        ( sum_distances_before( pivot, houses )
          + sum_distances_after( pivot, houses ) );

      const distance_type d_right
        ( sum_distances_before( pivot + 1, houses )
          + sum_distances_after( pivot + 1, houses ) );

      DBG( std::cerr << "pivot=" << std::int64_t( pivot )
           << ", left=" << std::int64_t( d_left )
           << ", right=" << std::int64_t( d_right )
           << ", best=" << std::int64_t( best ) << '\n' );
      
      if ( d_left < d_right )
        {
          r.last = pivot - 1;
          best = std::min( d_left, best );
        }
      else
        {
          r.first = pivot + 1;
          best = std::min( d_right, best );
        }
    }

  DBG( std::cerr << "normal exit " << best << '\n' );
  
  return best + 0.5;
}

int main()
{
  std::size_t n;
  std::cin >> n;
  
  std::vector< house > houses;
  
  std::int64_t xmin( std::numeric_limits< std::int64_t >::max() );
  std::int64_t xmax( std::numeric_limits< std::int64_t >::min() );
  std::int64_t ymin( std::numeric_limits< std::int64_t >::max() );
  std::int64_t ymax( std::numeric_limits< std::int64_t >::min() );

  for ( std::size_t i( 0 ); i != n; ++i )
    {
      std::int64_t x;
      std::int64_t y;
      std::cin >> x >> y;

      if ( !houses.empty() && ( houses.back().coord == y ) )
        ++houses.back().count;
      else
        {
          houses.push_back( house{ y, 1 } );
          
          ymin = std::min( ymin, y );
          ymax = std::max( ymax, y );
        }
      
      xmin = std::min( xmin, x );
      xmax = std::max( xmax, x );
    }

  std::sort
    ( houses.begin(), houses.end(),
      []( const house& a, const house& b ) -> bool
      {
        return a.coord < b.coord;
      } );

  const std::int64_t distances( search_pivot( range{ ymin, ymax }, houses ) );
  
  std::cout << xmax - xmin + distances << '\n';
  
  return 0;
}

         
