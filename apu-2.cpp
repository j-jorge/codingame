#ifdef AT_HOME
#define _DEBUG
#define LOGS( e ) std::clog << e
#define IF_DEBUG( e ) e
#else
#define NDEBUG
#define LOGS( e )
#define IF_DEBUG( e )
#endif

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>


struct node
{
  node( std::uint8_t x, std::uint8_t y, std::uint8_t c )
    : x( x ),
      y( y ),
      card( c ),
      initial_cardinality( c ),
      neighbor_count( 0 )
  {}

  bool two_guaranteed( const std::vector< node >& nodes ) const
  {
    std::size_t neighborhood_cardinality( 0 );
    
    for ( std::size_t i( 0 ); i != neighbor_count; ++i )
      neighborhood_cardinality +=
        std::min< std::size_t >
        ( 2, nodes[ neighbors[ i ] ].initial_cardinality );
    
    return card == neighborhood_cardinality;
  }

  bool single_edge( const std::vector< node >& nodes ) const
  {
    if ( card != 1 )
      return false;

    std::size_t non_unary_neighbors( 0 );
    for ( std::size_t i( 0 ); i != neighbor_count; ++i )
      if ( nodes[ neighbors[ i ] ].card != 1 )
        ++non_unary_neighbors;
    
    return non_unary_neighbors == 1;
  }
  
  void add_neighbor( std::size_t index, const node& n )
  {
    assert( neighbor_count < neighbors.size() );
    assert
      ( std::find
        ( neighbors.begin(), neighbors.begin() + neighbor_count, index )
        == neighbors.begin() + neighbor_count );

    neighbors[ neighbor_count ] = index;
    remaining_links[ neighbor_count ] = std::min< std::size_t >( 2, n.card );
    
    ++neighbor_count;
  }
  
  void erase_neighbor( std::size_t n )
  {
    assert( neighbor_count != 0 );
    
    for ( std::size_t i( 0 ); i != neighbor_count; ++i )
      if ( neighbors[ i ] == n )
        {
          neighbors[ i ] = neighbors[ neighbor_count - 1 ];
          remaining_links[ i ] = remaining_links[ neighbor_count - 1 ];
          --neighbor_count;
          return;
        }
  }

  void set_one_guaranteed( const std::vector< node >& nodes )
  {
    std::size_t neighborhood_cardinality( 0 );
    
    for ( std::size_t i( 0 ); i != neighbor_count; ++i )
      {
        const std::size_t c
          ( nodes[ neighbors[ i ] ].initial_cardinality );

        if ( c == 1 )
          {
            one_guaranteed = false;
            return;
          }
        
        neighborhood_cardinality += 2;
      }
    
    one_guaranteed = ( initial_cardinality == neighborhood_cardinality - 1 );
  }

  std::uint8_t x;
  std::uint8_t y;
  std::uint8_t card;
  const std::uint8_t initial_cardinality;
  std::uint8_t neighbor_count;
  std::array< std::size_t, 4 > neighbors;
  std::array< unsigned char, 4 > remaining_links;
  bool one_guaranteed;
};

bool full( const std::vector< node >& nodes, const node& n )
{
  if ( n.single_edge( nodes ) )
    return true;
  
  if ( n.two_guaranteed( nodes ) )
    return true;
  
  std::size_t neighbor_cardinality( 0 );

  for ( std::size_t i( 0 ); i != n.neighbor_count; ++i )
    neighbor_cardinality +=
      std::min< std::size_t >
      ( n.remaining_links[ i ], nodes[ n.neighbors[ i ] ].card );

  return neighbor_cardinality == n.card;
}

bool compare( const std::vector< node >& nodes, const node& a, const node& b )
{
  return std::make_tuple
    ( !full( nodes, a ), !a.one_guaranteed, -int( a.card ), a.x, a.y )
    < std::make_tuple
      ( !full( nodes, b ), !b.one_guaranteed, -int( b.card ), b.x, b.y );
}

struct edge
{
  edge( std::size_t a, std::size_t b )
    : s( a ),
      t( b )
  {
    if ( t < s )
      std::swap( s, t );
  }

  bool operator<( const edge& that ) const
  {
    if ( s == that.s )
      return t < that.t;

    return s < that.s;
  }
  
  std::size_t s;
  std::size_t t;
};

void sort_queue
( const std::vector< node >& nodes, std::vector< std::size_t >& queue )
{
  std::sort
    ( queue.begin(), queue.end(),
      [ &nodes ]( std::size_t a, std::size_t b ) -> bool
      {
        assert( a < nodes.size() );
        assert( b < nodes.size() );
        return !compare( nodes, nodes[ a ], nodes[ b ] );
      } );
}

void print_queue
( const std::vector< node >& nodes, std::vector< std::size_t >& queue )
{
  std::clog << "queue:";
  for ( std::size_t i : queue )
    std::clog << ' ' << i << '[' << (int)nodes[ i ].card << ']';
  std::clog << '\n';
}

static const std::size_t g_free_cell
  ( std::numeric_limits< std::size_t >::max() );

void print_grid( const std::vector< std::vector< std::size_t > >& rows )
{
  const std::size_t h( rows.size() );
  const std::size_t w( rows[ 0 ].size() );

  for ( std::size_t y( 0 ); y != h; ++y )
    {
      for ( std::size_t x( 0 ); x != w; ++x )
        if ( rows[ y ][ x ] == g_free_cell )
          std::clog << '.';
        else
          std::clog << rows[ y ][ x ];

      std::clog << '\n';
    }
}

void print_cardinality
( const std::vector< node >& nodes,
  const std::vector< std::vector< std::size_t > >& rows )
{
  const std::size_t h( rows.size() );
  const std::size_t w( rows[ 0 ].size() );

  for ( std::size_t y( 0 ); y != h; ++y )
    {
      for ( std::size_t x( 0 ); x != w; ++x )
        if ( rows[ y ][ x ] == g_free_cell )
          std::clog << "    ";
        else
          std::clog << '[' << std::setfill( ' ' ) << std::setw( 2 )
                    << (int)nodes[rows[ y ][ x ] ].card << ']';

      std::clog << '\n';
    }
}

void read_nodes
( std::vector< node >& nodes, std::vector< std::vector< std::size_t > >& rows,
  std::vector< std::vector< std::size_t > >& columns )
{
  const std::size_t h( rows.size() );
  const std::size_t w( rows[ 0 ].size() );
  
  std::string line;

  for ( std::size_t y( 0 ); y != h; ++y )
    {
      std::getline( std::cin, line );
      for ( std::size_t x( 0 ); x != w; ++x )
        if ( line[ x ] != '.' )
          {
            rows[ y ][ x ] = columns[ x ][ y ] = nodes.size();
            nodes.push_back( node( x, y, line[ x ] - '0' ) );
          }
    }
}

std::map< edge, std::size_t > find_neighbors
( std::vector< node >& nodes,
  const std::vector< std::vector< std::size_t > >& rows,
  const std::vector< std::vector< std::size_t > >& columns )
{
  const std::size_t h( rows.size() );
  const std::size_t w( rows[ 0 ].size() );

  std::map< edge, std::size_t > result;

  auto not_free
    ( []( std::size_t i ) -> bool
      {
        return i != g_free_cell;
      } );
  
  for ( std::size_t y( 0 ); y != h; ++y )
    for ( std::size_t x( 0 ); x != w; ++x )
      if ( not_free( rows[ y ][ x ] ) )
        {
          const std::size_t s( rows[ y ][ x ] );

          const auto it_row( rows[ y ].begin() + x + 1 );
          const auto row_end( rows[ y ].end() );

          auto it( std::find_if( it_row, row_end, not_free ) );
          
          if ( it != row_end )
            {
              nodes[ s ].add_neighbor( *it, nodes[ *it ] );
              nodes[ *it ].add_neighbor( s, nodes[ s ] );
              result[ edge( s, *it ) ] = 0;
            }

          const auto it_column( columns[ x ].begin() + y + 1 );
          const auto column_end( columns[ x ].end() );

          it = std::find_if( it_column, column_end, not_free );
  
          if ( it != column_end )
            {
              nodes[ s ].add_neighbor( *it, nodes[ *it ] );
              nodes[ *it ].add_neighbor( s, nodes[ s ] );
              result[ edge( s, *it ) ] = 0;
            }
        }

  return result;
}

void print_neighbors( const std::vector< node >& nodes )
{
  for ( std::size_t i( 0 ); i != nodes.size(); ++i )
    {
      std::clog << i
                << ": full? " << full( nodes, nodes[ i ] )
                << ": one? " << nodes[ i ].one_guaranteed
                << ':';
      
      for( std::size_t j( 0 ); j != nodes[ i ].neighbor_count; ++j )
        std::clog << ' ' << nodes[ i ].neighbors[ j ];
      std::clog << '\n';
    }
}

std::vector< std::size_t > initialize_queue( std::size_t n )
{
  std::vector< std::size_t > queue( n );
  std::size_t i( 0 );
  std::generate
    ( queue.begin(), queue.end(),
      [ &i ]() -> std::size_t
      {
        return i++;
      } );

  return queue;
}

void edges_for_single
( std::map< edge, std::size_t >& result, std::vector< node >& nodes,
  std::size_t i )
{
  node& n( nodes[ i ] );
  assert( n.single_edge( nodes ) );
  
  n.card = 0;

  for ( std::size_t j( 0 ); j != n.neighbor_count; ++j )
    {
      const std::size_t neighbor( n.neighbors[ j ] );

      if ( nodes[ neighbor ].card != 1 )
        {
          result[ edge( i, neighbor ) ] = 1;
          --nodes[ neighbor ].card;
          return;
        }
    }

  assert( false );
}

void edges_for_full
( std::map< edge, std::size_t >& result, std::vector< node >& nodes,
  std::size_t i )
{
  node& n( nodes[ i ] );

  assert( full( nodes, n ) );
  
  for ( std::size_t j( 0 ); j != n.neighbor_count; ++j )
    {
      const std::size_t neighbor( n.neighbors[ j ] );
      LOGS( "neighbor " << neighbor << ' ' << result[ edge( i, neighbor ) ]
            << '\n' );
                
      assert( nodes[ neighbor ].card > 0 );
      const std::size_t c
        ( std::min( nodes[ neighbor ].card, n.remaining_links[ j ] ) );
      result[ edge( i, neighbor ) ] += c;
                
      assert( nodes[ neighbor ].card >= c );
      nodes[ neighbor ].card -= c;
      
      assert( n.card >= c );
      n.card -= c;
    }
  
  assert( n.card == 0 );
}

void edges_for_one_guaranteed
( std::map< edge, std::size_t >& result, std::vector< node >& nodes,
  std::size_t i )
{
  node& n( nodes[ i ] );

  assert( n.one_guaranteed );
  assert( n.card >= 1 );
  
  for ( std::size_t j( 0 ); j != n.neighbor_count; ++j )
    {
      const std::size_t neighbor( n.neighbors[ j ] );

      if ( result[ edge( i, neighbor ) ] != 0 )
        continue;
      
      ++result[ edge( i, neighbor ) ];
            
      assert( nodes[ neighbor ].card >= 1 );
      --nodes[ neighbor ].card;

      for ( std::size_t k( 0 ); k != nodes[ neighbor ].neighbor_count; ++k )
        if ( nodes[ neighbor ].neighbors[ k ] == i )
          {
            --nodes[ neighbor ].remaining_links[ k ];
            break;
          }

      assert( n.card >= 1 );
      --n.card;
      --n.remaining_links[ j ];
    }

  n.one_guaranteed = false;
}

void edges_random
( std::map< edge, std::size_t >& result, std::vector< node >& nodes,
  std::size_t i )
{
  node& n( nodes[ i ] );
  std::size_t neighbor_index( n.neighbor_count );
  std::size_t neighbor_card( 0 );
  
  for ( std::size_t j( 0 ); j != n.neighbor_count; ++j )
    {
      if ( n.remaining_links[ j ] == 0 )
        continue;

      const std::size_t neighbor( n.neighbors[ j ] );

      if ( nodes[ neighbor ].card > neighbor_card )
        neighbor_index = j;
    }

  assert( neighbor_index != n.neighbor_count );

  const std::size_t neighbor( n.neighbors[ neighbor_index ] );
  ++result[ edge( i, neighbor ) ];
  --nodes[ neighbor ].card;

  for ( std::size_t j( 0 ); j != nodes[ neighbor ].neighbor_count; ++j )
    if ( nodes[ neighbor ].neighbors[ j ] == i )
      {
        --nodes[ neighbor ].remaining_links[ j ];
        break;
      }
      
  assert( n.card >= 1 );
  --n.card;
  --n.remaining_links[ neighbor_index ];
}

void clean_queue
( std::vector< node >& nodes, std::vector< std::size_t >& queue )
{
  std::vector< std::size_t > rm;
  rm.reserve( queue.size() );
      
  for ( auto it( queue.begin() ); it != queue.end(); )
    if ( nodes[ *it ].card == 0 )
      {
        rm.push_back( *it );
        it = queue.erase( it );
      }
    else
      ++it;
      
  for ( std::size_t i : rm )
    {
      node& n( nodes[ i ] );

      LOGS( "remove node " << i << '[' << (int)n.card << ']' << '\n' );
      
      for ( std::size_t j( 0 ); j != n.neighbor_count;  )
        {
          const std::size_t neighbor( n.neighbors[ j ] );
          
          LOGS( "erase " << i << " from " << neighbor << '\n' );
          
          nodes[ neighbor ].erase_neighbor( i );
          n.erase_neighbor( neighbor );
        }
    }
}

int main()
{
  std::size_t w;
  std::size_t h;
  ( std::cin >> w >> h ).ignore();

  std::vector< node > nodes;
  nodes.reserve( w * h );

  std::vector< std::vector< std::size_t > > rows
    ( h, std::vector< std::size_t >( w, g_free_cell ) );
  std::vector< std::vector< std::size_t > > columns
    ( w, std::vector< std::size_t >( h, g_free_cell ) );

  read_nodes( nodes, rows, columns );
  IF_DEBUG( print_grid( rows ) );
  
  std::map< edge, std::size_t > result
    ( find_neighbors( nodes, rows, columns ) );

  IF_DEBUG( print_neighbors( nodes ) );
  
  for ( node& n : nodes )
    n.set_one_guaranteed( nodes );
  
  std::vector< std::size_t > queue( initialize_queue( nodes.size() ) );
  sort_queue( nodes, queue );

  IF_DEBUG( print_cardinality( nodes, rows ) );
  
  while( !queue.empty() )
    {
      IF_DEBUG( print_queue( nodes, queue ) );
      
      const std::size_t i( queue.back() );
      node& n( nodes[ i ] );
      LOGS( "node " << i << ' ' << n.two_guaranteed( nodes ) << ' '
            << n.one_guaranteed << ' ' << (int)n.x << ' '
            << (int)n.y << '\n' );

      if ( n.single_edge( nodes ) )
        edges_for_single( result, nodes, i );
      else if ( full( nodes, n ) )
        edges_for_full( result, nodes, i );
      else if ( n.one_guaranteed )
        edges_for_one_guaranteed( result, nodes, i );
      else
        edges_random( result, nodes, i );
      
      IF_DEBUG( print_cardinality( nodes, rows ) );
      
      clean_queue( nodes, queue );
      sort_queue( nodes, queue );
    }

  LOGS( result.size() << " potential edges\n" );
  
  for ( const auto& e : result )
    if( e.second != 0 )
      {
        std::cout << (std::size_t)nodes[ e.first.s ].x << ' '
                  << (std::size_t)nodes[ e.first.s ].y << ' '
                  << (std::size_t)nodes[ e.first.t ].x << ' '
                  << (std::size_t)nodes[ e.first.t ].y << ' '
                  << e.second << '\n';
      }
  
  return 0;
}
