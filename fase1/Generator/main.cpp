#include <iostream>
#include <string>
#include "headers/plane.h"
#include "headers/primitive.h"
#include "headers/box.h"
#include "headers/sphere.h"
#include "headers/cone.h"
#include <fstream>

// Para não repetir std:: em várias funções
using namespace std;

void file_creation(vector<Point> points, string filename){
    ofstream outputFile("../3DFiles/" + filename);

    for (Point point : points) {
        outputFile <<  point.getX() << ", " << point.getY() << ", " << point.getZ() << "\n";
    }
}

void generator(int argc, char** data){
    auto primType = string(data[1]);
    // Declarados aqui para evitar repetir lógica dentro dos ifs
    vector<Point> points;
    string filename;

    if (primType == "plane" && argc == 5)
    {
        float length = stof(data[2]);
        int div = stoi(data[3]);
        Plane plane(length, div);

        points = plane.genPoints();
        filename = data[4];
    }
    else if (primType == "cone" && argc == 7)
    {
        float radius = stof(data[2]);
        float height = stof(data[3]);
        int slices = stoi(data[4]);
        int stacks = stoi(data[5]);

        Cone cone(radius, height, slices, stacks);
        points = cone.genPoints();
        filename = data[6];
    }
    else if (primType == "sphere" && argc == 6)
    {
        float radius = stof(data[2]);
        int slices = stoi(data[3]);
        int stacks = stoi(data[4]);
        filename = data[5];

        Sphere sphere(radius, slices, stacks);
        points = sphere.genPoints();


    }
    else if (primType == "box" && argc == 5)
    {
        float length = stof(data[2]);
        int div = stoi(data[3]);
        Box box(length, div);

        points = box.genPoints();
        filename = data[4];
    }
    else
    {
        std::cout << "Graphical Primitive's Type not found!";
    }

    // Caso o input esteja errado então não executa
    if(!points.empty()){
        file_creation(points, filename);
    }
}

//@TODO Meter num método à parte para a main não ficar com lógica dentro dela?
// Da forma que estamos a fazer sq nao precisamos de hierarquia, rever.
int main(int argc, char** argv)
{
    generator(argc, argv);
    return 0;
}