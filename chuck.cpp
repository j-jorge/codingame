#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class encoder
{
public:
  std::string encoded() const { return m_encoded.str(); }

  void encode( char c )
  {
    encode( c, 6 );
  }

  void flush()
  {
    write_encoded();
  }
  
private:
  void encode( char c, unsigned char bit_index )
  {
    if ( bit_index != 0 )
      encode( c >> 1, bit_index - 1 );

    const unsigned char bit( c & 1 );

    if ( m_bit_count == 0 )
      reset_last_bit( bit );
    else if ( bit == m_last_bit )
        ++m_bit_count;
    else
      {
        write_encoded();
        reset_last_bit( bit );
      }
  }

  void write_encoded()
  {
    if ( !m_encoded.str().empty() )
      m_encoded << ' ';

    if ( m_last_bit == 1 )
      m_encoded << '0';
    else
      m_encoded << "00";

    m_encoded << ' ';

    for ( std::size_t i(0); i != m_bit_count; ++i )
      m_encoded << '0';

    m_bit_count = 0;
  }
  
  void reset_last_bit( unsigned char bit )
  {
    m_last_bit = bit;
    m_bit_count = 1;
  }
  
private:
  std::ostringstream m_encoded;
  std::size_t m_bit_count = 0;
  unsigned char m_last_bit;
};

int main()
{
  std::string message;
  std::getline(std::cin, message);

  encoder e;

  for ( const char& c : message )
    e.encode( c );

  e.flush();
  std::cout << e.encoded() << '\n';

  return 0;
}
