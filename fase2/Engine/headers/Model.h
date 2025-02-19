//
// Created by mikep on 26-03-2024.
//

#ifndef CG_FASE_2_MODEL_H
#define CG_FASE_2_MODEL_H

#include "../../Utils/point.h"
#include "vector"

class Model {
private:
    std::vector<Point> points;
    float r, g, b;
public:
    Model(std::vector<Point> pts, float r1, float g1, float b1);
    void addPoint(Point p);
    std::vector<Point> getPoints();
    void draw();
};


#endif //CG_FASE_2_MODEL_H
