#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <list>
#include <stack>

using namespace std;

#include "prototypes.h"

double EXTRA_FEET_TO_ROUTE = 0; // TODO: Add real value to 
                                // acount for wingspan

double TURN_RADIUS_FT = 150;

double DEFAULT_ALTITUDE = 200;

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
   read.close();
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
   read.close();
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

   double la;//reads in lattitude val
   double lo;//reads in longitude val
   double he;//reads in height
   char  com;//will trash commas
   
   while(read >> la >> com >> lo >> com >> he) {//uses inherent boolean
      temp.lat    = la;
      temp.log    = lo; //reads in val
      temp.height = he; 
      temp.crit = true;
      temps.push_back(temp); //adds to list
   }
   read.close();
   return temps;
}


std::list<point> readPoints2(string fileName){
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
   read.close();
   return temps;
}

int collides(const point& toCheck, const vector<obstacle>& obstacles){ // int, not bool, returns coliding obstacle index
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
   if(distanceFt(first,second) == 0){
      return true;
   }
   return (toTest.radius + EXTRA_FEET_TO_ROUTE) < (abs(((second.lat - first.lat)*toTest.log) - ((second.log - first.log)*toTest.lat) + (second.log*first.lat) - (second.lat*first.log))) / distanceFt(first, second);
}
// Above: rad+wingspan&misc < 2Area/distance. 
// If true, the distance from the center of the obstacle to its perimiter is less than 
// the distance from the center to the path, thus the path clears the obstacle.


// Checks if points will requite a turn
bool turnAhead(const point& first, const point& second, const point& third){
   trajectory ogPath(first, second);
   trajectory potPath(second, third);
   double hypoAng = ogPath.angleBetween(potPath);
   double maxTurn = turnAngleMax(second, third);
   return hypoAng > maxTurn;
}

trajectory::trajectory():coefficientI(0), coefficientJ(0), coefficientK(0){};

trajectory::trajectory(double i, double j, double k):coefficientI(i), coefficientJ(j), coefficientK(k){};

// latitude used as y, longitude as x, height as z
trajectory::trajectory(const point& tail, const point& head): coefficientI(head.log - tail.log), coefficientJ(head.lat - tail.lat), coefficientK(head.height - tail.height){}

double trajectory::angleBetween(const trajectory& second){
   double crossScalar = (this->coefficientI * second.coefficientI) + (this->coefficientJ * second.coefficientJ) + (this->coefficientK * second.coefficientK);
   double magFirst = sqrt((this->coefficientI * this->coefficientI) + (this->coefficientJ * this->coefficientJ) + (this->coefficientK * this->coefficientK)); 
   double magSecond = sqrt((second.coefficientI * second.coefficientI) + (second.coefficientJ * second.coefficientJ) + (second.coefficientK * second.coefficientK));
   return acos(crossScalar/(magFirst*magSecond));
}

double turnAngleMax(const point& center, const point& edge){
   double radLength = distanceFt(center, edge) / 2;
   return (M_PI / 2) - acos(radLength/TURN_RADIUS_FT);
}

vector<point> arcTurn(point& beg, point& mid, point& end){
   vector <point> toReturn(11);
   toReturn.at(0) = beg;
   toReturn.at(5) = mid;
   toReturn.at(10) = end; 
   point comp1 = midpoint(beg, mid);
   point comp2 = midpoint(mid, end);
   point center = midpoint(comp1, comp2);
   double radiusOfArc = distanceFt(center, comp1);
   // find angle between beg, mid, split arc up into points, add
   // do the same between mid and end, add
   // return
   trajectory centBeg(center, beg);
   trajectory centMid(center, mid);
   trajectory centEnd(center, end);
   double begMidAng = centBeg.angleBetween(centMid);
   double midEndAng = centMid.angleBetween(centEnd);
   double partSpacing1 = (begMidAng * radiusOfArc) / 4;
   double partSpacing2 = (midEndAng * radiusOfArc) / 4;

   double angToUse1 = partSpacing1 / radiusOfArc;
   double angToUse2 = partSpacing2 / radiusOfArc;
   for(unsigned i = 1; i <= 4; i++){
      point tempPoint1;
      point tempPoint2;
      angToUse1 = angToUse2 * i;
      angToUse2 = angToUse2 * i;

      tempPoint1.log = beg.log + (radiusOfArc * sin(angToUse1));
      tempPoint1.lat = beg.lat - (radiusOfArc * (1 - cos(angToUse1)));
      toReturn.at(i) = tempPoint1;
      tempPoint2.log = mid.log + (radiusOfArc * sin(angToUse2));
      tempPoint2.lat = mid.lat - (radiusOfArc * (1 - cos(angToUse2)));
      toReturn.at(i + 5) = tempPoint2;
   }
   return toReturn;
}

// Vector -> * length -> add vector to original point for new point
vector<point> radialRevision(point& clearB, point& conf, point& clearE, obstacle& inWay){
   point centObs(inWay.lat, inWay.log);
   trajectory radExt(centObs, conf);
   radExt.coefficientI = (radExt.coefficientI * (inWay.radius + 50));
   radExt.coefficientJ = (radExt.coefficientJ * (inWay.radius + 50));
   point confExt((radExt.coefficientI + inWay.lat), (radExt.coefficientJ + inWay.log), conf.height);
   //if(URGENT: must check if out of bounds) {
   //    radExt.coefficientI *= -1;
   //    radExt.coefficientJ *= -1;
   //    confExt.lat = radExt.coefficientI + inWay.lat;
   //    confExt.log = radExt.coefficientJ + inWay.log; 
   // }
   // check if in other obstacles?
   return arcTurn(clearB, confExt, clearE);
}

void routeWritter(list<waypoint>& wayPointsFin){
   cout << "Writing file OwO\n";
   ofstream write(finFileName());
   if(!write.is_open()){
      cout << "\nError opening file for final output. Terminating...\n";
      exit(1);
   } 

   write << "{\n\t\"fileType\": \"Plan\",\n";
   write << "\"geoFence\": {\n";
   write << "\t\t\"cirarcTurncles\": [";
   write << "\t\t],\n";
   write << "\t\t\"polygons\": [";
   write << "\t\t],\n";
   write << "\n\"version\": 2\n\t},\n";
   write << "\t\"groundStation\": \"QGroundControl\",\n";
   write << "\t\"mission\": {\n";
   write << "\t\t\"cruiseSpeed\": 12.0,\n";
   write << "\t\t\"firmwareType\": 3,\n";
   write << "\t\t\"hoverSpeed\": 5,";
   write << "\t\t\"items\": [\n";
   write << "\t\t\t{\n";
   write << "\t\t\t\t\"autoContinue\": true,\n";
   write << "\t\t\t\t\"command\": 22,\n";
   write << "\t\t\t\t\"doJumpId\": 1,\n";
   write << "\t\t\t\t\"frame\": 3,\n";
   write << "\t\t\t\t\"params\": [\n";
   write << "\t\t\t\t\t 15,\n"; // takeoff pitch
   write << "\t\t\t\t\t 0,\n";
   write << "\t\t\t\t\t 0,\n";
   write << "\t\t\t\t\t null,\n";
   write << "\t\t\t\t\t 0,\n";
   write << "\t\t\t\t\t 0,\n";
   write << "\t\t\t\t\t 60.96,\n"; // height in meters
   write << "\t\t\t\t],\n";
   write << "\t\t\t\t\"type\": \"SimpleItem\"\n";
   write << "\t\t\t},\n";
   
   int lastTemp = 1;
   for(std::list<waypoint>::iterator it= wayPointsFin.begin(); it != wayPointsFin.end(); ++it){
      pointWriter(write,*it);
      lastTemp = it->sequenceNumber;
   }

   write << "\t\t\t{\n";
   write << "\t\t\t\t\"autoContinue\": true,\n";
   write << "\t\t\t\t\"command\": 22,\n";
   write << "\t\t\t\t\"doJumpId\": ";
   write << lastTemp << ",\n";
   write << "\t\t\t\t\"frame\": 3,\n";
   write << "\t\t\t\t\"params\": [\n";
   write << "\t\t\t\t\t 15,\n"; // takeoff pitch
   write << "\t\t\t\t\t 0,\n";
   write << "\t\t\t\t\t 0,\n";
   write << "\t\t\t\t\t null,\n";
   write << "\t\t\t\t\t 0,\n";
   write << "\t\t\t\t\t 0,\n";
   write << "\t\t\t\t\t 60.96,\n"; // height in meters
   write << "\t\t\t\t],\n";
   write << "\t\t\t\t\"type\": \"SimpleItem\"\n";
   write << "\t\t\t}\n";

   write << "\t\t],\n";
   write << "\t\t\"plannedHomePosition\": [\n";
   write << "\t\t\t38.14575538617687,\n";
   write << "\t\t\t-76.42885386,\n";
   write << "\t\t\t3\n";
   write << "\t\t\"vehicleType\": 1,\n";
   write << "\t\t\"versio\": 2\n";
   write << "\t},\n";
   write << "\t\"rallyPoints\": {\n";
   write << "\t\t\"points\": [\n";
   write << "\t\t],\n";
   write << "\t\t\"version\": 2\n";
   write << "\t},\n";
   write << "\t\"version\": 1\n";
   write << "}\n";
   write.close();
   
   cout << "File written :D. Happy Travels" << endl;
}

void pointWriter(ostream& out, const waypoint& toWriteq){
   out << "\t\t\t{\n";
   out << "\t\t\t\t\"AMSLAltAboveTerrain\": ";
   out << toWriteq.height/3.2808 << ',' << endl;
   out << "\t\t\t\t\"Altitude\": ";
   out << toWriteq.height/3.2808 << ',' << endl;
   out << "\t\t\t\t\"AltitudeMode\": 1,\n";
   out << "\t\t\t\t\"autoContinue\": true,\n";
   out << "\t\t\t\t\"command\": 16,\n";
   out << "\t\t\t\t\"doJumpId\": " << toWriteq.sequenceNumber;
   out << ",\n";
   out << "\t\t\t\t\"frame\": 3,\n";
   out << "\t\t\t\t\"params\": [\n";
   out << "\t\t\t\t\t0,\n";
   out << "\t\t\t\t\t0,\n";
   out << "\t\t\t\t\t0,\n";
   out << "\t\t\t\t\tnull,\n"; 
   out << "\t\t\t\t\t" << toWriteq.lat << endl; // latitude, y
   out << "\t\t\t\t\t" << toWriteq.log << endl; // longitude, x
   out << "\t\t\t\t\t" << toWriteq.height/3.2808 << endl;
   out << "\t\t\t\t],\n";
   out << "\t\t\t\t\"type\": \"SimpleItem\"\n";
   out << "\t\t\t},\n";
}


string finFileName(){
   cout << "What would you like to call the file? (No spaces, just the name)\nDo not include an extension!\n" << endl;
   string temp;
   getline(cin, temp);
   temp += ".plan";
   return temp;
}

vector<point> boundaryReader(string fileName){
   point temp; 
   vector<point> temps;
   ifstream read(fileName);

   if(!read.is_open()){
      cout << "Error opening file. Terminating..." << endl;
      exit(1); 
   }

   double deg;//reads in degrees
   double min;//reads in minutes
   double sec;//reads in seconds
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
         temps.push_back(temp);
      }
   }
   read.close();
   return temps;
}

double findIntercept(double distance, point& point)
{
    double y = point.lat;
    double x = point.log;

    double intercept = y - (distance * x);
    return intercept;

}

bool intersection(point& begPath, point& endPath, point& begBound, point& endBound)
{
    double m1;
    double m2;
    double b1;
    double b2; 

    m1 = distanceFt(begPath, endPath);
    m2 = distanceFt(begBound, endBound);
    
    if (m1 == m2)
    {
        cout << "They do not intersect, they are parallel" << endl;
        return false;
    }

    b1 = findIntercept(m1, endPath);
    b2 = findIntercept(m2, endBound);

    double x = (b2-b1)/(m1-m2);
    double y = (m1*x) + b1;

    point intersect;
    intersect.log = x;
    intersect.lat = y; 

    point mid;
    mid.lat = (begBound.lat + endBound.lat) / 2;
    mid.log = (begBound.log + endBound.log) / 2;
    double d1 = distanceFt(mid,intersect);
    double d2 = distanceFt(begBound,mid);
    
    if (d1 <= d2)
    {
        return true;
    }
    else
    { 
        return false;
    }
}
