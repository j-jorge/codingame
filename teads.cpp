#define NDEBUG
#pragma GCC optimize ("-O3")

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <set>
#include <sstream>

class graph
{
public:
  explicit graph( std::size_t vertex_count )
    : m_vertex_count( vertex_count ),
      m_neighbors( vertex_count )
  {
    for ( auto& n : m_neighbors )
      n.reserve( vertex_count );
  }

  std::size_t vertex_count() const
  {
    return m_vertex_count;
  }

  void add_edge( std::size_t v1, std::size_t v2 )
  {
    m_neighbors[ v1 ].push_back( v2 );
    m_neighbors[ v2 ].push_back( v1 );
  }

  const std::vector< std::size_t >& neighbors( std::size_t v ) const
  {
    assert( v < m_neighbors.size() );
    return m_neighbors[ v ];
  }
  
private:

  std::size_t edge_cell( std::size_t v1, std::size_t v2 ) const
  {
    return v1 * m_vertex_count + v2;
  }
  
private:
  const std::size_t m_vertex_count;
  std::vector< std::vector< std::size_t > > m_neighbors;
};

struct edge
{
  std::size_t start;
  std::size_t end;
};

class shortest_path
{
public:
  explicit shortest_path( const graph& g )
    : m_g( g ),
      m_vertex_count( g.vertex_count() ),
      m_result( m_vertex_count ),
      m_seen( m_vertex_count )
  {
    m_pending.reserve( m_vertex_count );
    m_next.reserve( m_vertex_count );
  }
  
  const std::vector< std::size_t >& operator()( std::size_t start )
  {
    std::fill( m_result.begin(), m_result.end(), m_vertex_count );
    m_result[ start ] = 0;

    std::fill( m_seen.begin(), m_seen.end(), false );
    m_seen[ start ] = true;

    m_pending.resize( 0 );
    m_pending.push_back( start );

    while ( !m_pending.empty() )
      {
        m_next.resize( 0 );

        for ( const std::size_t& s : m_pending )
          for ( const std::size_t& v : m_g.neighbors( s ) )
            {
              if ( !m_seen[ v ] )
                {
                  m_seen[ v ] = true;
                  m_next.push_back( v );
              }
                
              const std::size_t d( m_result[ s ] + 1 );
              if ( d < m_result[ v ] )
                m_result[ v ] = d;
            }
        
        m_pending.swap( m_next );
      }

    return m_result;
  }

private:
  const graph& m_g;
  const std::size_t m_vertex_count;
  std::vector< std::size_t > m_result;
  std::vector< bool > m_seen;
  std::vector< std::size_t > m_pending;
  std::vector< std::size_t > m_next;
};

std::size_t depth( const graph& g, std::size_t start, std::size_t n )
{
  std::size_t result( 0 );

  std::vector< bool > seen( g.vertex_count(), false );
  seen[ start ] = true;
  seen[ n ] = true;
  
  std::vector< std::size_t > pending = { n };
  pending.reserve( g.vertex_count() );

  std::vector< std::size_t > next;
  next.reserve( g.vertex_count() );
  
  while ( !pending.empty() )
    {
      ++result;
      next.resize( 0 );

      for ( const std::size_t& v : pending )
        for ( const std::size_t& e : g.neighbors( v ) )
          if ( !seen[ e ] )
            {
              next.push_back( e );
              seen[ e ] = true;
            }

      pending.swap( next );
    }

  return result;
}

int main()
{
  std::size_t edge_count;
  std::cin >> edge_count;

  std::vector< edge > edges( edge_count );
  std::size_t vertex_count( 0 );
  
  for ( edge& e : edges )
    {
      std::cin >> e.start >> e.end;

      if ( e.start >= vertex_count )
        vertex_count = e.start + 1;

      if ( e.end >= vertex_count )
        vertex_count = e.end + 1;
    }

  graph g( vertex_count );

  for ( const edge& e : edges )
    g.add_edge( e.start, e.end );

  std::vector< std::size_t > d( vertex_count, 0 );

  for ( const std::size_t& n : g.neighbors( 0 ) )
    d[ n ] = depth( g, 0, n );

  std::sort( d.begin(), d.end() );
  const std::size_t diameter( *d.rbegin() + *(++d.rbegin()) );

  if ( diameter & 1 )
    std::cout << diameter / 2 + 1 << '\n';
  else
    std::cout << diameter / 2 << '\n';

  return 0;
}
