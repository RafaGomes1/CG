//
// Created by mikep on 26-03-2024.
//
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


void Group::draw(){
    glPushMatrix();

    for(const auto& transformation : transformations){
        transformation->transform();
    }

    for(auto model: models){
        model.draw();
    }

    for (auto & subGroup : subGroups) {
        subGroup.draw();
    }
    glPopMatrix();
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
            transfs.push_back(std::make_unique<Transformation>(type,x,y,z,angle));
        }
        else if (strcmp(child->Name(), "translate") == 0 && !hasTranslate)
        {
            hasTranslate = true;
            const char* type = child->Name();
            child->QueryFloatAttribute("x", &x);
            child->QueryFloatAttribute("y", &y);
            child->QueryFloatAttribute("z", &z);
            transfs.push_back(std::make_unique<Transformation>(type,x,y,z,angle));
        } else if (strcmp(child->Name(), "rotate") == 0 && !hasRotate)
        {
            hasRotate = true;
            const char* type = child->Name();
            child->QueryFloatAttribute("x", &x);
            child->QueryFloatAttribute("y", &y);
            child->QueryFloatAttribute("z", &z);
            child->QueryFloatAttribute("angle", &angle);
            transfs.push_back(std::make_unique<Transformation>(type,x,y,z,angle));
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



