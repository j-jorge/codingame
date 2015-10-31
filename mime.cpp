#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cctype>

int main()
{
  std::map< std::string, std::string > mimes;
  
  int N;
  std::cin >> N; std::cin.ignore();
  int Q;
  std::cin >> Q; std::cin.ignore();

  for (int i = 0; i < N; i++)
    {
      std::string EXT;
      std::string MT;
      std::cin >> EXT >> MT; std::cin.ignore();
      
      std::transform( EXT.begin(), EXT.end(), EXT.begin(), &::tolower );
      mimes[ EXT ] = MT;
    }
  
  for (int i = 0; i < Q; i++)
    {
      std::string FNAME;
      std::getline(std::cin, FNAME);

      const std::string::size_type p( FNAME.find_last_of( '.' ) );
      if ( p == std::string::npos )
        std::cout << "UNKNOWN\n";
      else
        {
          std::string ext( FNAME.substr( p + 1 ) );
          std::transform( ext.begin(), ext.end(), ext.begin(), &::tolower );

          const auto it( mimes.find( ext ) );
          if ( it == mimes.end() )
            std::cout << "UNKNOWN\n";
          else
            std::cout << it->second << '\n';
        }
    }

  return 0;
}
