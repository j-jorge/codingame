#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <string>

#ifdef AT_HOME
#define DBG(x) x
#else
#define DBG(x) do {} while( false )
#endif

static constexpr bool black( true );
static constexpr bool white( false );

class image
{
public:
  typedef std::vector< bool >::iterator iterator;
  typedef std::vector< bool >::const_reference const_reference;
  typedef std::vector< bool >::reference reference;
  
public:
  image( std::size_t w, std::size_t h )
    : m_width( w ),
      m_height( h ),
      m_pixels( w * h )
  {

  }

  iterator begin()
  {
    return m_pixels.begin();
  }

  std::size_t width() const
  {
    return m_width;
  }

  std::size_t height() const
  {
    return m_height;
  }

  const_reference operator()( std::size_t x, std::size_t y ) const
  {
    return m_pixels[ y * m_width + x ];
  }
  
  reference operator()( std::size_t x, std::size_t y )
  {
    return m_pixels[ y * m_width + x ];
  }
  
  void dump()
  {
    std::ofstream f( "output.data", std::ios::binary | std::ios::out );
    for ( auto it( m_pixels.begin() ); it != m_pixels.end(); ++it )
      if ( *it == white )
        f.put( 255 );
      else
        f.put( 0 );
  }
  
private:
  std::size_t m_width;
  std::size_t m_height;
  std::vector< bool > m_pixels;
};

struct staff
{
  std::size_t left;
  std::size_t top;
  std::size_t thickness;
  std::size_t gap;
  std::size_t note_width;
};

staff find_staff( const image& img )
{
  const std::size_t w( img.width() );
  const std::size_t h( img.height() );
  
  for( std::size_t x( 0 ); x != w ; ++x )
    for ( std::size_t y( 0 ); y != h; ++y )
      if ( img( x, y ) == black )
        {
          staff result;
          result.left = x;
          result.top = y;
          
          while( img( x, y ) == black )
            ++y;

          result.thickness = y - result.top;
          
          const std::size_t top( y );
          
          while( img( x, y ) == white )
            ++y;

          result.gap = y - top;
          result.note_width = result.gap + 2;
          return result;
        }

  assert( false );
  return {};
}

void scan
( const image& img, const staff& s, std::size_t y,
  const char* const notes, std::map< std::size_t, std::string >& result )
{
  const std::size_t half_gap( s.gap / 2 );
  const std::size_t w( img.width() );
  
  for ( const char* n( notes ); *n != 0; ++n )
    {
      DBG( std::cerr << "scan " << *n << " at y=" << y << '\n' );
      
      for ( std::size_t x( s.left ); x != w; ++x )
        if ( img( x, y ) == black )
          {
            if ( ( img( x, y + s.thickness + 1 ) == black )
                 && ( img( x + s.note_width - 1, y ) == black ) )
              {
                if ( img( x + half_gap, y ) == black )
                  {
                    DBG( std::cerr << "* b/" << *n << " at " << x << ", " << y << '\n');
                    result[ x ] = *n + std::string( "Q" );
                  }
                else
                  {
                    DBG( std::cerr << "* w/" << *n << " at " << x << ", " << y << '\n');
                    result[ x ] = *n + std::string( "H" );
                  }
                
                x += s.note_width;
              }
            else
              {
                for ( ++x; img( x, y ) == black; ++x );
                --x;
              }
          }

      y += s.gap + s.thickness;
    }
}

image scale_down( const image& source, std::size_t d )
{
  const std::size_t w( source.width() / d );
  const std::size_t h( source.height() / d );
  
  image result( w, h );

  for ( std::size_t y( 0 ); y != h; ++y )
    for ( std::size_t x( 0 ); x != w; ++x )
      result( x, y ) = source( x * d, y * d );

  return result;
}

int main()
{
  std::size_t w, h;
  ( std::cin >> w >> h ).ignore();
  image img( w, h );

  std::string color;
  std::size_t count;
  auto it( img.begin() );

  while( std::cin >> color >> count )
    {
      const bool c( color == "B" );

      do
        {
          *it = c;
          ++it;
          --count;
        }
      while( count != 0 );
    }

  staff s( find_staff( img ) );

  std::cerr << "size is " << img.width() << "×" << img.height() << '\n';
  std::cerr << "staff starts at " << s.left << ", " << s.top << '\n';
  std::cerr << "line width is " << s.thickness << '\n';
  std::cerr << "line gap is " << s.gap << '\n';
  std::cerr << "note width is " << s.note_width << '\n';

  const std::size_t half_gap( s.gap / 2 );
  
  std::map< std::size_t, std::string > result;

  scan( img, s, s.top - half_gap - s.thickness / 2, "GECAFD", result );
  scan( img, s, s.top - 1, "FDBGEC", result );

  DBG( std::cerr << result.size() << " notes\n" );
  
  auto rit( result.begin() );
  std::cout << rit->second;
  
  for ( ++rit; rit != result.end(); ++rit )
    std::cout << ' ' << rit->second;

  std::cout << '\n';

  return 0;
}
