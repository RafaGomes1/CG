//
// Created by mikep on 01-03-2024.
//

#include "headers/cone.h"
#include "math.h"

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

std::vector<Point> Cone::genPoints() {
    std::vector<Point> points;

    float alfa_diff = (2 * M_PI) / slices;
    float height_diff = height / stacks;
    float radius_diff = radius/ stacks;

    //base
    for(int i = 0; i < slices; i++)
    {
        float alfa = i * alfa_diff;
        float next_alfa = (i + 1) * alfa_diff;

        points.push_back(Point{0.0f,0.0f,0.0f});
        points.push_back(Polcar2Carthesian(radius,next_alfa,0.0f));
        points.push_back(Polcar2Carthesian(radius,alfa,0.0f));

    }

    // Laterais
    for (int i = 0; i < stacks; i++)
    {
        float rad = radius - radius_diff * i;
        float nextRad = radius - (radius / stacks) * (i + 1);
        float heigt_ = i * height_diff;
        float nextHeight = (i + 1) * height_diff;

        for (int j = 0; j < slices; j++) {
            float alfa = j * alfa_diff;
            float next_alfa = (j + 1) * alfa_diff;

            Point p1 = Polcar2Carthesian(rad,next_alfa,heigt_); // Ponta parte baixo direita
            Point p2 = Polcar2Carthesian(nextRad,alfa,nextHeight); // Ponta parte cima direita

            Point p3 = Polcar2Carthesian(rad,alfa,heigt_); // Ponta parte baixo esquerda
            Point p4 = Polcar2Carthesian(nextRad,next_alfa,nextHeight); // Ponta parte cima esqueda

            points.push_back(p1);
            points.push_back(p2);
            points.push_back(p3);

            points.push_back(p1);
            points.push_back(p4);
            points.push_back(p2);

        }
    }


    return points;
}