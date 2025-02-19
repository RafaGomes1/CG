//
// Created by mikep on 29-02-2024.
//

#ifndef CLASS2_SPHERE_H
#define CLASS2_SPHERE_H

#include "primitive.h"
#include <tuple>

class Sphere : public Primitive{
private:
    float radius;
    int stacks;
    int slices;

public:
    Sphere(float, int, int );
    std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> genPoints();
    Point normalize(Point a);
};


#endif //CLASS2_SPHERE_H
