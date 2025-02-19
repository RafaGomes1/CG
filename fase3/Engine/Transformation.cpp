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

#include "headers/Transformation.h"
#include "headers/tinyxml2.h"
#include <string>
#include <iostream>
#include <cmath>
#include "../Utils/MatrixOperations.h"

using std::vector;

void Transformation::transform(float time_elapsed, bool){};

TransformationStatic::TransformationStatic(const char* t, float x_, float y_, float z_, float angle_ = 0.0) {
    type = strdup(t);
    x = x_;
    y = y_;
    z = z_;
    angle = angle_;

    glEnableClientState(GL_VERTEX_ARRAY);

}

TransformationStatic::~TransformationStatic() {
    free((void *) type);
}

void TransformationStatic::transform(float time_elapsed, bool) {
    if(strcmp(type, "translate") == 0){
        glTranslatef(x,y,z);
    }
    else if (strcmp(type, "rotate") == 0){
        glRotatef(angle,x,y,z);
    }
    else if (strcmp(type, "scale") == 0){
        glScalef(x,y,z);
    }
}

TranslationDynamic::TranslationDynamic(float time, vector<Point> p, bool isAligned) {
    duration = time * 1000;
    points = p;
    align = isAligned;
}

TranslationDynamic::~TranslationDynamic() {}
RotationDynamic::~RotationDynamic() {}

void RotationDynamic::transform(float time_elapsed, bool){
    float angle = time_elapsed * (360.0 / duration);
    glRotatef(angle, x, y, z);
}

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }

    }

}

void getCatmullRomPoint(float t, Point zero, Point um, Point dois, Point tres, float *pos, float *dev){
    // catmull-rom matrix
    std::vector<std::vector<float>> m_transpose = {
            {-0.5f,  1.0f, -0.5f,  0.0f},
            { 1.5f, -2.5f,  0.0f,  1.0f},
            {-1.5f,  2.0f,  0.5f,  0.0f},
            { 0.5f, -0.5f,  0.0f,  0.0f}
    };

    std::vector<std::vector<float>> p;
    p.push_back({zero.getX(), zero.getY(), zero.getZ()});
    p.push_back({um.getX(), um.getY(), um.getZ()});
    p.push_back({dois.getX(), dois.getY(), dois.getZ()});
    p.push_back({tres.getX(), tres.getY(), tres.getZ()});

    for (int i = 0; i < 3; i++){
        vector<vector<float>> g = {{p[0][i], p[1][i], p[2][i], p[3][i]}};

        vector<vector<float>> a = MatrixOperations::MultiplyMatrices(g,m_transpose);
        pos[i] = powf(t,3.0) * a[0][0] + powf(t, 2.0) * a[0][1] + t * a[0][2] + a[0][3];
        dev[i] = 3 * powf(t, 2.0) * a[0][0] + 2 * t * a[0][1] + a[0][2];
    }
}


void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, int p_count, vector<Point> points){
    float t = gt * p_count;
    int index = floor(t);
    t = t-index;

    // indices store the points
    int indices[4];
    indices[0] = (index + p_count - 1) % p_count;
    indices[1] = (indices[0] + 1) % p_count;
    indices[2] = (indices[1] + 1) % p_count;
    indices[3] = (indices[2] + 1) % p_count;

    getCatmullRomPoint(t,points[indices[0]], points[indices[1]], points[indices[2]], points[indices[3]], pos, deriv);
}

void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

void renderCatmullRomCurve(int tes, int p_count, vector<Point> points) {

    float gt = 0;
    float pos[3];
    float deriv[3];

    GLuint vbo[1];

    glEnableClientState(GL_VERTEX_ARRAY);
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER,*vbo);

    vector<Point> curvePoints{};

    while (gt < 1)
    {
        getGlobalCatmullRomPoint(gt, pos, deriv, p_count, points);
        curvePoints.emplace_back(pos[0],pos[1],pos[2]);
        gt += 1.0 / tes;

    }
    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(Point), &curvePoints[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glVertexPointer(3, GL_FLOAT, sizeof(Point), 0);
    glColor3f(1,1,1);
    glDrawArrays(GL_LINE_LOOP, 0, curvePoints.size());
}


void TranslationDynamic::transform(float time_elapsed, bool drawCurves) {
    float pos[3], deriv[3];

    if (drawCurves)
    {
        renderCatmullRomCurve(2 * duration/1000, points.size(), points);
    }

    getGlobalCatmullRomPoint(time_elapsed / duration, pos, deriv, points.size(), points);
    glTranslatef(pos[0], pos[1], pos[2]);

    if(align){
        float x[3] = {deriv[0], deriv[1], deriv[2]};
        normalize(x);

        float prev_y[3] = {0.0, 1.0, 0.0};
        float z[3];
        cross(x, prev_y, z);
        normalize(z);


        glRotatef(-90,x[0], x[1], x[2]);

        float y[3];
        cross(z, x, y);
        normalize(y);
        memcpy(prev_y, y, 3 * sizeof(float));

        float m[16];
        buildRotMatrix(x, y, z, m);
        glMultMatrixf(m);
    }

}


