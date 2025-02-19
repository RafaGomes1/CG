//
// Created by mikep on 23-02-2024.
//

#include "headers/primitive.h"

std::vector<Point> &Primitive::getPoints() {
    return points;
}

void Primitive::setPoints(std::vector<Point> &vector) {
    points = vector;
}