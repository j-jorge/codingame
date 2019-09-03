#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

constexpr int max_player_count = 4;
constexpr int rows = 20;
constexpr int columns = 35;
constexpr char free_cell = '.';
constexpr int max_int = std::numeric_limits< int >::max();

struct point
{
  int x;
  int y;

  bool operator==( const point& p ) const
  {
    return ( x == p.x ) && ( y == p.y );
  }
};

struct goal
{
  point p;
  int distance;
};
  
typedef std::array< std::array< char, columns >, rows > terrain;

goal furthest_reachable_free_cell
( const terrain& terrain, const std::array< point, max_player_count >& players,
  int player_count )
{
  goal result{ point{ -1, -1 }, -1 };
  
  for ( int y( 0 ); y != rows; ++y )
    for ( int x( 0 ); x != columns; ++x )
      if ( terrain[ y ][ x ] == free_cell )
      {
        std::array< int, max_player_count > distance_to_player
          {
           max_int, max_int, max_int, max_int
          };

        for ( int i( 0 ); i != player_count; ++i )
          {
            const point& p( players[ i ] );
            distance_to_player[ i ] =
              std::abs( x - p.x ) + std::abs( y - p.y );
          }

        const int distance_to_player_0( distance_to_player[ 0 ] );
        
        if ( distance_to_player_0 > result.distance )
          {
            if ( ( distance_to_player_0 < distance_to_player[ 1 ] )
                 && ( distance_to_player_0 < distance_to_player[ 2 ] )
                 && ( distance_to_player_0 < distance_to_player[ 3 ] ) )
              {
                result.distance = distance_to_player_0;
                result.p.x = x;
                result.p.y = y;
              }
          }
      }

  return result;
}

int main()
{
  int player_count;
  std::cin >> player_count;
  ++player_count;
  
  std::array< point, max_player_count > players =
    {
     point{ max_int, max_int },
     point{ max_int, max_int },
     point{ max_int, max_int },
     point{ max_int, max_int }
    };

  goal current_goal{ point{ -1, -1 }, -1 };
  
  while (1)
    {
      int round;
        
      std::cin >> round;
        
      for (int i( 0 ); i != player_count; ++i)
        {
          int opponent_rewind;
          std::cin >> players[ i ].x >> players[ i ].y >> opponent_rewind;
        }

      std::cin.ignore();

      terrain terrain;
      
      for ( int i( 0 ); i != rows; ++i )
        std::cin.read( terrain[ i ].data(), columns ).ignore();

      const goal new_goal
        ( furthest_reachable_free_cell( terrain, players, player_count ) );

      if ( ( current_goal.p == players[ 0 ] )
           || ( new_goal.distance > current_goal.distance ) )
        current_goal = new_goal;

      if ( current_goal.p.x < 0 )
        std::cout << players[ 0 ].x << ' ' << players[ 0 ].y << '\n';
      else
        std::cout << current_goal.p.x << ' ' << current_goal.p.y << '\n';
    }

  return 0;
}
