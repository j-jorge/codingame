#include <iostream>
#include <vector>

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
    m_edges[ edge_cell( v1, v2 ) ] = true;
    m_edges[ edge_cell( v2, v1 ) ] = true;
  }

  void remove_edge( std::size_t v1, std::size_t v2 )
  {
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

std::size_t
pick_neighbor( const graph& g, std::size_t s, const std::vector<bool>& ends )
{
  struct candidate
  {
    std::size_t vertex;
    std::size_t start;
  };

  std::vector< candidate > pending;
  std::vector< bool > seen( g.vertex_count(), false );
  seen[ s ] = true;
  
  for ( std::size_t n : g.neighbors( s ) )
    {
      if ( ends[ n ] )
        return n;

      if ( seen[ n ] )
        continue;

      pending.push_back( { n, n } );
      seen[ n ] = true;
    }

  while ( !pending.empty() )
    {
      const candidate c( pending.front() );
      pending.erase( pending.begin() );

      for ( std::size_t n : g.neighbors( c.vertex ) )
        {
          if ( ends[ n ] )
            return c.start;

          if ( seen[ n ] )
            continue;

          seen[ n ] = true;
          pending.push_back( { n, c.start } );
        }
    }

  return -1;
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

  for ( std::size_t i( 0 ); i != gateway_count; ++i )
    {
      std::size_t g;
      std::cin >> g;
      gateway[ g ] = true;
    }

  while( true )
    {
      std::size_t agent;
      std::cin >> agent;

      const std::size_t n( pick_neighbor( g, agent, gateway ) );
      g.remove_edge( agent, n );

      std::cout << agent << ' ' << n << '\n';
    }
  
  return 0;
}
