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

   cout << "Preparing route creation containers...\n";
   while(!points.empty()){
      stackOfPoints.push(points.back());
      points.pop_back();
   }
   cout << "Containers filled successfuly.\n";
   
   cout << "Initializing route creation. Please wait, this may be a few minutes.\n";
   point tempPoint1;
   point tempPoint2;
   point tempForTurn;
   waypoint tempWay;
   bool badPath = false;
   int col = -1;
   vector<point> tempPoints;
   int i = 0;
   int cnt = 2;
   while(!stackOfPoints.empty()){
      tempPoint1 = stackOfPoints.top();
      stackOfPoints.pop();
      tempPoint2 = stackOfPoints.top();
      stackOfPoints.pop();
      badPath = false;
      
      col = -1;
      col = collides(tempPoint2, obstacles);
      if(col != -1){
         cout << "Problem encountered. Fixing..." << endl;
         tempForTurn = stackOfPoints.top();
         stackOfPoints.pop();
         tempPoints = radialRevision(tempPoint1, tempForTurn, tempPoint2, obstacles.at(col));
         while(!tempPoints.empty()){
            stackOfPoints.push(*tempPoints.end());
            tempPoints.pop_back();
         }
      }
      else{
         for(i = 0; (i < obstacles.size()) && !badPath; i++){
            if(!pathCheckClear(tempPoint1, tempPoint2, obstacles.at(i))){
               badPath = true;
               tempForTurn = midpoint(tempPoint1, tempPoint2);
            }
         }
         if(badPath){
            stackOfPoints.push(tempPoint1);
            stackOfPoints.push(tempForTurn);
            stackOfPoints.push(tempPoint2);
         }
         else{
            tempWay = waypoint(tempPoint1.lat, tempPoint1.log, cnt++, tempPoint1.height);
            waypoints.push_back(tempWay);
         }
      }
   }
   routeWritter(waypoints);
}

int main() {
   masterRouteMaker();
   return 0;
}