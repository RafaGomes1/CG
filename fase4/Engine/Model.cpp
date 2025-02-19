//
// Created by mikep on 26-03-2024.
//
#define GL_GLEXT_PROTOTYPES


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <utility>
#include <cstring>
#include "headers/Model.h"
#include <tuple>
#include <cstring>
#include <IL/il.h>
//#include <IL/ilu.h>

Model::Model(std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>>& data, float r1, float g1, float b1, 
          GLfloat diff[4], GLfloat amb[4], GLfloat spec[4], GLfloat emis[4], 
          GLfloat shini, const std::string& texFile)
{
    r = r1;
    g = g1;
    b = b1;
    auto& [pts, norms, texs] = data;
    

    std::vector<float> vbo_data;
    std::vector<float> normal_data;
    std::vector<float> tex_data;

    for (Point& pt : pts) {
        vbo_data.push_back(pt.getX());
        vbo_data.push_back(pt.getY());
        vbo_data.push_back(pt.getZ());
    }

    for (Point& norm : norms) {
        normal_data.push_back(norm.getX());
        normal_data.push_back(norm.getY());
        normal_data.push_back(norm.getZ());
    }

    for (Point& tex : texs) {
        tex_data.push_back(tex.getX());
        tex_data.push_back(tex.getY());
    }
    vertice_num = vbo_data.size()/3;

    std::memcpy(diffusive, diff, 4 * sizeof(GLfloat));
    std::memcpy(ambient, amb, 4 * sizeof(GLfloat));
    std::memcpy(specular, spec, 4 * sizeof(GLfloat));
    std::memcpy(emissive, emis, 4 * sizeof(GLfloat));
    shininess = shini;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glGenBuffers(3, vbo);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(float), vbo_data.data(), GL_STATIC_DRAW);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normal_data.size() * sizeof(float), normal_data.data(), GL_STATIC_DRAW);

    // Texture Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, tex_data.size() * sizeof(float), tex_data.data(), GL_STATIC_DRAW);
    
    if (!texFile.empty()) {
        has_texture = true;
        unsigned int t, tw, th;
        unsigned char* texData;

        ilGenImages(1, &t);
        ilBindImage(t);
        textureFile = "../XMLFiles/" + texFile;

        ilLoadImage((ILstring)textureFile.c_str());

        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        texData = ilGetData();

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);
        GLint texture_width = 0;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texture_width);
        if (texture_width == 0) {
            std::cerr << "Failed to generate texture" << std::endl;
            has_texture = false;
        }

    }
}

std::vector<Point> Model::getPoints() {
    return points;
}

void Model::addPoint(Point p)
{
    points.push_back(p);
}


void Model::draw() {
    glColor3f(r,g,b);
    // Set the material properties

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffusive);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // Bind and set vertex pointer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // Bind and set normal pointer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glNormalPointer(GL_FLOAT, 0, 0);
    if (has_texture) {
        // Bind and set texture coordinate pointer
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }

    // Enable client states
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    if (has_texture) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    // Draw the arrays
    glDrawArrays(GL_TRIANGLES, 0, vertice_num);

    // Disable client states after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    if (has_texture) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture
    }

}

void Model::clear(){
    glDeleteBuffers(3, vbo);
}