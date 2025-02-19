//
// Created by goncalo on 24-02-2024.
//

#ifndef CLASS2_BOX_H
#define CLASS2_BOX_H


#include "primitive.h"

class Box : public Primitive {
private:
    float size;
    int div;

public:
    Box(float , int);
    std::vector<Point> genPoints();
};


#endif //CLASS2_BOX_H
