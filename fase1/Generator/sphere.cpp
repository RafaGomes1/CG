//
// Created by mikep on 29-02-2024.
//

#include "headers/sphere.h"
#include "math.h"

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

std::vector<Point> Sphere::genPoints() {
    std::vector<Point> points;

    float diff_stacks = M_PI / stacks;
    float diff_slices = (2.0f * M_PI) / slices;
    bool isStackEven = (stacks % 2) == 0;
    stacks /= 2;
    float betaUp = 0.0;
    float betaDown = 0.0;


    for (int i = 0; i < slices; i++) {

        if (!isStackEven)
        {
            betaUp = diff_stacks / 2;
            betaDown = - betaUp;

            points.push_back(Spherical2Carthesian(radius, i * diff_slices, betaDown));
            points.push_back(Spherical2Carthesian(radius, (i + 1) * diff_slices, betaDown));
            points.push_back(Spherical2Carthesian(radius, i * diff_slices, betaUp));

            points.push_back(Spherical2Carthesian(radius, i * diff_slices, betaUp));
            points.push_back(Spherical2Carthesian(radius, (i + 1) * diff_slices, betaDown));
            points.push_back(Spherical2Carthesian(radius, (i + 1) * diff_slices, betaUp));

        }
        else
        {
            betaUp = 0.0f;
            betaDown = 0.0f;
        }

        for (int j = 0; j < stacks - 1; j++)
        {

            Point pSuperior1 = Spherical2Carthesian(radius, i * diff_slices, betaUp);
            Point pSuperior2 = Spherical2Carthesian(radius, (i + 1) * diff_slices, betaUp);
            betaUp += diff_stacks;
            Point pSuperior3 = Spherical2Carthesian(radius, i * diff_slices, betaUp);
            Point pSuperior4 = Spherical2Carthesian(radius, (i + 1) * diff_slices, betaUp);


            Point pInfeior1 = Spherical2Carthesian(radius, i * diff_slices, betaDown);
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



            points.push_back(pInferior2);
            points.push_back(pInfeior1);
            points.push_back(pInferior3);

            points.push_back(pInferior3);
            points.push_back(pInfeior1);
            points.push_back(pInferior4);


            if (j == (stacks - 2))
            {
                Point topo = {0.0f, radius, 0.0f};
                Point baixo = {0.0f, -radius, 0.0f};

                points.push_back(pSuperior3);
                points.push_back(pSuperior4);
                points.push_back(topo);


                points.push_back(baixo);
                points.push_back(pInferior3);
                points.push_back(pInferior4);

            }

        }
    }

    return points;
}