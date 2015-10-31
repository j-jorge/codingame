#define NDEBUG

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <list>

static const double pi( 3.14159265359 );

inline void check( bool result )
{
  assert( result );
}

class graph
{
public:
  explicit graph( std::size_t vertex_count )
    : m_vertex_count( vertex_count ),
      m_neighbors( vertex_count ),
      m_sources( vertex_count ),
      m_distances( vertex_count, std::vector< double >( vertex_count, 0 ) )
  {
    for ( auto& n : m_neighbors )
      n.reserve( vertex_count );

    for ( auto& n : m_sources )
      n.reserve( vertex_count );
  }

  std::size_t vertex_count() const
  {
    return m_vertex_count;
  }

  void add_edge( std::size_t v1, std::size_t v2, double distance )
  {
    assert( v1 != v2 );
    assert( distance != 0 );
    assert( std::find( m_neighbors[ v1 ].begin(), m_neighbors[ v1 ].end(), v2 )
            == m_neighbors[ v1 ].end() );
    
    m_neighbors[ v1 ].push_back( v2 );
    m_sources[ v2 ].push_back( v1 );
    m_distances[ v1 ][ v2 ] = distance;
  }

  double distance( std::size_t v1, std::size_t v2 ) const
  {
    return m_distances[ v1 ][ v2 ];
  }
  
  const std::vector< std::size_t >& neighbors( std::size_t v ) const
  {
    assert( v < m_neighbors.size() );
    return m_neighbors[ v ];
  }

  const std::vector< std::size_t >& sources( std::size_t v ) const
  {
    assert( v < m_sources.size() );
    return m_sources[ v ];
  }
  
private:
  const std::size_t m_vertex_count;
  std::vector< std::vector< std::size_t > > m_neighbors;
  std::vector< std::vector< std::size_t > > m_sources;
  std::vector< std::vector< double > > m_distances;
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
      m_distances( m_vertex_count ),
      m_seen( m_vertex_count ),
      m_paths( m_vertex_count )
  {
    m_pending.reserve( m_vertex_count );

    for ( auto& p : m_paths )
      p.reserve( m_vertex_count );
  }
  
  const std::vector< std::size_t >& operator()
    ( std::size_t start, const std::size_t end )
  {
    const double upper_bound( std::numeric_limits< double >::max() );
    std::fill( m_distances.begin(), m_distances.end(), upper_bound );
    m_distances[ start ] = 0;

    std::fill( m_seen.begin(), m_seen.end(), false );
    m_seen[ start ] = true;

    m_pending.push_back( start );

    auto queue_order
      ( [ this ]( std::size_t a, std::size_t b ) -> bool
        {
          return m_distances[ a ] > m_distances[ b ];
        } );
      
    while ( !m_seen[ end ] && !m_pending.empty() )
      {
        std::size_t s( m_pending.back() );
        m_pending.pop_back();
        
        for ( const std::size_t& v : m_g.neighbors( s ) )
          {
            const double d( m_distances[ s ] + m_g.distance( s, v ) );
            if ( d < m_distances[ v ] )
              {
                m_paths[ v ] = m_paths[ s ];
                m_paths[ v ].push_back( s );
                m_distances[ v ] = d;
              }

            if ( !m_seen[ v ] )
              {
                m_seen[ v ] = true;
                m_pending.insert
                  ( std::lower_bound
                    ( m_pending.begin(), m_pending.end(), v, queue_order ),
                    v );
              }
          }
      }

    if ( m_seen[ end ] )
      m_paths[ end ].push_back( end );
    
    return m_paths[ end ];
  }

private:
  const graph& m_g;
  const std::size_t m_vertex_count;
  std::vector< double > m_distances;
  std::vector< bool > m_seen;
  std::vector< std::size_t > m_pending;
  std::vector< std::vector< std::size_t > > m_paths;
};

struct stop
{
  std::string id;
  std::string name;
  double latitude;
  double longitude;
};

double distance( const stop& a, const stop& b )
{
  const double x
    ( ( b.longitude - a.longitude )
      * std::cos( ( a.latitude + b.latitude ) / 2 ) );
  const double y( b.latitude - a.latitude );

  return std::sqrt( x * x + y * y ) * 6371;
}

std::size_t
stop_vertex( const std::vector< stop >& stops, const std::string& id )
{
  const auto bit( stops.begin() );
  const auto eit( stops.end() );
  const stop key = { id };
  
  auto it
    ( std::lower_bound
      ( bit, eit, key,
        []( const stop& a, const stop& b ) -> bool
        {
          return a.id < b.id;
        } ) );
  
  assert( it != eit );
  assert( it->id == id );
  return it - bit;
}

int main()
{
  std::string start_id;
  check( std::getline( std::cin, start_id ) );
  
  std::string end_id;
  check( std::getline( std::cin, end_id ) );

  std::size_t stop_count;
  check( ( std::cin >> stop_count ).ignore() );

  std::vector< stop > stops( stop_count );
  std::string trash;
  
  for ( std::size_t i( 0 ); i != stop_count; ++i )
    {
      stop& s( stops[ i ] );
      check( std::getline( std::cin, s.id, ',' ) );
      check( std::getline( std::cin, trash, '"' ) );
      check( std::getline( std::cin, s.name, '"' ) );
      check( std::getline( std::cin, trash, ',' ) );
      check( std::getline( std::cin, trash, ',' ) );
      check( std::cin >> s.latitude );
      check( std::getline( std::cin, trash, ',' ) );
      check( std::cin >> s.longitude );
      check( std::getline( std::cin, trash ) );

      s.latitude = pi * s.latitude / 180;
      s.longitude = pi * s.longitude / 180;
    }

  std::sort
    ( stops.begin(), stops.end(),
      []( const stop& a, const stop& b ) -> bool
      {
        return a.id < b.id;
      } );
  
  graph g( stop_count );
  
  std::size_t edge_count;
  for ( check( std::cin >> edge_count ); edge_count != 0; --edge_count )
    {
      std::string edge_start_id;
      std::string edge_end_id;
      check( std::cin >> edge_start_id >> edge_end_id );

      if ( edge_start_id == edge_end_id )
        continue;

      const std::size_t start( stop_vertex( stops, edge_start_id ) );
      const std::size_t end( stop_vertex( stops, edge_end_id ) );
      g.add_edge( start, end, distance( stops[ start ], stops[ end ] ) );
    }

  shortest_path s( g );
  const std::vector< std::size_t >& path
    ( s( stop_vertex( stops, start_id ), stop_vertex( stops, end_id ) ) );

  if ( path.empty() )
    std::cout << "IMPOSSIBLE\n";
  else
    {
      assert( path.back() == stop_vertex( stops, end_id ) );
      assert( path.front() == stop_vertex( stops, start_id ) );
      for ( std::size_t v : path )
        std::cout << stops[ v ].name << '\n';
    }

  return 0;
}
