//
// Created by mikep on 26-03-2024.
//

#ifndef CG_FASE_2_GROUP_H
#define CG_FASE_2_GROUP_H

#include <GL/glew.h>
#include "Transformation.h"
#include "Model.h"
#include "vector"
#include "tinyxml2.h"
#include "memory"
using namespace tinyxml2;

class Group {
private:
    std::vector<std::unique_ptr<Transformation>>  transformations;
    std::vector<Model> models;
    std::vector<Group> subGroups;

public:
    Group(XMLElement *e);
    void draw(float t, bool);
    void clearModelVBOs();
    Group parseGroup(XMLElement *e);
};


#endif //CG_FASE_2_GROUP_H
