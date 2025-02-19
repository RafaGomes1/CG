//
// Created by mikep on 26-03-2024.
//
#include <GL/glut.h>
#include <iostream>
#include <utility>
#include "headers/Model.h"

Model::Model(std::vector<Point> pts, float r1, float g1, float b1) {
    points = std::move(pts);
    r = r1;
    g = g1;
    b = b1;
}

std::vector<Point> Model::getPoints() {
    return points;
}

void Model::addPoint(Point p) {
    points.push_back(p);
}

void Model::draw(){
    int j = 0;
    glColor3f(r,g,b);
    for (auto i = points.begin(); i != points.end(); i += 3){
        Point p1 = *i;
        Point p2 = *(i + 1);
        Point p3 = *(i + 2);

        glBegin(GL_TRIANGLES);
        glVertex3f(p1.getX(),p1.getY(),p1.getZ());
        glVertex3f(p2.getX(),p2.getY(),p2.getZ());
        glVertex3f(p3.getX(),p3.getY(),p3.getZ());
        glEnd();

        j += 3;
    }
    //std::cout << "desenhei modelo" << "\n";
}