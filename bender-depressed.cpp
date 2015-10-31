#define NDEBUG
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

enum class circuit_state
  {
    off,
    normal,
    inverted
  };

enum class direction
  {
    north,
    south,
    east,
    west
  };

struct decision_state
{
  circuit_state circuit;
  direction forward;

  bool operator==( const decision_state& that ) const
  {
    return ( circuit == that.circuit ) && ( forward == that.forward );
  }
};

struct point
{
  std::size_t x;
  std::size_t y;

  bool operator==( const point& that ) const
  {
    return ( x == that.x ) && ( y == that.y );
  }
};

struct bender_state
{
  decision_state decision;
  point position;
  bool breaker_mode;
  bool blocked;

  bool operator==( const bender_state& that ) const
  {
    return ( decision == that.decision )
      && ( position == that.position )
      && ( breaker_mode == that.breaker_mode )
      && ( blocked == that.blocked );
  }
  
};

typedef std::vector< std::string > map_type;
typedef std::vector< std::vector< bender_state > > visited_type;

void invert_circuit( bender_state& bender )
{
  switch( bender.decision.circuit )
    {
    case circuit_state::normal:
      bender.decision.circuit = circuit_state::inverted;
      break;
    case circuit_state::inverted:
      bender.decision.circuit = circuit_state::normal;
      break;
    case circuit_state::off:
      break;
    }
}

void reset_visited( visited_type& visited )
{
  static bender_state state;
  state.decision.circuit = circuit_state::off;

  for ( auto& e : visited )
    std::fill( e.begin(), e.end(), state );
}

point position_in_direction( direction dir, const point& p )
{
  switch( dir )
    {
    case direction::north:
      return { p.x, p.y - 1 };
    case direction::south:
      return { p.x, p.y + 1 };
    case direction::east:
      return { p.x + 1, p.y };
    case direction::west:
      return { p.x - 1, p.y };
    default:
      {
        assert( false );
      }
    };
}

direction next_direction( direction dir, circuit_state circuit )
{
  if ( circuit == circuit_state::normal )
    switch( dir )
      {
      case direction::north:
        return direction::west;
      case direction::south:
        return direction::east;
      case direction::east:
        return direction::north;
      case direction::west:
        return direction::south;
      }
  else
    {
      assert( circuit == circuit_state::inverted );
      
      switch( dir )
        {
        case direction::north:
          return direction::east;
        case direction::south:
          return direction::west;
        case direction::east:
          return direction::south;
        case direction::west:
          return direction::north;
        }
    }

  assert( false );
}

direction first_direction( circuit_state circuit )
{
  if ( circuit == circuit_state::normal )
    return direction::south;

  assert( circuit == circuit_state::inverted );
  return direction::west;
}

bool next_position( map_type& map, bender_state& bender )
{
  direction dir
    ( bender.blocked
      ? first_direction( bender.decision.circuit )
      : bender.decision.forward );
  std::size_t guard( 5 );
  
  while( true )
    {
      assert( guard != 0 );
      --guard;
      
      const point next( position_in_direction( dir, bender.position ) );

      char& code( map[ next.y ][ next.x ] );
      
      if ( ( code == 'X' ) && bender.breaker_mode )
        {
          code = ' ';
          bender.decision.forward = dir;
          bender.position = next;
          bender.blocked = false;
          return true;
        }
      else if ( ( code != 'X' ) && ( code != '#' ) )
        {
          bender.decision.forward = dir;
          bender.position = next;
          bender.blocked = false;
          return false;
        }
      else
        {
          if ( bender.blocked )
            dir = next_direction( dir, bender.decision.circuit );
          else
            {
              bender.blocked = true;
              dir = first_direction( bender.decision.circuit );
            }
        }
    }
}

std::string to_string( direction dir )
{
  switch( dir )
    {
    case direction::north:
      return "NORTH";
    case direction::south:
      return "SOUTH";
    case direction::east:
      return "EAST";
    case direction::west:
      return "WEST";
    default:
      {
        assert( false );
        return "";
      }
    }
}

bool move_bender
( const std::vector< point >& teleport, map_type& map, bender_state& bender,
  std::vector< std::string >& path )
{
  const bool result( next_position( map, bender ) );

  path.push_back( to_string( bender.decision.forward ) );
  
  const char cell( map[ bender.position.y ][ bender.position.x ] );

  assert( cell != 'X' );
  assert( cell != '#' );
  
  switch( cell )
    {
    case 'I': invert_circuit( bender ); break;
    case 'B': bender.breaker_mode = !bender.breaker_mode; break;
    case 'T':
      {
        assert( teleport.size() == 2 );
        if ( bender.position == teleport[ 0 ] )
          bender.position = teleport[ 1 ];
        else
          bender.position = teleport[ 0 ];

        break;
      }
    case 'S': bender.decision.forward = direction::south; break;
    case 'E': bender.decision.forward = direction::east; break;
    case 'N': bender.decision.forward = direction::north; break;
    case 'W': bender.decision.forward = direction::west; break;
    case ' ': break;
    case '$': break;
    default:
      {
        assert( false );
      }
    };

  return result;
}

bool move
( const std::vector< point >& teleport, map_type& map, visited_type& visited,
  bender_state& bender, std::vector< std::string >& path )
{
  const bool reset( move_bender( teleport, map, bender, path ) );

  if ( reset )
    reset_visited( visited );

  if ( visited[ bender.position.y ][ bender.position.x ] == bender )
    return false;

  visited[ bender.position.y ][ bender.position.x ] = bender;
  
  return map[ bender.position.y ][ bender.position.x ] != '$';
}

int main()
{
  std::size_t rows;
  std::size_t columns;
  ( std::cin >> rows >> columns ).ignore();

  bender_state bender;
  bender.decision.circuit = circuit_state::normal;
  bender.decision.forward = direction::south;
  bender.breaker_mode = false;
  bender.blocked = false;
  
  std::vector< point > teleport;
  
  map_type map( rows );
  for ( std::size_t y( 0 ); y != rows; ++y )
    {
      std::string& row( map[ y ] );
      std::getline( std::cin, row );

      const auto bit( row.begin() );
      const auto eit( row.end() );
      
      auto it( std::find( bit, eit, '@' ) );

      if ( it != eit )
        {
          *it = ' ';
          bender.position.x = it - bit;
          bender.position.y = y;
        }

      for ( it = std::find( bit, eit, 'T' ); it != eit;
            it = std::find( it + 1, eit, 'T' ) )
        teleport.push_back( point{ std::size_t( it - bit ), y } );
    }

  visited_type visited( rows, visited_type::value_type( columns ) );
  reset_visited( visited );

  std::vector< std::string > path;
  
  while( move( teleport, map, visited, bender, path ) );

  if ( map[ bender.position.y ][ bender.position.x ] != '$' )
    std::cout << "LOOP\n";
  else
    for ( const auto& p : path )
      std::cout << p << '\n';
  
  return 0;
}
