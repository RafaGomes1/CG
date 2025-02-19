//
// Created by mikep on 26-03-2024.
//

#ifndef CG_FASE_2_TRANSFORMATION_H
#define CG_FASE_2_TRANSFORMATION_H

#include "vector"
#include <string>
#include "../../Utils/point.h"

class Transformation {
public:
    virtual void transform(float, bool);
};

class TransformationStatic : public Transformation {
private:
    const char* type; // Type of transformation
    float x, y, z; // Coords Values
    float angle; // For Scales

public:
    void transform(float, bool);
    TransformationStatic(const char* type, float x, float y, float z, float angle);
    ~TransformationStatic();
};


class TranslationDynamic : public Transformation {
private:
    float duration;
    bool align = false;         // Default value
    std::vector<Point> points;
    GLuint vbo[1];

public:
    void transform(float, bool);
    TranslationDynamic(float angle, std::vector<Point> p, bool isAlign);
    ~TranslationDynamic();
};

class RotationDynamic : public Transformation {
private:
    float duration;
    float x,y,z;
public:
    void transform(float, bool);
    RotationDynamic(float time, float new_x, float new_y, float new_z) {
        duration = time * 1000;
        x = new_x;
        y = new_y;
        z = new_z;
    }
    ~RotationDynamic();
};




#endif //CG_FASE_2_TRANSFORMATION_H
