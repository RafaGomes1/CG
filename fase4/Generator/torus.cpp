//
// Created by goncalo on 26-03-2024.
//

#include <math.h>
#include "headers/torus.h"

/*
 * x = (R1 + R2 cos(v)) cos(u)
 * y = (R1 + R2 cos(v)) sin(u)
 * z = R2 sin(v)
 */

Torus::Torus(float radius1, float radius2, int side, int ring) {
    r1 = radius1;
    r2 = radius2;
    sides = side;
    rings = ring;
}

std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> Torus::genPoints() {
    std::vector<Point> points;
    std::vector<Point> texCoords;
    std::vector<Point> normals;
    float u = (M_PI * 2) / rings;
    float v = (M_PI * 2) / sides;

    for(int i = 0; i < rings; i++){
        float curr_u = i * u;

        for(int j = 0; j < sides; j++){
            float curr_v = j * v;

            points.push_back(Point(
                    (r1 + r2 * cos(v + curr_v)) * cos(curr_u),
                    (r1 + r2 * cos(v + curr_v)) * sin(curr_u),
                    r2 * sin(v+curr_v)
            ));
            points.push_back(Point(
                    (r1 + r2 * cos(curr_v)) * cos(curr_u),
                    (r1 + r2 * cos(curr_v)) * sin(curr_u),
                    r2 * sin(curr_v)
            ));
            points.push_back(Point(
                    (r1 + r2 * cos(curr_v)) * cos(u+curr_u),
                    (r1 + r2 * cos(curr_v)) * sin(u+curr_u),
                    r2 * sin(curr_v)
            ));


            points.push_back(Point(
                    (r1 + r2 * cos(v + curr_v)) * cos(curr_u),
                    (r1 + r2 * cos(v + curr_v)) * sin(curr_u),
                    r2 * sin(v+curr_v)
            ));
            points.push_back(Point(
                    (r1 + r2 * cos(curr_v)) * cos(u+curr_u),
                    (r1 + r2 * cos(curr_v)) * sin(u+curr_u),
                    r2 * sin(curr_v)
            ));
            points.push_back(Point(
                    (r1 + r2 * cos(v+curr_v)) * cos(u+curr_u),
                    (r1 + r2 * cos(v+curr_v)) * sin(u+curr_u),
                    r2 * sin(v+curr_v)
            ));

            // Normais
            normals.push_back(Point(cos(v + curr_v) * cos(curr_u), cos(v + curr_v) * sin(curr_u), sin(v + curr_v)));
            normals.push_back(Point(cos(curr_v) * cos(curr_u), cos(curr_v) * sin(curr_u), sin(curr_v)));
            normals.push_back(Point(cos(curr_v) * cos(u + curr_u), cos(curr_v) * sin(u + curr_u), sin(curr_v)));
            normals.push_back(Point(cos(curr_v) * cos(u + curr_u), cos(curr_v) * sin(u + curr_u), sin(curr_v)));
            normals.push_back(Point(cos(v + curr_v) * cos(u + curr_u), cos(v + curr_v) * sin(u + curr_u), sin(v + curr_v)));
            normals.push_back(Point(cos(v + curr_v) * cos(curr_u), cos(v + curr_v) * sin(curr_u), sin(v + curr_v)));

            // Coordenadas de textura
            texCoords.push_back(Point((float)i / rings, (float)(j + 1) / sides, 0));
            texCoords.push_back(Point((float)(i + 1) / rings, (float)j / sides, 0));
            texCoords.push_back(Point((float)i / rings, (float)j / sides, 0));
            texCoords.push_back(Point((float)i / rings, (float)(j + 1) / sides, 0));
            texCoords.push_back(Point((float)(i + 1) / rings, (float)(j + 1) / sides, 0));
            texCoords.push_back(Point((float)(i + 1) / rings, (float)j / sides, 0));


        }
    }
    return std::make_tuple(points, normals, texCoords);
}
