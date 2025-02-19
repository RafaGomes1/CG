//
// Created by mikep on 23-02-2024.
//

#include "headers/plane.h"
#include "../Utils/point.h"

Plane::Plane(float s, int d) {
    size = s;
    div = d;
}

std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> Plane::genPoints() {
    std::vector<Point> points;
    std::vector<Point> normals;
    std::vector<Point> texCoords;

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

             // Normais
            // Todos os vértices têm a mesma normal (0, 1, 0) porque é um plano horizontal
            normals.push_back(Point(0, 1, 0));
            normals.push_back(Point(0, 1, 0));
            normals.push_back(Point(0, 1, 0));

            normals.push_back(Point(0, 1, 0));
            normals.push_back(Point(0, 1, 0));
            normals.push_back(Point(0, 1, 0));

            // Coordenadas de textura
            // (i / div, j / div) e ((i + 1) / div, (j + 1) / div) mapeiam a textura sobre o plano
            texCoords.push_back(Point((float)i / div, (float)j / div, 0));
            texCoords.push_back(Point((float)(i + 1) / div, (float)j / div, 0));
            texCoords.push_back(Point((float)i / div, (float)(j + 1) / div, 0));

            texCoords.push_back(Point((float)(i + 1) / div, (float)(j + 1) / div, 0));
            texCoords.push_back(Point((float)i / div, (float)(j + 1) / div, 0));
            texCoords.push_back(Point((float)(i + 1) / div, (float)j / div, 0));

        }
    }

    return std::make_tuple(points, normals, texCoords);
}