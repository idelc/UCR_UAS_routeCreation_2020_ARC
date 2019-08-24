#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <list>

using namespace std;

#include "prototypes.h"

double EXTRA_FEET_TO_ROUTE = 0; // TODO: Add real value to 
                                // acount for wingspan and turn radius

vector<obstacle> readObstacles(string fileName) {
   obstacle temp; 
   vector<obstacle> temps;
   ifstream read(fileName);

   if(!read.is_open()){
      cout << "Error opening file" << endl;
      exit(1); 
   }

   double la;//reads in lattitude val
   double lo;//reads in longitude val
   int   rad;//reads in radius
   char  com;//will trash commas
   
   while(read >> la >> com >> lo >> com >> rad) {//uses inherent boolean
      temp.lat    = la;
      temp.log    = lo; //reads in val
      temp.radius = rad; 
      temps.push_back(temp); //adds to vector
   }
   return temps;
}

vector<obstacle> readObstacles2(string fileName){
   obstacle temp; 
   vector<obstacle> temps;
   ifstream read(fileName);

   if(!read.is_open()){
      cout << "Error opening file" << endl;
      exit(1); 
   }

   double deg;//reads in degrees
   double min;//reads in minutes
   double sec;//reads in 
   double rad;
   char   head;
   char   rand;//will trash commas
   bool   first = true;
   double tempNum = 0;
   
   while(read >> head >> deg >> rand >> min >> rand >> sec){ //uses inherent boolean
      if((head == 'S') || (head == 'W')){
         tempNum = (deg + (min/60) + (sec/3600)) * -1;
      }
      else{
         tempNum = (deg + (min/60) + (sec/3600));
      }
      if(first){
         temp.lat = tempNum;
         first = false;
      }
      else{
         temp.log = tempNum;
         first = true;
         read >> rad;
         temp.radius = rad;
         temps.push_back(temp);
      }
   }
   return temps;
}

std::list<point> readPoints(string fileName){
   point temp; 
   list<point> temps;
   ifstream read(fileName);

   if(!read.is_open()) {
      cout << "Error opening file" << endl;
      exit(1); 
   }

   double deg;//reads in degrees
   double min;//reads in minutes
   double sec;//reads in 
   char   head;
   char   rand;//will trash commas
   bool   first = true;
   double tempNum = 0;
   
   while(read >> head >> deg >> rand >> min >> rand >> sec){ //uses inherent boolean
      if((head == 'S') || (head == 'W')){
         tempNum = (deg + (min/60) + (sec/3600)) * -1;
      }
      else{
         tempNum = (deg + (min/60) + (sec/3600));
      }
      if(first){
         temp.lat = tempNum;
         first = false;
      }
      else{
         temp.log = tempNum;
         first = true;
         temp.crit = true;
         temps.push_back(temp);
      }
   }
   return temps;
}

int colides(const point& toCheck, const vector<obstacle>& obstacles){ // int, not bool, returns coliding obstacle index
   double x = 0, y = 0;
   x = toCheck.log;
   y = toCheck.lat;
   double h = 0, k = 0, r = 0;
   for(unsigned i = 0; i < obstacles.size(); ++i){
      h = obstacles.at(i).log;
      k = obstacles.at(i).lat;
      r = obstacles.at(i).radius;
      if(pow((x-h), 2) + pow((y-k), 2) <= pow(r, 2)){
         return i;
      }
   }
   return -1; 
}

double distanceFt(const point& one, const point& two){
   // Using Haversine formula in order to acount for curvature
   // likely irrelevant due to actual distances but included just in case
   // Note: in the case of resource concervation being an issue, this is one place
   // to consider slimming 
   double distLat = (two.lat - one.lat) * M_PI / 180.0; 
   double distLon = (two.log - one.log) * M_PI / 180.0; 
  
   // convert to radians 
   double lat1 = (one.lat) * M_PI / 180.0; 
   double lat2 = (two.lat) * M_PI / 180.0; 

   // formula
   double a = pow(sin(distLat / 2), 2) +  
      pow(sin(distLon / 2), 2) * cos(lat1) * cos(lat2); 
   double rad = 20925197 ; // ft. rad of earth 
   double c = 2 * asin(sqrt(a)); // asin == arcsine
   return rad * c; 
}

point midpoint(const point& one, const point& two){
   point mid;
   mid.lat = (one.lat + two.lat) / 2;
   mid.log = (one.log + two.log) / 2;
   return mid;
}

//Assuming number of div.(variable n) >= 3

                   //x,y vals;       radius; #of sides for poly.
std::list<point> subdivideCircle(const obstacle& o, int n) {
   std::list<point> points;
   point pt;
   for(int i = 0; i < n; ++i) {
      pt.log = o.radius * cos(((2 * M_PI) / n) * i) + o.log;
      pt.lat = o.radius * sin(((2 * M_PI) / n) * i) + o.lat;
      points.push_front(pt);
   }
   return points;
}
// Given the center of the obstacle,
// its radius, and the number of points we want
// this function will create and return a list 
// of said points
// this shoudl be in clockwise order,
// the lat and log of the point will be used
// to find the geographical loc. of the points
// and not just their relation to the circle (which was
// found using simple trig.)

// Distance from a point to a line, will return true for clear
// latitude used as y, longitude as x. Since theire is "no constraint on height" it shall be ignored.
bool pathCheckClear(const point& first, const point& second, const obstacle& toTest){
   return (abs(((second.lat - first.lat)*toTest.log) - ((second.log - first.log)*toTest.lat) + (second.log*first.lat) - (second.lat*first.log))) / distanceFt(first, second);
}
// Above: 2A/distance
