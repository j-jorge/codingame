#include <iostream>
#include <cassert>
#include <algorithm>

typedef std::uint64_t code_type;
static constexpr std::size_t bits_per_code( sizeof( code_type ) * 8 );

struct dna
{
  std::uint64_t code;
  std::size_t length;
};
  
static constexpr std::size_t bits_per_base( 3 );
static constexpr code_type base_mask( 7 );

code_type char_to_base( char c )
{
  switch( c )
    {
    case 'A': return 1;
    case 'C': return 2;
    case 'G': return 3;
    case 'T': return 4;
    }

  assert( false );
  return 0;
}

char base_to_char( code_type c )
{
  switch( c )
    {
    case 1: return 'A';
    case 2: return 'C';
    case 3: return 'G';
    case 4: return 'T';
    }

  assert( false );
  return 0;
}

dna append_base( dna op, code_type base )
{
  assert
    ( ( op.code
        & ( base_mask << ( sizeof( op.code ) * 8 - bits_per_base ) ) )
      == 0 );
  op.code = ( op.code << bits_per_base ) | base;
  ++op.length;
  
  return op;
}

dna string_to_strand( const std::string& s )
{
  dna result{ 0, 0 };
  
  for ( char c : s )
    result = append_base( result, char_to_base( c ) );

  return result;
}

std::string strand_to_string( dna d )
{
  if ( d.length == 0 )
    return "";

  return strand_to_string( { d.code >> bits_per_base, d.length - 1 } )
    + base_to_char( d.code & base_mask );
}

std::size_t best_offset( const dna& anchor, const dna& tail )
{
  std::size_t result( 0 );

  code_type c( 0 );
  code_type anchor_mask( 0 );
  
  for ( std::size_t i( 0 ); i != anchor.length; ++i )
    {
      c = ( c << bits_per_base );
      anchor_mask = ( anchor_mask << bits_per_base );
      
      if ( i < tail.length )
        {
          const std::size_t offset( ( tail.length - i - 1 ) * bits_per_base );

          c = c | ( ( tail.code & ( base_mask << offset ) ) >> offset );
          anchor_mask = anchor_mask | base_mask;
        }
      
      if ( ( anchor.code & anchor_mask ) == c )
        result = i + 1;
    }

  return result;
}

dna compress( const std::vector< dna >& strands )
{
  dna result( strands.front() );

  for ( std::size_t i( 1 ); i != strands.size(); ++i )
    {
      const dna& s( strands[ i ] );
      const std::size_t offset( best_offset( result, s ) );

      if ( offset < s.length )
        {
          const std::size_t keep( s.length - offset );
          result.length += keep;
          assert( result.length <= 21 );

          const std::size_t keep_bits( keep * bits_per_base );
          result.code =
            ( result.code << keep_bits )
            | ( s.code & ~code_type( 0 ) >> ( bits_per_code - keep_bits ) );
        }
    }
  
  return result;
}

bool compare_code( const dna& a, const dna& b )
{
  return a.code < b.code;
}

int main()
{
  std::string line;
  std::getline( std::cin, line );

  std::vector< dna > strands;
  while( std::getline( std::cin, line ) )
    strands.push_back( string_to_strand( line ) );

  std::sort( strands.begin(), strands.end(), &compare_code );
  
  dna result{ 0, 50 };
  
  do
    {
      const dna d( compress( strands ) );

      if ( d.length < result.length )
        result = d;
    }
  while( std::next_permutation
         ( strands.begin(), strands.end(), &compare_code ) );

  std::cout << result.length << '\n';
  
  return 0;
}

