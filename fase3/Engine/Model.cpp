//
// Created by mikep on 26-03-2024.
//
#define GL_GLEXT_PROTOTYPES


#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <utility>
#include "headers/Model.h"

Model::Model(std::vector<Point> pts, float r1, float g1, float b1)
{
    points = std::move(pts);
    r = r1;
    g = g1;
    b = b1;

    glEnableClientState(GL_VERTEX_ARRAY);
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point), &points[0], GL_STATIC_DRAW);
}

std::vector<Point> Model::getPoints() {
    return points;
}

void Model::addPoint(Point p)
{
    points.push_back(p);
}

void Model::draw(){
    int numTriangles = points.size() / 3;
    glColor3f(r,g,b);
    
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glVertexPointer(3, GL_FLOAT, sizeof(Point), 0);

    // Draw using VBO
    glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);
}

void Model::clear(){
    glDeleteBuffers(1, vbo);
}