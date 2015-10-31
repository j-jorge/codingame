#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

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

    std::vector< std::size_t > elevator_per_floor( floor_count );
    
    for ( int i( 0 ); i != elevator_count; ++i )
      {
        int elevator_floor;
        std::cin >> elevator_floor;
        ( std::cin >> elevator_per_floor[ elevator_floor ] ).ignore();
    }

    elevator_per_floor[ exit_floor ] = exit_position;
    
    while ( true )
      {
        int clone_floor;
        int clone_position;
        std::string direction;
        ( std::cin >> clone_floor >> clone_position >> direction ).ignore();

        if ( clone_floor == -1 )
          std::cout << "WAIT\n";
        else
          {
            assert( clone_floor < elevator_per_floor.size() );
            
            if ( ( ( direction == "RIGHT" )
                   && ( elevator_per_floor[ clone_floor ] < clone_position ) )
                 || ( ( direction == "LEFT" )
                      && ( elevator_per_floor[ clone_floor ]
                           > clone_position ) ) )
              std::cout << "BLOCK\n";
            else
              std::cout << "WAIT\n";
          }
      }

    return 0;
}
