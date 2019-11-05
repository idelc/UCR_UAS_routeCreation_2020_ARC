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
    point(double lati, double lo, double hi = 200): lat(lati), log(lo), crit(false){};
};

class waypoint : public point{
 public:
    int sequenceNumber; 
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

int collides(const point&, vector<obstacle>&);

double distanceFt(point&, point&);

point midpoint(point&, point&);

bool pathCheckClear(const point&, const point&, const obstacle&);

bool turnAhead(const point&, const point&, const point&);

double turnAngleMax(const point&, const point&);

vector<point> arcTurn(point&, point&, point&);

vector<point> radialRevision(point&, point&, point&, obstacle&);

// waypointRadial Revision? If all else is done, plan and implement

vector<waypoint> routeCreation();

void routeWritter();

void wayWritter(const point);

void dropWritter(const point);

string finFileName();