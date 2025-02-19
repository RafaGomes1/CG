//
// Created by mikep on 01-03-2024.
//

#ifndef CLASS2_CONE_H
#define CLASS2_CONE_H

#include "primitive.h"
#include <tuple>

class Cone : public Primitive{
private:
    float radius;
    float height;
    int slices;
    int stacks;

public:
    Cone(float r, float h, int sl, int st);
    std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> genPoints();
    Point normalize(Point& a);
    Point cross(Point& a, Point& b);
    Point calcNormal(Point& p1, Point& p2, Point& p3);
};


#endif //CLASS2_CONE_H
