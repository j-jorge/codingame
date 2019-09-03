#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

int main()
{
  std::random_device rd;
  std::mt19937 random(rd());
  std::uniform_int_distribution<> random_row( 0, 19 );
  std::uniform_int_distribution<> random_column( 0, 34 );
  
  int opponent_count;
  std::cin >> opponent_count;

  while (1)
    {
      int round;
      int my_x;
      int my_y;
      int my_rewind;
        
      std::cin >> round >> my_x >> my_y >> my_rewind;
        
      for (int i( 0 ); i != opponent_count; ++i)
        {
          int opponent_x;
          int opponent_y;
          int opponent_rewind;
          std::cin >> opponent_x >> opponent_y >> opponent_rewind;
        }

      std::cin.ignore();
        
      for (int i( 0 ); i != 20; ++i)
        {
          std::string line;
          std::cin >> line;
          std::cin.ignore();
        }

      std::cout << random_column(random) << ' ' << random_row(random) << '\n';
    }

  return 0;
}
