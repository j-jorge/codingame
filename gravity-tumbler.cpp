#include <iostream>

void sort( char* first, char* last, int count, int step )
{
  for ( int i( 0 ); i != count; ++i )
    if ( *first == '#' )
      first += step;
    else
      {
        *first = *last;
        *last = '.';
        last -= step;
      }
}

void gravity_down( char* cells, int width, int height )
{
  char* p( cells );
  int last_offset( width * ( height - 1 ) );
  
  for ( int i( 0 ); i != width; ++i, ++p )
    sort( p, p + last_offset, height, width );
}

void gravity_up( char* cells, int width, int height )
{
  char* p( cells );
  int last_offset( width * ( height - 1 ) );
  
  for ( int i( 0 ); i != width; ++i, ++p )
    sort( p + last_offset, p, height, -width );
}

void gravity_left( char* cells, int width, int height )
{
  char* p( cells );

  for ( int i( 0 ); i != height; ++i, p += width )
    sort( p, p + width - 1, width, 1 );
}

void gravity_right( char* cells, int width, int height )
{
  char* p( cells );

  for ( int i( 0 ); i != height; ++i, p += width )
    sort( p + width - 1, p, width, -1 );
}

void print( char* first, int count, int step )
{
  char* p( first );
  
  for ( int i( 0 ); i != count; ++i, p += step )
    std::cout << *p;

  std::cout << '\n';
}

void print_0( char* cells, int width, int height )
{
  char* p( cells + width * ( height - 1 ) );
  
  for ( int i( 0 ); i != height; ++i, p -= width )
    print( p, width, 1 );
}

void print_90( char* cells, int width, int height )
{
  char* p( cells + width * height - 1 );
  
  for ( int i( 0 ); i != width; ++i, --p )
    print( p, height, -width );
}

void print_180( char* cells, int width, int height )
{
  char* p( cells + width - 1 );

  for ( int i( height ); i != 0; --i, p += width )
    print( p, width, -1 );
}

void print_270( char* cells, int width, int height )
{
  char* p( cells );
  
  for ( int i( 0 ); i != width; ++i, ++p )
    print( p, height, width );
}

int main()
{
    int width;
    int height;
    int count;

    std::cin >> width >> height >> count;
    std::cin.ignore();

    char cells[ 99 * 9 ];
    char* p( cells + width * ( height - 1 ) );
    
    for ( int y( 0 ); y != height; ++y, p -= width )
      {
        for ( int x( 0 ); x != width; ++x )
          std::cin >> p[ x ];

        std::cin.ignore();
    }

    int direction( 0 );
    
    for ( ; count != 0; --count )
      {
        if ( direction == 3 )
          direction = 0;
        else
          ++direction;
        
        switch( direction )
          {
          case 0:
            gravity_down( cells, width, height );
            break;
          case 1:
            gravity_left( cells, width, height );
            break;
          case 2:
            gravity_up( cells, width, height );
            break;
          case 3:
            gravity_right( cells, width, height );
            break;
          }
      }

    switch( direction )
      {
      case 0:
        print_0( cells, width, height );
        break;
      case 1:
        print_90( cells, width, height );
        break;
      case 2:
        print_180( cells, width, height );
        break;
      case 3:
        print_270( cells, width, height );
        break;
      }

    return 0;
}
