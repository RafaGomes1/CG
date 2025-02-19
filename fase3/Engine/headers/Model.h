//
// Created by mikep on 26-03-2024.
//

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#ifndef CG_FASE_2_MODEL_H
#define CG_FASE_2_MODEL_H

#include "../../Utils/point.h"
#include "vector"

class Model {
private:
    std::vector<Point> points;
    float r, g, b;
    GLuint vbo[1];

public:
    Model(std::vector<Point> pts, float r1, float g1, float b1);
    void addPoint(Point p);
    std::vector<Point> getPoints();
    void draw();
    void clear();
};

#endif //CG_FASE_3_MODEL_H
