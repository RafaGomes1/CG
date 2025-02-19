//
// Created by mikep on 01-03-2024.
//

#include "headers/cone.h"
#include "math.h"
#include <iostream>

using namespace std;


Point Polcar2Carthesian(float radius, float alpha, float y)
{
    float px = radius * sin(alpha);
    float pz = radius * cos(alpha);

    return {px,y,pz};
}

Cone::Cone(float r, float h, int sl, int st) {
    radius = r;
    height = h;
    slices = sl;
    stacks = st;
}

Point Cone::cross(Point& a, Point& b) {
    Point result = Point(a.getY() * b.getZ() - a.getZ() * b.getY(),
                         a.getZ() * b.getX() - a.getX() * b.getZ(),
                         a.getX() * b.getY() - a.getY() * b.getX());
    return result;
}

Point Cone::normalize(Point& a) {
    float length = std::sqrt(a.getX() * a.getX() + a.getY() * a.getY() + a.getZ() * a.getZ());

    if (length > 0.0f) {
        return Point(a.getX() / length, a.getY() / length, a.getZ() / length);
    }
    return a;
}

// Point Cone::calcNormal(Point& p1, Point& p2, Point& p3) {
//     Point u = p2 - p1;
//     Point v = p3 - p1;
//     Point normal = cross(u, v);
//     Point normalized = normalize(normal);
//     return normalized;
// }

Point Cone::calcNormal(Point& p1, Point& p2, Point& p3) {
    Point A = p1 - p2;
    Point B = p3 - p2;
    Point C = p3 - p1;

    Point R = cross(A,B);
    Point normal = cross(C,R);
    Point normalized = normalize(normal);

    return normalized;
}

std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> Cone::genPoints() {
    std::vector<Point> points;
    std::vector<Point> texCoords;
    std::vector<Point> normals;

    float alfa_diff = (2 * M_PI) / slices;
    float height_diff = height / stacks;
    float radius_diff = radius / stacks;

    // Base
    for (int i = 0; i < slices; i++) {
        float alfa = i * alfa_diff;
        float next_alfa = (i + 1) * alfa_diff;

        points.push_back(Point{0.0f, 0.0f, 0.0f});
        points.push_back(Polcar2Carthesian(radius, next_alfa, 0.0f));
        points.push_back(Polcar2Carthesian(radius, alfa, 0.0f));

        // Normals for the base (pointing downwards)
        normals.push_back(Point(0, -1, 0));
        normals.push_back(Point(0, -1, 0));
        normals.push_back(Point(0, -1, 0));

        texCoords.push_back(Point(0.5f, 0.5f, 0));
        texCoords.push_back(Point(0.5f + 0.5f * cos(next_alfa), 0.5f + 0.5f * sin(next_alfa), 0));
        texCoords.push_back(Point(0.5f + 0.5f * cos(alfa), 0.5f + 0.5f * sin(alfa), 0));
    }

    // Sides
    for (int i = 0; i < stacks; i++) {
        float rad = radius - radius_diff * i;
        float nextRad = radius - (radius / stacks) * (i + 1);
        float height_ = i * height_diff;
        float nextHeight = (i + 1) * height_diff;

        for (int j = 0; j < slices; j++) {

            if (i == stacks-1){
                float alfa = j * alfa_diff;
                float next_alfa = (j + 1) * alfa_diff;
              float u1 = (float)j / slices;
            float u2 = (float)(j + 1) / slices;
            float v1 = (float)i / stacks;
            float v2 = (float)(i + 1) / stacks;

                Point p1 = Polcar2Carthesian(rad,next_alfa,height_); // Ponta parte baixo direita
                Point p2 = Polcar2Carthesian(nextRad,alfa,nextHeight); // Ponta parte cima direita

                Point p3 = Polcar2Carthesian(rad,alfa,height_); // Ponta parte baixo esquerda
                Point p4 = Polcar2Carthesian(nextRad,next_alfa,nextHeight); // Ponta parte cima esqueda

                Point orig{0.0f,0.0f,0.0f};
                Point apex{0.0f,height,0.0f};

                Point n1 = calcNormal(p1,orig,apex);
                Point n2 = calcNormal(p2,orig,apex);
                Point n3 = calcNormal(p3,orig,apex);
                Point n4 = calcNormal(p4,orig,apex);

                points.push_back(p1);
                points.push_back(p2);
                points.push_back(p3);

                points.push_back(p1);
                points.push_back(p4);
                points.push_back(p2);


                normals.push_back(n1);
                normals.push_back(Point(0,1,0));
                normals.push_back(n3);

                normals.push_back(n1);
                normals.push_back(Point(0,1,0));
                normals.push_back(n2);

                texCoords.push_back(Point(u2, v1, 0));
            texCoords.push_back(Point(u1, v2, 0));
            texCoords.push_back(Point(u1, v1, 0));

            texCoords.push_back(Point(u2, v1, 0));
            texCoords.push_back(Point(u2, v2, 0));
            texCoords.push_back(Point(u1, v2, 0));
            }
            else{
                float alfa = j * alfa_diff;
                float next_alfa = (j + 1) * alfa_diff;
              float u1 = (float)j / slices;
            float u2 = (float)(j + 1) / slices;
            float v1 = (float)i / stacks;
            float v2 = (float)(i + 1) / stacks;

                Point p1 = Polcar2Carthesian(rad,next_alfa,height_); // Ponta parte baixo direita
                Point p2 = Polcar2Carthesian(nextRad,alfa,nextHeight); // Ponta parte cima direita

                Point p3 = Polcar2Carthesian(rad,alfa,height_); // Ponta parte baixo esquerda
                Point p4 = Polcar2Carthesian(nextRad,next_alfa,nextHeight); // Ponta parte cima esqueda

                Point orig{0.0f,0.0f,0.0f};
                Point apex{0.0f,height,0.0f};

                Point n1 = calcNormal(p1,orig,apex);
                Point n2 = calcNormal(p2,orig,apex);
                Point n3 = calcNormal(p3,orig,apex);
                Point n4 = calcNormal(p4,orig,apex);

                points.push_back(p1);
                points.push_back(p2);
                points.push_back(p3);

                points.push_back(p1);
                points.push_back(p4);
                points.push_back(p2);

                // Point normal1 = calcNormal(p1, p3, p2);
                // Point normal2 = calcNormal(p1, p2, p4);

                normals.push_back(n1);
                normals.push_back(n2);
                normals.push_back(n3);

                normals.push_back(n1);
                normals.push_back(n4);
                normals.push_back(n2);

                // Coordenadas de textura para as laterais
                texCoords.push_back(Point(u2, v1, 0));
            texCoords.push_back(Point(u1, v2, 0));
            texCoords.push_back(Point(u1, v1, 0));

            texCoords.push_back(Point(u2, v1, 0));
            texCoords.push_back(Point(u2, v2, 0));
            texCoords.push_back(Point(u1, v2, 0));
            }
        }
    }

    return std::make_tuple(points, normals, texCoords);
}

