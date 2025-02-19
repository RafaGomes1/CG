//
// Created by mikep on 26-03-2024.
//
#include <GL/glew.h>
#include <GL/glut.h>
#include "headers/Group.h"
#include <iostream>
#include "headers/Transformation.h"
#include "headers/Model.h"
#include <memory>
#include "../Utils/point.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace tinyxml2;


void Group::draw(float time_elapsed, bool drawCurves){
    glPushMatrix();

    for(const auto& transformation : transformations){
        transformation->transform(time_elapsed, drawCurves);
    }

    for(auto model: models){
        model.draw();
    }

    for (auto & subGroup : subGroups) {
        subGroup.draw(time_elapsed, drawCurves);
    }
    glPopMatrix();
}

void Group::clearModelVBOs() {
    for (auto& model : models) {
        model.clear();
    }

    for (auto& subGroup : subGroups) {
        subGroup.clearModelVBOs();
    }
}

std::unique_ptr<Transformation> parse_rotate(XMLElement *child){
    float x=0.0f, y=0.0f, z=0.0f, angle=0.0f, time=0.0f;

    const char* type = child->Name();
    child->QueryFloatAttribute("x", &x);
    child->QueryFloatAttribute("y", &y);
    child->QueryFloatAttribute("z", &z);
    child->QueryFloatAttribute("angle", &angle);

    if (child->QueryFloatAttribute("time", &time) == tinyxml2::XML_SUCCESS){
        return std::make_unique<RotationDynamic>(time,x,y,z);
    }
    return std::make_unique<TransformationStatic>(type, x,y,z,angle);
}

std::unique_ptr<Transformation> parse_translate(XMLElement *child){
    const char* type = child->Name();

    float x = 0.0f, y = 0.0f, z = 0.0f, angle= 0.0f, time=0.0f;
    bool timed = false, align=false;
    std::vector<Point> result;

    if (child->QueryFloatAttribute("time", &time) == tinyxml2::XML_SUCCESS){
        timed = true;
        child->QueryBoolAttribute("align", &align);

        for (XMLElement *element = child->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()){
            element->QueryFloatAttribute("x", &x);
            element->QueryFloatAttribute("y", &y);
            element->QueryFloatAttribute("z", &z);
            result.emplace_back(x,y,z);
        }
        return std::make_unique<TranslationDynamic>(time, result, align);
    }
    else{
        child->QueryFloatAttribute("x", &x);
        child->QueryFloatAttribute("y", &y);
        child->QueryFloatAttribute("z", &z);
        return std::make_unique<TransformationStatic>(type,x,y,z,angle);
    }
}


std::vector<std::unique_ptr<Transformation>> parseTransformation(XMLElement *element) {
    std::vector<std::unique_ptr<Transformation>> transfs;
    bool hasScale = false, hasTranslate = false, hasRotate = false;

    for (XMLElement *child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
        float x = 0.0f, y = 0.0f, z = 0.0f, angle = 0.0f;

        if (strcmp(child->Name(), "scale") == 0 && !hasScale)
        {
            hasScale = true;
            const char* type = child->Name();
            child->QueryFloatAttribute("x", &x);
            child->QueryFloatAttribute("y", &y);
            child->QueryFloatAttribute("z", &z);
            transfs.push_back(std::make_unique<TransformationStatic>(type,x,y,z,angle));
        }
        else if (strcmp(child->Name(), "translate") == 0 && !hasTranslate)
        {
            hasTranslate = true;
            transfs.push_back(parse_translate(child));
        } else if (strcmp(child->Name(), "rotate") == 0 && !hasRotate)
        {
            hasRotate = true;
            transfs.push_back(parse_rotate(child));
        }
        else
        {
            std::cout << "Invalid Transformation Group! Ignoring...";
        }
    }

    return transfs;
}

std::vector<Point> read_3D_File(std::string filename){
    std::vector<Point> points;
    std::ifstream inputFile(filename);

    std::string line;
    while (getline(inputFile, line)){
        std::istringstream iss(line);
        float x, y, z;
        char delimiter;

        iss >> x >> delimiter >> y >> delimiter >> z;
        points.emplace_back(x, y, z);
    }
    return points;
}

std::vector<Model> parseModels(XMLElement *element) {
    std::vector<Model> models;

    for (XMLElement *modelElement = element->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
        const char* file = modelElement->Attribute("file");
        float r = 1.0, g = 1.0, b = 1.0;
        if (file) {
            std::string ficheiro = "../3DFiles/" + (std::string)file;
            modelElement->QueryFloatAttribute("r", &r);
            modelElement->QueryFloatAttribute("g", &g);
            modelElement->QueryFloatAttribute("b", &b);
            models.emplace_back(read_3D_File(ficheiro), r,g,b);
        }
    }

    return models;
}

Group::Group(XMLElement *e){
    for (XMLElement *child = e->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        std::string elementName = child->Name();
        if (elementName == "transform") {
            transformations = parseTransformation(child);
        } else if (elementName == "models") {
            models = parseModels(child);
        } else if (elementName == "group") {
            Group subGroup(child);
            subGroups.push_back(std::move(subGroup));
        }
    }
}



