#include <set>
#include <iostream>

struct node
{
  explicit node( char n )
    : number( n )
  {

  }
  
  bool operator<( const node& that ) const
  {
    return number < that.number;
  }

  template< typename Iterator >
  std::size_t insert( Iterator first, Iterator last ) const
  {
    if ( first == last )
      return 0;
    
    auto r( next.insert( node( *first ) ) );
    return std::size_t( r.second ) + r.first->insert( ++first, last );
  }
  
  char number;
  mutable std::set< node > next;
};
  
int main()
{
  std::size_t count;
  (std::cin >> count).ignore();

  std::size_t result( 0 );
  node numbers( 0 );
  std::string n;
  for( ; count != 0; --count )
    {
      std::getline( std::cin, n );
      result += numbers.insert( n.begin(), n.end() );
    }

  std::cout << result << '\n';
  return 0;
}
