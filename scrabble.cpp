#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <cassert>

std::size_t letter_points( const std::string& s )
{
  static const std::size_t points[ 26 ] =
    {
      1, 3, 3, 2, 1,
      4, 2, 4, 1, 8,
      5, 1, 3, 1, 1,
      3, 10, 1, 1, 1,
      1, 4, 4, 8, 4,
      10
    };

  std::size_t result( 0 );

  for ( const auto& c : s )
    result += points[ c - 'a' ];

  return result;
}

std::uint32_t word_code( const std::string& s )
{
  std::uint32_t result( 0 );

  for ( const auto& c : s )
    result = result | ( 1 << ( c - 'a') );

  return result;
}

std::map< char, std::size_t > word_cardinality( const std::string& s )
{
  std::map< char, std::size_t > result;
  
  for ( const auto& c : s )
    ++result[ c ];

  return result;
}

struct dictionary_entry
{
public:
  explicit dictionary_entry( const std::string& s );
  
  std::string word;
  std::size_t points;
  std::uint32_t code;
  std::map< char, std::size_t > cardinality;
};

dictionary_entry::dictionary_entry( const std::string& s )
  : word( s ),
    points( letter_points( s ) ),
    code( word_code( s ) ),
    cardinality( word_cardinality( s ) )
{

}

void filter_by_code
( const std::vector< dictionary_entry >& dictionary,
  std::vector< std::size_t >& indices, std::uint32_t code )
{
  auto eit( indices.end() );
  
  indices.erase
    ( std::remove_if
      ( indices.begin(), indices.end(),
        [ &dictionary, &code ]( std::size_t i ) -> bool
        {
          return ( dictionary[ i ].code & ~code ) != 0;
        } ),
      eit );
}

void filter_by_cardinality
( const std::vector< dictionary_entry >& dictionary,
  std::vector< std::size_t >& indices, const std::string& letters )
{
  const auto cardinality( word_cardinality( letters ) );
  
  auto eit( indices.end() );
  
  indices.erase
    ( std::remove_if
      ( indices.begin(), indices.end(),
        [ &dictionary, &cardinality ]( std::size_t i ) -> bool
        {
          for ( const auto& e : dictionary[ i ].cardinality )
            if ( e.second > cardinality.find( e.first )->second )
              return true;

          return false;
        } ),
      eit );
}

int main()
{
  std::size_t word_count;
  ( std::cin >> word_count ).ignore();
  
  std::string word;
  std::vector< dictionary_entry > dictionary;
  dictionary.reserve( word_count );
  
  for ( std::size_t i( 0 ); i != word_count; ++i )
    {
      std::getline( std::cin, word );

      if ( word.size() > 7 )
        continue;

      dictionary.push_back( dictionary_entry( word ) );
    }

  std::string letters;
  std::getline( std::cin, letters );

  std::vector< std::size_t > indices( dictionary.size() );

  std::size_t i( 0 );
  std::generate
    ( indices.begin(), indices.end(), [ &i ]() -> std::size_t { return i++; } );

  filter_by_code( dictionary, indices, word_code( letters ) );
  std::cerr << indices.size() << " by indices\n";
  filter_by_cardinality( dictionary, indices, letters );
  std::cerr << indices.size() << " by cardinality\n";

  assert( !indices.empty() );
  auto it
    ( std::max_element
      ( indices.begin(), indices.end(),
        [ &dictionary ]( std::size_t a, std::size_t b ) -> bool
        {
          return dictionary[ a ].points < dictionary[ b ].points;
        } ) );
  
  std::cout << dictionary[ *it ].word << '\n';

  return 0;
}
