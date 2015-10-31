#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct point
{
  int x;
  int y;
};

void pick_landing_site
( const std::vector< point >& land, int X, int& landing_site_left,
  int& landing_site_right, int& landing_site_bottom )
{
  const point* prev( &land[ 0 ] );
  
  for ( std::size_t i( 1 ); i != land.size(); ++i )
    if ( land[ i ].y == prev->y )
      {
        landing_site_left = prev->x + 20;
        landing_site_bottom = prev->y;
        landing_site_right = land[ i ].x - 20;
        return;
      }
    else
      prev = &land[ i ];
}

int main()
{
    int surfaceN; // the number of points used to draw the surface of Mars.
    ( std::cin >> surfaceN ).ignore();

    std::vector< point > land( surfaceN );
    
    for (int i = 0; i < surfaceN; i++) {
      ( std::cin >> land[ i ].x >> land[ i ].y ).ignore();
    }

    int landing_site_left{ -1 };
    int landing_site_right{ -1 };
    int landing_site_bottom{ -1 };
    int landing_site{ -1 };

    static const int left( 45 );
    static const int right( -45 );
    
    while ( true )
      {
        int X;
        int Y;
        int hSpeed; // the horizontal speed (in m/s), can be negative.
        int vSpeed; // the vertical speed (in m/s), can be negative.
        int fuel; // the quantity of remaining fuel in liters.
        int rotate; // the rotation angle in degrees (-90 to 90).
        int power; // the thrust power (0 to 4).

        ( std::cin >> X >> Y >> hSpeed >> vSpeed >> fuel >> rotate >> power )
          .ignore();

        int direction_angle( rotate );
        int thrust( power );

        float angle_need( 0 );
        
        if ( landing_site_left == -1 )
          {
            pick_landing_site
              ( land, X, landing_site_left, landing_site_right,
                landing_site_bottom );
            landing_site = ( landing_site_left + landing_site_right ) / 2;
          }

        if ( ( landing_site_left < X ) && ( X < landing_site_right  ) )
          {
            std::cerr << "in the landing zone: " << hSpeed << "\n";
            // in the landing zone
            if ( hSpeed <= -5 )
              {
                direction_angle = right;
                angle_need = std::min< float >( 1, float( -hSpeed - 5 ) / 5 );
              }
            else if ( hSpeed >= 5 )
              {
                direction_angle = left;
                angle_need = std::min< float >( 1, float( hSpeed - 5 ) / 5 );
              }
            else
                std::cerr << "hSpeed = " << hSpeed << '\n';
          }
        else if ( landing_site_right < X )
          {
            std::cerr << "to left\n";
            direction_angle = left;

            if ( hSpeed > 0 )
              angle_need = 1;
            else if ( hSpeed < -70 )
              {
                direction_angle = right;
                angle_need = 1;
              }
            else
              {
                angle_need = float( X + hSpeed - landing_site_right ) / 7000;
              }
          }
        else if ( X < landing_site_left )
          {
            std::cerr << "to right\n";
            direction_angle = right;

            if ( hSpeed < 0 )
              angle_need = 1;
            else if ( hSpeed > 70 )
              {
                direction_angle = left;
                angle_need = 1;
              }
            else
            {
              angle_need = float( landing_site_left - X - hSpeed ) / 7000;
                              if ( Y > 2300 )
                  direction_angle = std::min( 2 * direction_angle, -90 );
}
          }

std::cerr << ( Y - landing_site_bottom ) << ' ' << Y << '\n';
        if ( ( Y - landing_site_bottom > 2200 )
             && ( Y > 2400 )  )
          {
            if ( vSpeed > 0 )
              thrust = 0;
            else if ( vSpeed > -10 )
              thrust = 2;
            else 
              thrust = 4;
          }
        else if ( vSpeed <= -40 )
          {
            thrust = 4;
          }
        else if ( vSpeed <= -35 )
          {
            thrust = 4;
            angle_need /= 2;
          }
        else if ( angle_need != 0 )
          {
              thrust = 4;
          }
        else
          thrust = 0;
        
        std::cout << int( angle_need * direction_angle ) << ' ' << thrust << '\n';
    }
}
