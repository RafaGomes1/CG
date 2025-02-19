//
// Created by mikep on 26-03-2024.
//

#ifndef CG_FASE_2_TRANSFORMATION_H
#define CG_FASE_2_TRANSFORMATION_H

#include "vector"
#include <string>

class Transformation {
private:
    const char* type; // Type of transformation
    float x, y, z; // Coords Values
    float angle; // For Scales
public:
    Transformation(const char* type, float x, float y, float z, float angle);
    ~Transformation();
    void transform();
};


#endif //CG_FASE_2_TRANSFORMATION_H
