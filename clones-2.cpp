#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

struct range
{
  int left;
  int right;
};

range
expand( int width, const std::vector< std::size_t >& elevators, const range& r )
{
  if ( elevators.empty() )
    return { 0, width - 1 };
  
  int left( 0 );

  for ( std::size_t i( 0 ); i != elevators.size(); ++i )
    if ( elevators[ i ] < r.left )
      left = elevators[ i ] + 1;
    else
      break;

  int right( width - 1 );
  
  for ( std::size_t i( elevators.size() ); i != 0; )
    {
      --i;
      if ( elevators[ i ] > r.right )
        right = elevators[ i ] - 1;
      else
        break;
    }

  return { left, right };
}

bool region_contains_elevator
( const range& r, const std::vector< std::size_t >& elevators )
{
  for ( const std::size_t& e : elevators )
    if ( ( r.left <= e ) && ( e <= r.right ) )
      return true;

  return false;
}

enum class action
{
  wait,
  block,
  elevator
};

int main()
{
    int floor_count;
    int width;
    int round_count;
    int exit_floor;
    int exit_position;
    int clone_count;
    int additional_elevators;
    int elevator_count;

    ( std::cin >> floor_count >> width >> round_count >> exit_floor
      >> exit_position >> clone_count >> additional_elevators
      >> elevator_count ).ignore();

    std::vector< std::vector< std::size_t > > elevator_per_floor( floor_count );
    
    for ( int i( 0 ); i != elevator_count; ++i )
      {
        int elevator_floor;
        int elevator_position;
        ( std::cin >> elevator_floor >> elevator_position ).ignore();
        elevator_per_floor[ elevator_floor ].push_back( elevator_position );
    }

    elevator_per_floor[ exit_floor ].push_back( exit_position );
    
    for ( auto& e : elevator_per_floor )
      std::sort( e.begin(), e.end() );

    std::vector< bool > needed_elevator( floor_count );
    
    std::vector< range > green_ranges( floor_count );

    range r
      ( expand
        ( width, elevator_per_floor[ exit_floor ],
          { exit_position, exit_position } ) );
    green_ranges[ exit_floor ] = r;
    
    for ( int f( exit_floor - 1 ); f >= 0; --f )
      {
        green_ranges[ f ] = r;
        needed_elevator[ f ] =
          !region_contains_elevator( r, elevator_per_floor[ f ] );
        r = expand( width, elevator_per_floor[ f ], r );
      }

    while ( true )
      {
        int clone_floor;
        int clone_position;
        std::string direction;
        ( std::cin >> clone_floor >> clone_position >> direction ).ignore();

        action a;
        
        if ( clone_floor == -1 )
          a = action::wait;
        else
          {
            assert( clone_floor < elevator_per_floor.size() );
            assert( clone_floor < green_ranges.size() );
            auto& elevators( elevator_per_floor[ clone_floor ] );
            const range& r( green_ranges[ clone_floor ] );
            
            if ( clone_position < r.left )
              {
                if ( direction == "LEFT" )
                  a = action::block;
              }
            else if ( r.right < clone_position )
              {
                if ( direction == "RIGHT" )
                  a = action::block;
              }
            else if ( needed_elevator[ clone_floor ] )
              a = action::elevator;
            else if ( ( direction == "LEFT" )
                      && !region_contains_elevator
                      ( { r.left, clone_position }, elevators ) )
              a = action::block;
            else if ( ( direction == "RIGHT" )
                      && !region_contains_elevator
                      ( { clone_position, r.right }, elevators ) )
              a = action::block;
            else
              a = action::wait;
          }

        switch ( a )
          {
          case action::wait:
            std::cout << "WAIT\n";
            break;
          case action::elevator:
            std::cout << "ELEVATOR\n";
            elevators.push_back( clone_position );
            needed_elevator[ clone_floor ] = false;
            --clone_count;
            break;
          case action::block:
            std::cout << "BLOCK\n";
            --clone_count;
            break;
          default:
            assert( false );
          }
      }

    return 0;
}
