#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class point{               //specifies a point in space in decimal degrees
 public:
    double lat;
	double log;
    double height;
    bool crit;
    point(): lat(0), log(0), height(0), crit(0){};
    point(double lati, double lo, double hi = 200): lat(lati), log(lo), crit(false){};
};

class waypoint : public point{
 public:
    int sequenceNumber;
    waypoint(): point(), sequenceNumber(-1){};
    waypoint(double lati, double lo, int seq, double hi = 200): point(lati, lo, hi), sequenceNumber(seq){};    //what number are we along route
};

class obstacle : public point{
 public:
    double radius;
};

class trajectory{
 public:
    double coefficientI;
    double coefficientJ;
    double coefficientK;
    trajectory();
    trajectory(double, double, double);
    trajectory(const point&, const point&);
    double angleBetween(const trajectory&);
};

std::list<point> subdivideCircle(const obstacle&, int numPoints);

std::vector<obstacle> readObstacles(std::string);

std::vector<obstacle> readObstacles2(std::string);

std::list<point> readPoints(std::string);

std::list<point> readPoints2(std::string);

int collides(const point&, const std::vector<obstacle>&);

double distanceFt(const point&, const point&);

point midpoint(const point&, const point&);

bool pathCheckClear(const point&, const point&, const obstacle&);

bool turnAhead(const point&, const point&, const point&);

double turnAngleMax(const point&, const point&);

std::vector<point> arcTurn(point&, point&, point&);

std::vector<point> radialRevision(point&, point&, point&, obstacle&);

std::vector<point> radialRevision(point& clearB, point& conf, point& clearE, obstacle& inWay);

void routeWritter(std::list<waypoint>& );

void wayWritter(const point);

void dropWritter(const point);

std::string finFileName();

void pointWriter(std::ostream&, const waypoint&);

#endif