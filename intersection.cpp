#include <iostream>
#include <cmath>
#include <vector>
#include "prototypes.h"

using namespace std;

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

bool overlap (obstacle& input1, obstacle& input2)
{
    
}