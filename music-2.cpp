#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <string>
#include <algorithm>

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
  
  void dump( const std::string& name )
  {
    std::ofstream f( name, std::ios::binary | std::ios::out );
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
  std::size_t tail_width;
};

struct v_bounds
{
  std::size_t top;
  std::size_t bottom;

  bool empty() const
  {
    return top == bottom;
  }
};
  
v_bounds tail_bounds( const image& img, std::size_t x, std::size_t threshold )
{
  const std::size_t h( img.height() );

  for ( std::size_t y( 0 ); y != h; )
    {
      std::size_t start( y );
      for ( ; ( start != h ) && ( img( x, start ) == white ); ++start );

      std::size_t end( start );
      for ( ; ( end != h ) && ( img( x, end ) == black ); ++end );

      if ( end - start >= threshold )
        return { start, end };

      y = end;
    }

  return { h, h };
}

std::size_t compute_tail_width
( const image& img, std::size_t start_x, std::size_t threshold,
  std::size_t max_width )
{
  const std::size_t w( img.width() );
  
  for ( std::size_t x( start_x ); x != w; ++x )
    {
      const v_bounds b( tail_bounds( img, x, threshold ) );

      if ( b.empty() )
        continue;
      
      std::vector< std::size_t > result( max_width, 0 );

      for ( std::size_t y( b.top ); y != b.bottom; ++y )
        {
          std::size_t f;
          
          for ( f = x;
                ( f - x != max_width ) && ( img( f, y ) == black );
                ++f );

          ++result[ f - x ];
        }

      return std::max_element( result.begin(), result.end() ) - result.begin();
    }

  assert( false );
  return 0;
}

staff find_staff( const image& img )
{
  const std::size_t w( img.width() );
  const std::size_t h( img.height() );

  staff result;
  bool stop( false );

  std::size_t x;
  
  for( x = 0; !stop && ( x != w ) ; ++x )
    for ( std::size_t y( 0 ); !stop && ( y != h ); ++y )
      if ( img( x, y ) == black )
        {
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
          stop = true;
        }

  result.tail_width =
    compute_tail_width
    ( img, x, result.gap + 2 * result.thickness + 1, result.gap );
  
  return result;
}

void erase_region
( image& img, const staff& s, const std::size_t x, const std::size_t max_x,
  const std::size_t y )
{
  bool modified( true );
  std::size_t cy( y );
  
  while ( modified )
    {
      modified = false;
      for ( std::size_t cx( x ); cx != max_x; ++cx )
        if ( img( cx, cy ) == black )
          {
            modified = true;
            img( cx, cy ) = white;
          }
      --cy;
    }

  modified = true;
  cy = y + s.thickness;
  
  while ( modified )
    {
      modified = false;
      for ( std::size_t cx( x ); cx != max_x; ++cx )
        if ( img( cx, cy ) == black )
          {
            modified = true;
            img( cx, cy ) = white;
          }

      ++cy;
    }
}

std::size_t erase_black
( image& img, const staff& s, const std::size_t x, const std::size_t y )
{
  std::size_t cx( x );
  
  for( ; img( cx, y ) == black; ++cx ) assert( cx != img.width() );
  erase_region( img, s, x, cx, y );

  return cx;
}

std::size_t erase_white
( image& img, const staff& s, const std::size_t x, const std::size_t y )
{
  std::size_t cx( x );
  
  for( ; img( cx, y ) == black; ++cx ) assert( cx != img.width() );
  for( ; img( cx, y ) == white; ++cx ) assert( cx != img.width() );
  for( ; img( cx, y ) == black; ++cx ) assert( cx != img.width() );

  erase_region( img, s, x, cx, y );

  return cx;
}

void scan
( image& img, const staff& s, std::size_t y, std::size_t v_scan,
  const char* const notes, std::map< std::size_t, std::string >& result )
{
  const std::size_t half_gap( s.gap / 2 );
  const std::size_t w( img.width() );
  
  for ( const char* n( notes ); *n != 0; ++n )
    {
      DBG( std::cerr << "scan " << *n << " at y=" << y << '\n' );
      
      for ( std::size_t x( s.left ); x != w; ++x )
        if ( ( img( x, y ) == black )
             && ( img( x, y + s.thickness + 1 ) == black ) )
          {
            if ( img( x + half_gap, y ) == black )
              {
                DBG( std::cerr << "* b/" << *n << " at " << x << ", "
                     << y << '\n' );
                result[ x ] = *n + std::string( "Q" );

                x = erase_black( img, s, x, y );
              }
            else
              {
                DBG( std::cerr << "* w/" << *n << " at " << x << ", "
                     << y << '\n' );
                result[ x ] = *n + std::string( "H" );

                x = erase_white( img, s, x, y );
              }
          }

      y += s.gap + s.thickness;
    }
}

std::size_t remove_tail( const staff& s, image& img, std::size_t x )
{
  const std::size_t threshold( s.gap + 2 * s.thickness + 1 );

  v_bounds bounds( tail_bounds( img, x, threshold ) );

  if ( bounds.empty() )
    return x + 1;

  bool color_above_tail( img( x - 1, bounds.top - 1 ) );
  bool monochrome( true );

  for ( std::size_t i( x - 1 ); monochrome && ( i != x + s.tail_width + 1 );
        ++i )
    monochrome = monochrome && ( img( i, bounds.top - 1 ) == color_above_tail );

  if ( monochrome ) // tail goes upward
    bounds.bottom -= s.gap / 2;
  else // tail goes downward
    bounds.top += s.gap / 2;
  
  for ( std::size_t y( bounds.top ); y != bounds.bottom; ++y )
    if ( ( img( x + s.tail_width, y ) == white )
         && ( img( x - 1, y ) == white ) )
      for ( std::size_t i( 0 ); i != s.tail_width; ++i )
        img( x + i, y ) = white;
  
  return x + s.tail_width;
}

void remove_tails( const staff& s, image& img )
{
  const std::size_t w( img.width() );

  for ( std::size_t x( 0 ); x != w; )
    x = remove_tail( s, img, x );
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
  std::cerr << "tail width is " << s.tail_width << '\n';

  remove_tails( s, img );
  img.dump( "clean.data" );

  const std::size_t half_gap( s.gap / 2 );
  
  std::map< std::size_t, std::string > result;

  const std::size_t y_on_line( s.top - 1 );
  scan( img, s, y_on_line, s.gap / 2, "FDBGEC", result );

  const std::size_t y_between_lines( s.top - half_gap - s.thickness / 2 );
  scan
    ( img, s, y_between_lines, ( s.gap - s.thickness / 2 ) / 2, "GECAFD",
      result );

  img.dump( "final.data" );

  DBG( std::cerr << result.size() << " notes\n" );
  
  auto rit( result.begin() );
  std::cout << rit->second;
  
  for ( ++rit; rit != result.end(); ++rit )
    std::cout << ' ' << rit->second;

  std::cout << '\n';
  
  return 0;
}
