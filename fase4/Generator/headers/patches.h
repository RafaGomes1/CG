//
// Created by mikep on 17-04-2024.
//

#ifndef CG_FASE_3_PATCHES_H
#define CG_FASE_3_PATCHES_H

#include "primitive.h"
#include <string>
#include "../../Utils/MatrixOperations.h"
#include <tuple>

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
    std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> genPoints();
    Point calcPoint(float u, float v, std::vector<int> patchIndex);
    Point calcNormal(Point& p1, Point& p2, Point& p3);
    Point cross(Point& a, Point& b);
    Point normalize(Point& a);
};


#endif //CG_FASE_3_PATCHES_H
