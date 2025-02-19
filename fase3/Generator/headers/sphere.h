//
// Created by mikep on 29-02-2024.
//

#ifndef CLASS2_SPHERE_H
#define CLASS2_SPHERE_H

#include "primitive.h"

class Sphere : public Primitive{
private:
    float radius;
    int stacks;
    int slices;

public:
    Sphere(float, int, int );
    std::vector<Point> genPoints();
};


#endif //CLASS2_SPHERE_H
