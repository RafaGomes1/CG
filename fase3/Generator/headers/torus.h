//
// Created by goncalo on 26-03-2024.
//

#ifndef PROJECT_CG_2324_TORUS_H
#define PROJECT_CG_2324_TORUS_H

#include "primitive.h"

class Torus : public Primitive {
private:
    float r1;
    float r2;
    int sides;
    int rings;

public:
    Torus(float , float, int, int);
    std::vector<Point> genPoints();
};


#endif //PROJECT_CG_2324_TORUS_H
