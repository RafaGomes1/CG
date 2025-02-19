//
// Created by mikep on 23-02-2024.
//

#include "headers/plane.h"
#include "../Utils/point.h"

Plane::Plane(float s, int d) {
    size = s;
    div = d;
}

std::vector<Point> Plane::genPoints() {
    std::vector<Point> points;

    float length = size / 2.0;
    float square_size = size / div;
    float x1 = 0, x2 = 0, z1 = 0, z2 = 0;

    for (int i = 0; i < div; i++) {
        for (int j = 0; j < div; j++)
        {
            x1 = -length + (i * square_size);
            x2 = -length + ((i + 1) * square_size);
            z1 = -length + (j * square_size);
            z2 = -length + ((j + 1) * square_size);

            // Face de cima

            points.push_back(Point(x1,0,-z1));
            points.push_back(Point(x2,0,-z1));
            points.push_back(Point(x1,0,-z2));

            points.push_back(Point(x2,0,-z2));
            points.push_back(Point(x1,0,-z2));
            points.push_back(Point(x2,0,-z1));

        }
    }

    return points;
}