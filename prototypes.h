#include <list>
#include <vector>
#include <string>

class point{               //specifies a point in space in decimal degrees
 public:
    double lat;
	double log;
    double height;
    bool crit;
    point();
    point(double lati, double lo, double hi = 0): lat(lati), log(lo), crit(false){};
};

class waypoint : public point{
 public:
    int sequenceNumber; 
    waypoint(double lati, double lo, int seq, double hi = 0): point(lati, lo, hi), sequenceNumber(seq){};    //what number are we along route
};

class obstacle : public point{
 public:
    double radius;
};

struct collision {          //is the object that defines a detected collision
    obstacle    danger;     //the object that will be hit in a direct route
    waypoint       first;   //from the point first
    waypoint       last;    //to the point last
};


std::list<point> subdivideCircle(const obstacle&, int numPoints);

std::vector<collision> detect(std::list<waypoint> originalRoute, std::vector<obstacle> obstacles);

std::vector<obstacle> readObstacles(std::string);

std::vector<obstacle> readObstacles2(std::string);

std::list<point> readPoints(std::string);

int colides(const point&, vector<obstacle>&);

double distanceFt(point&, point&);

point midpoint(point&, point&);

bool pathCheckClear(const point&, const point&, const obstacle&);