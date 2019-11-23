#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>

#include "prototypes.h"

using namespace std;

void masterRouteMaker(){
   char trash;
   string fileName;
   vector<obstacle> obstacles;
   list<point> points;
   list<waypoint> waypoints;
   stack<point> stackOfPoints;

   cout << "MarshManeuvers\nUniversity of California, Riverside - Unmanned Aerial Systems 2020\n\n";
   cout << "Hello!\nPlease ensure connection to plane is established and preparation checklist has been successfuly completed.\n";
   cout << "Press any key to continue\n";
   cin >> trash; // TODO: can I output backspace?
   cout << endl;
   cout << "Please verify that the files containg obstacles and mandatory waypoints are in the same folder as this program\n";
   cout << "Press any key to continue\n";
   cin >> trash;
   cout << endl;
   cout << "Preparing route generation...\n";

   cout << "Please enter the full name of the obstacle file: ";
   getline(cin, fileName);
   cout << endl;
   obstacles = readObstacles(fileName);
   cout << "Obstacles successfully read.\n";

   cout << "Please enter the full name of the point file: ";
   getline(cin, fileName);
   cout << endl;
   points = readPoints(fileName);
   cout << "Points successfully read.\n";

   cout << "Preparing dynamic creation containers\n";
   
}