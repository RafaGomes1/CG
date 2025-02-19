//
// Created by mikep on 26-03-2024.
//

#include <GL/glut.h>
#include "headers/Transformation.h"
#include "headers/tinyxml2.h"
#include <string>
#include <iostream>


Transformation::Transformation(const char* t, float x_, float y_, float z_, float angle_ = 0.0) {
    type = strdup(t);
    x = x_;
    y = y_;
    z = z_;
    angle = angle_;
}

Transformation::~Transformation() {
    free((void *) type); // Freeing the allocated memory
}

void Transformation::transform() {
    if(strcmp(type, "translate") == 0){
        glTranslatef(x,y,z);
    }
    else if (strcmp(type, "rotate") == 0){
        glRotatef(angle,x,y,z);
    }
    else if (strcmp(type, "scale") == 0){
        glScalef(x,y,z);
    }
}
