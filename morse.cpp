#define NDEBUG

#include <iostream>
#include <array>
#include <cassert>
#include <vector>
#include <limits>

static std::array< char, 255 > g_code_to_char;

std::uint8_t append_to_code( std::uint8_t code, char c )
{
  static constexpr std::uint8_t dot( 1 );
  static constexpr std::uint8_t dash( 2 );

  if ( c == '.' )
    return ( code << 2 ) | dot;

  assert( c == '-' );
  return ( code << 2 ) | dash;
}

std::uint8_t pattern_to_code( const std::string& p )
{
  assert( !p.empty() );
  assert( p.size() <= 4 );

  std::uint8_t result( 0 );

  for ( char c : p )
    result = append_to_code( result, c );

  return result;
}

void fill_code_to_char()
{
  for ( char& c : g_code_to_char )
    c = 0;

  g_code_to_char[ pattern_to_code( ".-" ) ] = 'A';
  g_code_to_char[ pattern_to_code( "-..." ) ] = 'B';
  g_code_to_char[ pattern_to_code( "-.-." ) ] = 'C';
  g_code_to_char[ pattern_to_code( "-.." ) ] = 'D';
  g_code_to_char[ pattern_to_code( "." ) ] = 'E';
  g_code_to_char[ pattern_to_code( "..-." ) ] = 'F';
  g_code_to_char[ pattern_to_code( "--." ) ] = 'G';
  g_code_to_char[ pattern_to_code( "...." ) ] = 'H';
  g_code_to_char[ pattern_to_code( ".." ) ] = 'I';
  g_code_to_char[ pattern_to_code( ".---" ) ] = 'J';
  g_code_to_char[ pattern_to_code( "-.-" ) ] = 'K';
  g_code_to_char[ pattern_to_code( ".-.." ) ] = 'L';
  g_code_to_char[ pattern_to_code( "--" ) ] = 'M';
  g_code_to_char[ pattern_to_code( "-." ) ] = 'N';
  g_code_to_char[ pattern_to_code( "---" ) ] = 'O';
  g_code_to_char[ pattern_to_code( ".--." ) ] = 'P';
  g_code_to_char[ pattern_to_code( "--.-" ) ] = 'Q';
  g_code_to_char[ pattern_to_code( ".-." ) ] = 'R';
  g_code_to_char[ pattern_to_code( "..." ) ] = 'S';
  g_code_to_char[ pattern_to_code( "-" ) ] = 'T';
  g_code_to_char[ pattern_to_code( "..-" ) ] = 'U';
  g_code_to_char[ pattern_to_code( "...-" ) ] = 'V';
  g_code_to_char[ pattern_to_code( ".--" ) ] = 'W';
  g_code_to_char[ pattern_to_code( "-..-" ) ] = 'X';
  g_code_to_char[ pattern_to_code( "-.--" ) ] = 'Y';
  g_code_to_char[ pattern_to_code( "--.." ) ] = 'Z';
}

class dictionary
{
public:
  dictionary();
  dictionary( const dictionary& ) = delete;
  dictionary& operator=( const dictionary& ) = delete;
  ~dictionary();
  
  template<typename Iterator>
  void insert( Iterator first, Iterator last );
    
  bool terminal() const;

  const dictionary* suffixes( char key ) const;

private:
  std::array<dictionary, 'Z' - 'A' + 1>* m_next;
  bool m_terminal;
};

dictionary::dictionary()
  : m_next( nullptr ),
    m_terminal( false )
{

}

dictionary::~dictionary()
{
  delete m_next;
}
    
bool dictionary::terminal() const
{
  return m_terminal;
}

const dictionary* dictionary::suffixes( char key ) const
{
  assert( key >= 'A' );
  assert( key <= 'Z' );
  
  if ( m_next == nullptr )
    return nullptr;

  return &(*m_next)[ key - 'A' ];
}      

template<typename Iterator>
void dictionary::insert( Iterator first, Iterator last )
{
  if ( first == last )
    m_terminal = true;
  else
    {
      assert( 'A' <= *first );
      assert( *first <= 'Z' );
      
      if ( m_next == nullptr )
        m_next = new std::array<dictionary, 26>();

      const char key( *first - 'A' );
      ++first;
      (*m_next)[ key ].insert( first, last );
    }
}

class solver
{
public:
  solver( const std::string& message, const dictionary& d );

  std::uint64_t operator()();

private:
  struct node
  {
    std::size_t i;
    const dictionary* dict;
  };

private:
  std::uint64_t solve( std::size_t start );
  
private:
  static const std::uint64_t unknown_result;
  const std::string& m_message;
  const std::size_t m_message_size;
  const dictionary& m_dictionary;
  
  std::vector< node > m_pending;
  std::vector< std::uint64_t > m_result_at;
};

const std::uint64_t solver::unknown_result =
  std::numeric_limits< std::uint64_t >::max();

solver::solver( const std::string& message, const dictionary& d )
  : m_message( message ),
    m_message_size( m_message.size() ),
    m_dictionary( d ),
    m_result_at( m_message_size, unknown_result )
{
  m_pending.reserve( 100 );
}

std::uint64_t solver::solve( std::size_t start )
{
  std::uint64_t result( 0 );
  m_pending.resize( 0 );
  m_pending.push_back( { start, &m_dictionary } );

  while ( !m_pending.empty() )
    {
      const node n( m_pending.back() );
      m_pending.pop_back();

      if ( n.i == m_message_size )
        {
          result += n.dict->terminal();
          continue;
        }

      if ( n.dict->terminal() )
        {
          assert( m_result_at[ n.i ] != unknown_result );
          result += m_result_at[ n.i ];
        }

      std::uint8_t pattern( 0 );
      
      for ( std::size_t i( 0 );
            ( i != 4 ) && ( n.i + i != m_message_size );
            ++i )
        {
          const std::size_t message_index( n.i + i );
          pattern = append_to_code( pattern, m_message[ message_index ] );
          
          const char letter( g_code_to_char[ pattern ] );

          if ( letter == 0 )
            continue;
          
          const dictionary* const next_dict( n.dict->suffixes( letter ) );

          if ( next_dict != nullptr )
            m_pending.push_back( { message_index + 1, next_dict } );
        }
    }

  return result;
}

std::uint64_t solver::operator()()
{
  for ( std::size_t i( 0 ); i != m_message_size; ++i )
    {
      const std::size_t start( m_message_size - i - 1 );
      m_result_at[ start ] = solve( start );
    }

  return m_result_at[ 0 ];
}

int main()
{
  fill_code_to_char();

  std::string message;
  std::getline( std::cin, message );

  dictionary d;

  std::size_t word_count;
  ( std::cin >> word_count ).ignore();

  std::string word;
  
  for ( ; word_count != 0; --word_count )
    {
      std::getline( std::cin, word );
      d.insert( word.begin(), word.end() );
    }

  solver s( message, d );
  std::cout << s() << '\n';
  return 0;
}
