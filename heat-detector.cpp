#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

struct cell_t
{
  int x;
  int y;

  bool operator==( const cell_t& that ) const
  {
    return ( x == that.x ) && ( y == that.y );
  }
};

struct region
{
  int top;
  int left;
  int width;
  int height;

  region remove_right( int x ) const
  {
    assert( left < x );
    assert( x < left + width );
    return { top, left, x - left, height };
  }
  
  region remove_left( int x ) const
  {
    assert( left <= x );
    assert( x < left + width );
    return { top, x + 1, left + width - x - 1, height };
  }
  
  region remove_bottom( int y ) const
  {
    assert( top < y );
    assert( y < top + height );
    return { top, left, width, y - top };
  }
  
  region remove_top( int y ) const
  {
    assert( top <= y );
    assert( y < top + height );
    return { y + 1, left, width, top + height - y - 1 };
  }

  cell_t center() const
  {
    return { left + width / 2, top + height / 2 };
  }
};


int main()
{
  region area = { 0, 0, 0, 0 };
  std::size_t n;
  cell_t batman;

  ( std::cin >> area.width >> area.height >> n >> batman.x
    >> batman.y ).ignore();

  std::string bomb;
  
  while( true )
    {
      std::getline( std::cin, bomb );

      std::cerr << bomb << std::endl;
      
      if ( bomb == "U" )
        area = area.remove_bottom( batman.y );
      else if ( bomb == "UR" )
        {
          area = area.remove_bottom( batman.y );
          area = area.remove_left( batman.x );
        }
      else if ( bomb == "UL" )
        {
          area = area.remove_bottom( batman.y );
          area = area.remove_right( batman.x );
        }
      else if ( bomb == "D" )
        area = area.remove_top( batman.y );
      else if ( bomb == "DR" )
        {
          area = area.remove_top( batman.y );
          area = area.remove_left( batman.x );
        }
      else if ( bomb == "DL" )
        {
          area = area.remove_top( batman.y );
          area = area.remove_right( batman.x );
        }
      else if ( bomb == "L" )
        area = area.remove_right( batman.x );
      else if ( bomb == "R" )
        area = area.remove_left( batman.x );
      else
        {
          assert( false );
        }

      std::cerr << area.left << ' ' << area.top << ' '
                << area.width << ' ' << area.height << std::endl;
      
      batman = area.center();
      std::cout << batman.x << ' ' << batman.y << '\n';
    }

  return 0;
}
