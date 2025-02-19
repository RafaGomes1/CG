//
// Created by mikep on 23-02-2024.
//

#include "point.h"

Point::Point(float a, float b, float c) {
    x = a;
    y = b;
    z = c;
}

float Point::getX() {
    return x;
}

float Point::getY() {
    return y;
}

float Point::getZ() {
    return z;
}


