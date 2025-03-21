//
// Created by mikep on 23-02-2024.
//

#ifndef FASE1_PLANE_H
#define FASE1_PLANE_H
#include "primitive.h"

class Plane : public Primitive {
    private:
        float size;
        int div;

    public:
        Plane(float , int);
        std::vector<Point> genPoints();
};

#endif //FASE1_PLANE_H
