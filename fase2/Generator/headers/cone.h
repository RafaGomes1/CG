//
// Created by mikep on 01-03-2024.
//

#ifndef CLASS2_CONE_H
#define CLASS2_CONE_H

#include "primitive.h"

class Cone : public Primitive{
private:
    float radius;
    float height;
    int slices;
    int stacks;

public:
    Cone(float r, float h, int sl, int st);
    std::vector<Point> genPoints();
};


#endif //CLASS2_CONE_H
