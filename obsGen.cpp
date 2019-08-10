#include "prototypes.h"
#include <cstdlib>
#include <cmath>
#include <list>

//Assuming number of div.(variable n) >= 3

                   //x,y vals;       radius; #of sides for poly.
std::list<point> subdivideCircle(const obstacle o, int n) {
   std::list<point> points;
   point pt;
   for(int i = 0; i < n; ++i) {
      pt.log = o.radius * cos(((2 * M_PI) / n) * i) + o.log;
      pt.lat = o.radius * sin(((2 * M_PI) / n) * i) + o.lat;
      points.push_front(pt);
   }
   return points;
}




//----------------------------------------------------------

// Given the center of the obstacle,
// its radius, and the number of points we want
// this function will create and return a list 
// of said points
// this shoudl be in clockwise order,
// the lat and log of the point will be used
// to find the geographical loc. of the points
// and not just their relation to the circle (which was
// found using simple trig.)
