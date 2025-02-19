#include <iostream>
#include <string>
#include "headers/plane.h"
#include "headers/primitive.h"
#include "headers/box.h"
#include "headers/sphere.h"
#include "headers/cone.h"
#include "headers/torus.h"
#include "headers/patches.h"
#include <fstream>
#include <tuple>

// Para não repetir std:: em várias funções
using namespace std;

void file_creation(Primitive *primitive, string filename) {
    ofstream outputFile("../3DFiles/" + filename);
    auto [points, normals, texCoords] = (*primitive).genPoints();

    for (size_t i = 0; i < points.size(); i++) {
        outputFile << points[i].getX() << ", " << points[i].getY() << ", " << points[i].getZ() << ", "
                   << normals[i].getX() << ", " << normals[i].getY() << ", " << normals[i].getZ() << ", "
                   << texCoords[i].getX() << ", " << texCoords[i].getY() << ", " << texCoords[i].getZ() << "\n";
    }
}

void generator(int argc, char** data){
    auto primType = string(data[1]);
    // Declarados aqui para evitar repetir lógica dentro dos ifs
    vector<Point> points;
    vector<Point> normals;
    vector<Point> texCoords;
    string filename;

    if (primType == "plane" && argc == 5) // ./generator plane <length> <divisions> <3d file>
    {
        float length = stof(data[2]);
        int div = stoi(data[3]);
        Plane plane(length, div);
        filename = data[4];
        file_creation(&plane, filename);
    }
    else if (primType == "cone" && argc == 7) // ./generator cone <radius> <height> <slices> <stacks> <3d file>
    {
        float radius = stof(data[2]);
        float height = stof(data[3]);
        int slices = stoi(data[4]);
        int stacks = stoi(data[5]);

        Cone cone(radius, height, slices, stacks);
        filename = data[6];
        file_creation(&cone, filename);
    }
    else if (primType == "sphere" && argc == 6) // ./generator sphere <radius> <slices> <stacks> <3d file>
    {
        float radius = stof(data[2]);
        int slices = stoi(data[3]);
        int stacks = stoi(data[4]);
        filename = data[5];

        Sphere sphere(radius, slices, stacks);
        file_creation(&sphere, filename);
    }
    else if (primType == "box" && argc == 5) // ./generator box <length> <divisions> <3d file>
    {
        float length = stof(data[2]);
        int div = stoi(data[3]);
        Box box(length, div);
        filename = data[4];
        file_creation(&box, filename);
    }
    else if (primType == "torus" && argc == 7){ // ./generator torus <radius> <height> <sides> <rings> <3d file>
        float radius = stof(data[2]);
        float height = stof(data[3]);
        int sides = stoi(data[4]);
        int rings = stoi(data[5]);

        Torus torus(radius, height, sides, rings);
        file_creation(&torus, data[6]);
    }
    else if (primType == "patch" && argc == 5) // ./generator bezier <tesselation> <patch File> <3d file>
    {
        string patchFile = data[2];
        int tesselation = stoi(data[3]);
        filename = data[4];

        Patches patches(tesselation, patchFile);
        file_creation(&patches, filename);

    }
    else
    {
        std::cout << "Graphical Primitive's Type not found!" << std::endl;
    }

    // Caso o input esteja errado então não executa
    //if (!points.empty() && !normals.empty() && !texCoords.empty()) {
    //    file_creation(points, normals, texCoords, filename);
    //}
    //else
    //{
    //    std::cout << "Error: No Points were generated." << std::endl;
    //}
}

//@TODO Meter num método à parte para a main não ficar com lógica dentro dela?
// Da forma que estamos a fazer sq nao precisamos de hierarquia, rever.
int main(int argc, char** argv)
{
    generator(argc, argv);
    return 0;
}