#include <iostream>
#include <string>
#include <vector>

class Answer
{
public:
  static int closestToZero(int ints[], int size) {

    if ( ( ints == nullptr ) || ( size == 0 ) )
      return 0;

    int best( ints[ 0 ] );
    
    for ( int i( 1 ); i != size; ++i )
      {
        const int candidate( ints[ i ] );
        if ( std::abs( candidate ) < std::abs( best ) )
            best = candidate;
        else if ( ( std::abs( candidate ) == std::abs( best ) )
                  && ( candidate > 0 ) )
          best = candidate;
      }
    
    return best;
  }
};

int main()
{
    int n; // the number of temperatures to analyse
    std::cin >> n; std::cin.ignore();

    std::vector< int > t( n );
    for ( auto& i : t )
      std::cin >> i;
    
    std::cout << Answer::closestToZero( t.data(), t.size() ) << std::endl;
    return 0;
}
#if 0
  int ints_1[] = { -9, 8, 2, -5, 7 };
  std::cout << Answer::closestToZero( ints_1, 5 ) << '\n';

  int ints_2[] = { -2147483647 };
  std::cout << Answer::closestToZero( ints_2, 1 ) << '\n';

  int ints_3[] = { 2147483647 };
  std::cout << Answer::closestToZero( ints_3, 1 ) << '\n';

  int ints_4[] = { -3, -2, -9 };
  std::cout << Answer::closestToZero( ints_4, 3 ) << '\n';
  
  int ints_5[] = { -3, -3 };
  std::cout << Answer::closestToZero( ints_5, 2 ) << '\n';

  std::cout << Answer::closestToZero( ints_5, 0 ) << '\n';

  std::cout << Answer::closestToZero( nullptr, 20 ) << '\n';
  return 0;
}
#endif
