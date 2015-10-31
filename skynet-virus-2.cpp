#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

class graph
{
public:
  explicit graph( std::size_t vertex_count )
    : m_vertex_count( vertex_count ),
      m_edges( vertex_count * vertex_count, false )
  {

  }

  std::size_t vertex_count() const
  {
    return m_vertex_count;
  }

  void add_edge( std::size_t v1, std::size_t v2 )
  {
    assert( m_edges[ edge_cell( v1, v2 ) ] == false );
    assert( m_edges[ edge_cell( v2, v1 ) ] == false );

    m_edges[ edge_cell( v1, v2 ) ] = true;
    m_edges[ edge_cell( v2, v1 ) ] = true;
  }

  void remove_edge( std::size_t v1, std::size_t v2 )
  {
    assert( m_edges[ edge_cell( v1, v2 ) ] == true );
    assert( m_edges[ edge_cell( v2, v1 ) ] == true );

    m_edges[ edge_cell( v1, v2 ) ] = false;
    m_edges[ edge_cell( v2, v1 ) ] = false;
  }
  
  std::vector< std::size_t > neighbors( std::size_t v ) const
  {
    std::vector< std::size_t > result;
    for ( std::size_t i( 0 ); i != m_vertex_count; ++i )
      if ( m_edges[ edge_cell( v, i ) ] )
        result.push_back( i );

    return result;
  }
  
  void print()
  {
    for ( std::size_t y( 0 ); y != m_vertex_count; ++y )
      {
        for ( std::size_t x( 0 ); x != m_vertex_count; ++x )
          std::cout << m_edges[ edge_cell( x, y ) ];

        std::cout << '\n';
      }
  }
  
private:

  std::size_t edge_cell( std::size_t v1, std::size_t v2 ) const
  {
    return v1 * m_vertex_count + v2;
  }
  
private:
  const std::size_t m_vertex_count;
  std::vector< bool > m_edges;
};

struct edge
{
  std::size_t start;
  std::size_t end;
};

edge pick_edge( const graph& g, std::size_t s, const std::vector<bool>& ends,
                const std::vector< std::size_t >& weights )
{
  const std::size_t vertex_count( g.vertex_count() );
  std::vector< edge > pending;
  std::vector< edge > result;
  std::vector< bool > seen( vertex_count, false );
  seen[ s ] = true;
  pending.push_back( { s, s } );

  std::vector< std::size_t > distances( vertex_count, 0 );
  
  while ( !pending.empty() )
    {
      std::vector< edge > next;
      
      for ( const edge& c : pending )
        for ( std::size_t n : g.neighbors( c.end ) )
          {
            if ( seen[ n ] )
              continue;

            assert( weights[ c.end ] <= distances[ c.end ] + 1 );
            distances[ n ] = distances[ c.end ] + 1 - weights[ c.end ];
            
            if ( ends[ n ] )
              {
                if ( c.end == s )
                  return { s, n };
                
                result.push_back( { c.end, n } );
              }

            seen[ n ] = true;
            next.push_back( { c.end, n } );
          }

      pending.swap( next );
    }

  std::sort
    ( result.begin(), result.end(),
      [ &weights, &distances ]( edge a, edge b ) -> bool
      {
        if ( weights[ a.start ] == weights[ b.start ] )
          return distances[ a.start ] < distances[ b.start ];

        return weights[ a.start ] > weights[ b.start ];
      } );

  assert( !result.empty() );
  return result.front();
}

int main()
{
  std::size_t vertex_count;
  std::size_t edge_count;
  std::size_t gateway_count;
  std::cin >> vertex_count >> edge_count >> gateway_count;

  graph g( vertex_count );

  for ( std::size_t i( 0 ); i != edge_count; ++i )
    {
      std::size_t s;
      std::size_t v;
      std::cin >> s >> v;
      g.add_edge( s, v );
    }

  std::vector< bool > gateway( gateway_count, false );
  std::vector< std::size_t > weights( vertex_count, 0 );
  
  for ( std::size_t i( 0 ); i != gateway_count; ++i )
    {
      std::size_t d;
      std::cin >> d;
      gateway[ d ] = true;

      for ( std::size_t n : g.neighbors( d ) )
        ++weights[ n ];
    }

  while( true )
    {
      std::size_t agent;
      std::cin >> agent;

      const edge e( pick_edge( g, agent, gateway, weights ) );
      g.remove_edge( e.start, e.end );
      --weights[ e.start ];

      std::cout << e.start << ' ' << e.end << '\n';
    }
  
  return 0;
}
