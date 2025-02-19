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
    GLfloat diffusive[4];
    GLfloat ambient[4];
    GLfloat specular[4];
    GLfloat emissive[4];
    GLfloat shininess;
    std::string textureFile;
    GLuint vbo[3];
    unsigned int texture;
    bool has_texture = false;
    size_t vertice_num;

public:
    Model(std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>>& data, float r1, float g1, float b1, 
          GLfloat diff[], GLfloat amb[], GLfloat spec[], GLfloat emis[], 
          GLfloat shininess, const std::string& texFile = "");
    void addPoint(Point p);
    std::vector<Point> getPoints();
    void draw();
    void clear();
};

#endif //CG_FASE_3_MODEL_H
