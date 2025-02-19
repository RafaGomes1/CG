#include "headers/Light.h"
#include "../Utils/point.h"
#include <iostream>
#include <vector>

Light::Light() : type(""), pos(4, 0.0f), dir(4, 0.0f), cutoff(0.0f) {
    // pos[3] = 1.0f; // É um ponto
}

// Setters
void Light::setPosition(Point& pos) {

    this->pos = {pos.getX(), pos.getY(), pos.getZ(), 1.0f};
}

void Light::setDirection(Point& direction) {
    this->dir = {direction.getX(), direction.getY(), direction.getZ(), 0.0f};
}

void Light::setCutoff(float cutoff) {
    this->cutoff = cutoff;
}

void Light::setType(const std::string& type) {
    this->type = type;
}

// Getters
std::vector<float> Light::getPosition() const {
    return pos;
}

std::vector<float> Light::getDirection() const {
    return dir;
}

size_t Light::getCutoff() const {
    return cutoff;
}

std::string Light::getType() const {
    return type;
}

std::ostream& operator<<(std::ostream& os, const Light& light) {
    os << "Type: " << light.getType() << "\n";
    if (light.getType() == std::string("point") || light.getType() == std::string("spotlight")) {
        os << "Position: (";
        auto pos = light.getPosition();
        for (size_t i = 0; i < pos.size(); ++i) {
            os << pos[i];
            if (i < pos.size() - 1) os << ", ";
        }
        os << ")\n";
        os << "Direction: (";
        auto dir = light.getDirection();
        for (size_t i = 0; i < dir.size(); ++i) {
            os << dir[i];
            if (i < dir.size() - 1) os << ", ";
        }
        os << ")\n";
        os << "Cutoff: " << light.getCutoff() << "\n";
    }
    if (light.getType() == std::string("directional") || light.getType() == std::string("spotlight")) {
        os << "Direction: (";
        auto dir = light.getDirection();
        for (size_t i = 0; i < dir.size(); ++i) {
            os << dir[i];
            if (i < dir.size() - 1) os << ", ";
        }
        os << ")\n";
    }
    if (light.getType() == std::string("spotlight")) {
        os << "Cutoff: " << light.getCutoff() << "\n";
    }
    return os;
}