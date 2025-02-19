//
// Created by mikep on 23-02-2024.
//

#ifndef FASE1_PRIMITIVE_H
#define FASE1_PRIMITIVE_H
#include <vector>
#include "../../Utils/point.h"

class Primitive{
    private:
        std::vector<Point> points;

    public:
        std::vector<Point> genPoints();
        std::vector<Point> &getPoints();
        void setPoints(std::vector<Point> &vector);
};


#endif //FASE1_PRIMITIVE_H
