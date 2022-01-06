#ifndef POINT_H
#define POINT_H


class Point
{

public:
    Point(){};
    Point(double a,double b,double c):x(a),y(b),z(c){};
public:
    double x;
    double y;
    double z;
};

#endif // POINT_H
