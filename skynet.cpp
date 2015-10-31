#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int R; // the length of the road before the gap.
    cin >> R; cin.ignore();
    int G; // the length of the gap.
    cin >> G; cin.ignore();
    int L; // the length of the landing platform.
    cin >> L; cin.ignore();

    // game loop
    while (1) {
        int S; // the motorbike's speed.
        cin >> S; cin.ignore();
        int X; // the position on the road of the motorbike.
        cin >> X; cin.ignore();

        if ( X >= R )
          std::cout << "SLOW\n";
        else if ( X + 1 == R )
          std::cout << "JUMP\n";
        else if ( S <= G )
          std::cout << "SPEED\n";
        else if ( S > G + 1 )
          std::cout << "SLOW\n";
        else
          std::cout << "WAIT\n";
    }
}
