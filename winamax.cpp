#include <array>
#include <deque>
#include <algorithm>
#include <iostream>
#include <cassert>

static const std::array< std::string, 13 > g_card_key =
  { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

enum class color_t
{
  spade,
  club,
  heart,
  diamond
};

struct card
{
  int value;
  color_t color;
};


card to_card( std::string str )
{
  card result;

  switch( str.back() )
    {
    case 'D':
      result.color = color_t::diamond;
      break;
    case 'H':
      result.color = color_t::heart;
      break;
    case 'C':
      result.color = color_t::club;
      break;
    case 'S':
      result.color = color_t::spade;
      break;
    default:
      {
        assert( false );
      }
    }

  str.pop_back();
  
  const auto it( std::find( g_card_key.begin(), g_card_key.end(), str ) );
  assert( it != g_card_key.end() );
  result.value = it - g_card_key.begin();

  return result;
}

std::deque< card > read_cards( std::istream& is )
{
  std::deque< card > result;
  std::size_t n;
  (is >> n).ignore();

  for ( std::size_t i( 0 ); i != n; ++i )
    {
      std::string line;
      std::getline( is, line );
      result.push_back( to_card( line ) );
    }

  return result;
}

void enqueue
( std::deque< card >& cards, std::vector< card >& first,
  std::vector< card >& second )
{
  cards.insert( cards.end(), first.begin(), first.end() );
  first.clear();
  cards.insert( cards.end(), second.begin(), second.end() );
  second.clear();
}

void discard
( std::vector< card >& discarded, std::deque< card >& cards, std::size_t count )
{
  assert( count <= cards.size() );

  const auto bit( cards.begin() );
  const auto eit( bit + count );
  discarded.insert( discarded.end(), bit, eit );
  cards.erase( bit, eit );
}
    
int main()
{
  std::deque< card > p1( read_cards( std::cin ) );
  std::deque< card > p2( read_cards( std::cin ) );

  std::vector< card > discarded_1;
  std::vector< card > discarded_2;

  bool pat( false );
  std::size_t turn( 0 );
  
  while( !p1.empty() && !p2.empty() && !pat )
    {
      discard( discarded_1, p1, 1 );
      discard( discarded_2, p2, 1 );

      const card& c1( discarded_1.back() );
      const card& c2( discarded_2.back() );
      
      if ( c1.value > c2.value )
        {
          ++turn;
          enqueue( p1, discarded_1, discarded_2 );
        }
      else if ( c2.value > c1.value )
        {
          ++turn;
          enqueue( p2, discarded_1, discarded_2 );
        }
      else if ( ( p1.size() < 4 ) || ( p2.size() < 4 ) )
        pat = true;
      else
        {
          discard( discarded_1, p1, 3 );
          discard( discarded_2, p2, 3 );
        }
    }

  if ( pat )
    std::cout << "PAT\n";
  else if ( p1.empty() )
    {
      if ( p2.empty() )
        std::cout << "PAT\n";
      else
        std::cout << "2 " << turn << '\n';
    }
  else
    std::cout << "1 " << turn << '\n';

  return 0;
}
