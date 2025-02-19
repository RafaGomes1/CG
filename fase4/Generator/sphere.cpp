//
// Created by mikep on 29-02-2024.
//

#include "headers/sphere.h"
#include "math.h"

using namespace std;

Sphere::Sphere(float r, int sl, int st) {
    radius = r;
    slices = sl;
    stacks = st;
}

Point Spherical2Carthesian(float radius, float alpha, float beta)
{
    float px = radius * cos(beta) * sin(alpha);
    float py = radius * sin(beta);
    float pz = radius * cos(beta) * cos(alpha);

    return {px,py,pz};
}

/* std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> Sphere::genPoints() {
    std::vector<Point> points;
    std::vector<Point> normals;
    std::vector<Point> texCoords;

    float diff_stacks = M_PI / stacks;
    float diff_slices = (2.0f * M_PI) / slices;
    bool isStackEven = (stacks % 2) == 0;
    float halfStacks = stacks / 2;
    float betaUp = 0.0;
    float betaDown = 0.0;

    for (int i = 0; i < slices; i++) {
        if (!isStackEven) {
            betaUp = diff_stacks / 2;
            betaDown = -betaUp;

            Point p1 = Spherical2Carthesian(radius, i * diff_slices, betaDown);
            Point p2 = Spherical2Carthesian(radius, (i + 1) * diff_slices, betaDown);
            Point p3 = Spherical2Carthesian(radius, i * diff_slices, betaUp);
            Point p4 = Spherical2Carthesian(radius, (i + 1) * diff_slices, betaUp);

            points.push_back(p1);
            points.push_back(p2);
            points.push_back(p3);
            points.push_back(p3);
            points.push_back(p2);
            points.push_back(p4);

            normals.push_back(Point(p1.getX() / radius, p1.getY() / radius, p1.getZ() / radius));
            normals.push_back(Point(p2.getX() / radius, p2.getY() / radius, p2.getZ() / radius));
            normals.push_back(Point(p3.getX() / radius, p3.getY() / radius, p3.getZ() / radius));
            normals.push_back(Point(p3.getX() / radius, p3.getY() / radius, p3.getZ() / radius));
            normals.push_back(Point(p2.getX() / radius, p2.getY() / radius, p2.getZ() / radius));
            normals.push_back(Point(p4.getX() / radius, p4.getY() / radius, p4.getZ() / radius));

            texCoords.push_back(Point((float)i / slices, (float)(halfStacks - 1) / stacks, 0));
            texCoords.push_back(Point((float)(i + 1) / slices, (float)(halfStacks - 1) / stacks, 0));
            texCoords.push_back(Point((float)i / slices, (float)halfStacks / stacks, 0));
            texCoords.push_back(Point((float)i / slices, (float)halfStacks / stacks, 0));
            texCoords.push_back(Point((float)(i + 1) / slices, (float)(halfStacks - 1) / stacks, 0));
            texCoords.push_back(Point((float)(i + 1) / slices, (float)halfStacks / stacks, 0));
        } else {
            betaUp = 0.0f;
            betaDown = 0.0f;
        }

        for (int j = 0; j < halfStacks - 1; j++) {
            Point pSuperior1 = Spherical2Carthesian(radius, i * diff_slices, betaUp);
            Point pSuperior2 = Spherical2Carthesian(radius, (i + 1) * diff_slices, betaUp);
            betaUp += diff_stacks;
            Point pSuperior3 = Spherical2Carthesian(radius, i * diff_slices, betaUp);
            Point pSuperior4 = Spherical2Carthesian(radius, (i + 1) * diff_slices, betaUp);

            Point pInferior1 = Spherical2Carthesian(radius, i * diff_slices, betaDown);
            Point pInferior2 = Spherical2Carthesian(radius, (i + 1) * diff_slices, betaDown);
            betaDown -= diff_stacks;
            Point pInferior3 = Spherical2Carthesian(radius, (i + 1) * diff_slices, betaDown);
            Point pInferior4 = Spherical2Carthesian(radius, i * diff_slices, betaDown);

            points.push_back(pSuperior1);
            points.push_back(pSuperior2);
            points.push_back(pSuperior3);
            points.push_back(pSuperior3);
            points.push_back(pSuperior2);
            points.push_back(pSuperior4);

            normals.push_back(Point(pSuperior1.getX() / radius, pSuperior1.getY() / radius, pSuperior1.getZ() / radius));
            normals.push_back(Point(pSuperior2.getX() / radius, pSuperior2.getY() / radius, pSuperior2.getZ() / radius));
            normals.push_back(Point(pSuperior3.getX() / radius, pSuperior3.getY() / radius, pSuperior3.getZ() / radius));
            normals.push_back(Point(pSuperior3.getX() / radius, pSuperior3.getY() / radius, pSuperior3.getZ() / radius));
            normals.push_back(Point(pSuperior2.getX() / radius, pSuperior2.getY() / radius, pSuperior2.getZ() / radius));
            normals.push_back(Point(pSuperior4.getX() / radius, pSuperior4.getY() / radius, pSuperior4.getZ() / radius));

            float u1 = (float)i / slices;
            float u2 = (float)(i + 1) / slices;
            float v1 = halfStacks/(stacks)  + (float)j / stacks;
            float v2 = halfStacks/(stacks)  + (float)(j+1) / stacks;

            texCoords.push_back(Point(u1, v1, 0));
            texCoords.push_back(Point(u2, v1, 0));
            texCoords.push_back(Point(u1, v2, 0));

            texCoords.push_back(Point(u1, v2, 0));
            texCoords.push_back(Point(u2, v1, 0));
            texCoords.push_back(Point(u2, v2, 0));

            points.push_back(pInferior2);
            points.push_back(pInferior1);
            points.push_back(pInferior3);

            points.push_back(pInferior3);
            points.push_back(pInferior1);
            points.push_back(pInferior4);

            normals.push_back(Point(pInferior2.getX() / radius, pInferior2.getY() / radius, pInferior2.getZ() / radius));
            normals.push_back(Point(pInferior1.getX() / radius, pInferior1.getY() / radius, pInferior1.getZ() / radius));
            normals.push_back(Point(pInferior3.getX() / radius, pInferior3.getY() / radius, pInferior3.getZ() / radius));
            normals.push_back(Point(pInferior3.getX() / radius, pInferior3.getY() / radius, pInferior3.getZ() / radius));
            normals.push_back(Point(pInferior1.getX() / radius, pInferior1.getY() / radius, pInferior1.getZ() / radius));
            normals.push_back(Point(pInferior4.getX() / radius, pInferior4.getY() / radius, pInferior4.getZ() / radius));

            texCoords.push_back(Point(u2, v1 - 2*(float)j / stacks, 0));
            texCoords.push_back(Point(u1, v1 - 2*(float)j / stacks, 0));
            texCoords.push_back(Point(u2, v2 - 2*(float)(j+1) / stacks, 0));

            texCoords.push_back(Point(u2, v2 - 2*(float)(j+1) / stacks, 0));
            texCoords.push_back(Point(u1, v1 - 2*(float)j / stacks, 0));
            texCoords.push_back(Point(u1, v2 - 2*(float)(j+1) / stacks, 0));

            

            if (j == (halfStacks - 2)) {
                Point topo = {0.0f, radius, 0.0f};
                Point baixo = {0.0f, -radius, 0.0f};

                points.push_back(pSuperior3);
                points.push_back(pSuperior4);
                points.push_back(topo);
                points.push_back(baixo);
                points.push_back(pInferior3);
                points.push_back(pInferior4);

                normals.push_back(Point(topo.getX() / radius, topo.getY() / radius, topo.getZ() / radius));
                normals.push_back(Point(pSuperior3.getX() / radius, pSuperior3.getY() / radius, pSuperior3.getZ() / radius));
                normals.push_back(Point(pSuperior4.getX() / radius, pSuperior4.getY() / radius, pSuperior4.getZ() / radius));
                
                normals.push_back(Point(baixo.getX() / radius, baixo.getY() / radius, baixo.getZ() / radius));
                normals.push_back(Point(pInferior3.getX() / radius, pInferior3.getY() / radius, pInferior3.getZ() / radius));
                normals.push_back(Point(pInferior4.getX() / radius, pInferior4.getY() / radius, pInferior4.getZ() / radius));

                texCoords.push_back(Point((float)i / slices, 0.5f + (halfStacks - 1) / stacks, 0));
                texCoords.push_back(Point((float)(i + 1) / slices, 0.5f + (halfStacks - 1) / stacks, 0));
                texCoords.push_back(Point(0.5f, 1.0f, 0));
                
                texCoords.push_back(Point(0.5f, 0.0f, 0));
                texCoords.push_back(Point((float)i / slices, 0.5f - (halfStacks - 1) / stacks, 0));
                texCoords.push_back(Point((float)(i + 1) / slices, 0.5f - (halfStacks - 1) / stacks, 0));
                
            }
        }
    }

    return std::make_tuple(points, normals, texCoords);
} */

Point Sphere::normalize(Point a) {
    float length = std::sqrt(a.getX() * a.getX() + a.getY() * a.getY() + a.getZ() * a.getZ());
    if (length > 0.0f) {
        return Point(a.getX() / length, a.getY() / length, a.getZ() / length);
    }
    return a;
}

std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> Sphere::genPoints() {
    std::vector<Point> points, normals, tex_coords;

    float new_alfa = (2 * M_PI) / slices;
    float new_beta = M_PI / stacks;

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            float alfa_j = new_alfa * j;
            float alfa_j1 = new_alfa * (j + 1);
            float beta_i = new_beta * i;
            float beta_i1 = new_beta * (i + 1);

            Point p1(radius * sin(beta_i) * cos(alfa_j), radius * cos(beta_i), radius * sin(beta_i) * sin(alfa_j));
            Point p2(radius * sin(beta_i) * cos(alfa_j1), radius * cos(beta_i), radius * sin(beta_i) * sin(alfa_j1));
            Point p3(radius * sin(beta_i1) * cos(alfa_j), radius * cos(beta_i1), radius * sin(beta_i1) * sin(alfa_j));
            Point p4(radius * sin(beta_i1) * cos(alfa_j1), radius * cos(beta_i1), radius * sin(beta_i1) * sin(alfa_j1));

            float t1 = static_cast<float>(j) / slices;
            float t2 = static_cast<float>(j + 1) / slices;
            float t3 = 1.0f - static_cast<float>(i) / stacks;
            float t4 = 1.0f - static_cast<float>(i + 1) / stacks;

            points.insert(points.end(), {p2, p4, p3, p3, p1, p2});
            normals.insert(normals.end(), {normalize(p2), normalize(p4), normalize(p3), normalize(p3), normalize(p1), normalize(p2)});
            tex_coords.insert(tex_coords.end(), {Point(t2, t3, 0), Point(t2, t4, 0), Point(t1, t4, 0), Point(t1, t4, 0), Point(t1, t3, 0), Point(t2, t3, 0)});
        }
    }

    return std::make_tuple(points, normals, tex_coords);
}