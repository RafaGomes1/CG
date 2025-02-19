//
// Created by mikep on 17-04-2024.
//

#ifndef CG_FASE_3_PATCHES_H
#define CG_FASE_3_PATCHES_H

#include "primitive.h"
#include <string>
#include "../../Utils/MatrixOperations.h"

class Patches : public Primitive {
private:
    int tesselation;
    std::string patchFile;
    int numberPatches;
    int numberControlPoints;
    std::vector<std::vector<int>> patchesIndex;
    std::vector<Point> controlPoints;

public:
    Patches(int t, std::string pFile);
    void readPatchesFile();
    std::vector<Point> genPoints();
    Point calcPoint(float u, float v, std::vector<int> patchIndex);
};


#endif //CG_FASE_3_PATCHES_H
