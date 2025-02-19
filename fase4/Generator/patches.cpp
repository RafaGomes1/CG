//
// Created by mikep on 17-04-2024.
//

#include "headers/patches.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include "math.h"

Patches::Patches(int t, std::string pFile)
{
    tesselation = t;
    patchFile = pFile;
    numberPatches = 0;
    numberControlPoints = 0;
    patchesIndex = std::vector<std::vector<int>>();
    controlPoints = std::vector<Point>();
}

// Lê o ficheiro de patches
void Patches::readPatchesFile()
{
    std::ifstream file("../Patches/" + patchFile);

    // Caso ocorra algum erro ao ler o ficheiro .patch
    if(!file.is_open())
    {
        std::cout << "Error opening patches file: \"" << patchFile << "\"" << std::endl;
        exit(1);
    }

    std::string line;

    // Ler o numero de patches
    std::getline(file, line);
    numberPatches = std::stoi(line);

    // Ler as linhas com os index dos patches
    for(int i = 0; i < numberPatches; ++i)
    {
        std::getline(file, line);
        std::vector<int> patchLine;

        // Separar a string nos valores separados por ","
        char* strNumbers = std::strtok(&line[0], ",");
        while(strNumbers != nullptr)
        {
            int number = std::stoi(strNumbers);
            patchLine.push_back(number);
            strNumbers = std::strtok(nullptr, ",");
        }
        patchesIndex.push_back(patchLine);
    }

    // Lê o número de pontos de controlo
    std::getline(file, line);
    numberControlPoints = std::stoi(line);

    // Ler os pontos de Controlo
    for(int i = 0; i < numberControlPoints; ++i)
    {
        std::getline(file, line);

        float x = std::stof(std::strtok(&line[0], ","));
        float y = std::stof(std::strtok(nullptr, ","));
        float z = std::stof(std::strtok(nullptr, ","));
        Point point = Point(x, y, z);
        controlPoints.push_back(point);
    }

    file.close();
}

// Calcular um ponto de um patch utilizando a interpolação de Bezier
Point Patches::calcPoint(float u, float v, std::vector<int> patchIndex)
{
    std::vector<std::vector<float>> bezierMatrix = { // Dimensões: 4x4
            {-1, 3, -3, 1},
            {3, -6, 3, 0},
            {-3, 3, 0, 0},
            {1, 0, 0, 0}
    };

    // Matrizes u e v para a interpolação
    std::vector<std::vector<float>> uMatrix = {{u*u*u, u*u, u, 1}}; // Dimensões: 1x4
    std::vector<std::vector<float>> vMatrix = {{v*v*v}, {v*v}, {v}, {1}}; // Dimensões 4x1

    // Calculo de matrizes intermedias: uMatrix x bezierMatrix ; (bezierMatrix)Transposta x vMatrix
    std::vector<std::vector<float>> uBezierMatrix = MatrixOperations::MultiplyMatrices(uMatrix, bezierMatrix);
    std::vector<std::vector<float>> bezierVMatrix = MatrixOperations::MultiplyMatrices(bezierMatrix, vMatrix);

    // Matrizes com as coordenadas x, y e z dos pontos de controlo do patch
    std::vector<std::vector<float>> xMatrix = { // Dimensões: 4x4
            {controlPoints[patchIndex[0]].getX(), controlPoints[patchIndex[1]].getX(), controlPoints[patchIndex[2]].getX(), controlPoints[patchIndex[3]].getX()},
            {controlPoints[patchIndex[4]].getX(), controlPoints[patchIndex[5]].getX(), controlPoints[patchIndex[6]].getX(), controlPoints[patchIndex[7]].getX()},
            {controlPoints[patchIndex[8]].getX(), controlPoints[patchIndex[9]].getX(), controlPoints[patchIndex[10]].getX(), controlPoints[patchIndex[11]].getX()},
            {controlPoints[patchIndex[12]].getX(), controlPoints[patchIndex[13]].getX(), controlPoints[patchIndex[14]].getX(), controlPoints[patchIndex[15]].getX()}
    };

    std::vector<std::vector<float>> yMatrix = { // Dimensões: 4x4
            {controlPoints[patchIndex[0]].getY(), controlPoints[patchIndex[1]].getY(), controlPoints[patchIndex[2]].getY(), controlPoints[patchIndex[3]].getY()},
            {controlPoints[patchIndex[4]].getY(), controlPoints[patchIndex[5]].getY(), controlPoints[patchIndex[6]].getY(), controlPoints[patchIndex[7]].getY()},
            {controlPoints[patchIndex[8]].getY(), controlPoints[patchIndex[9]].getY(), controlPoints[patchIndex[10]].getY(), controlPoints[patchIndex[11]].getY()},
            {controlPoints[patchIndex[12]].getY(), controlPoints[patchIndex[13]].getY(), controlPoints[patchIndex[14]].getY(), controlPoints[patchIndex[15]].getY()}
    };

    std::vector<std::vector<float>> zMatrix = { // Dimensões: 4x4
            {controlPoints[patchIndex[0]].getZ(), controlPoints[patchIndex[1]].getZ(), controlPoints[patchIndex[2]].getZ(), controlPoints[patchIndex[3]].getZ()},
            {controlPoints[patchIndex[4]].getZ(), controlPoints[patchIndex[5]].getZ(), controlPoints[patchIndex[6]].getZ(), controlPoints[patchIndex[7]].getZ()},
            {controlPoints[patchIndex[8]].getZ(), controlPoints[patchIndex[9]].getZ(), controlPoints[patchIndex[10]].getZ(), controlPoints[patchIndex[11]].getZ()},
            {controlPoints[patchIndex[12]].getZ(), controlPoints[patchIndex[13]].getZ(), controlPoints[patchIndex[14]].getZ(), controlPoints[patchIndex[15]].getZ()}
    };

    // Calculo das coordenadas x,y e z do ponto resultante da interpolação
    std::vector<std::vector<float>> uBezierPointXMatrix = MatrixOperations::MultiplyMatrices(uBezierMatrix,xMatrix);
    float x = MatrixOperations::MultiplyMatrices(uBezierPointXMatrix, bezierVMatrix)[0][0];

    std::vector<std::vector<float>> uBezierPointYMatrix = MatrixOperations::MultiplyMatrices(uBezierMatrix,yMatrix);
    float y = MatrixOperations::MultiplyMatrices(uBezierPointYMatrix, bezierVMatrix)[0][0];

    std::vector<std::vector<float>> uBezierPointZMatrix = MatrixOperations::MultiplyMatrices(uBezierMatrix,zMatrix);
    float z = MatrixOperations::MultiplyMatrices(uBezierPointZMatrix, bezierVMatrix)[0][0];

    return {x,y,z};
}


std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>> Patches::genPoints() {
    std::vector<Point> points;
    std::vector<Point> normals;
    std::vector<Point> texCoords;
    readPatchesFile();

    float step = 1.0f / tesselation;
    float u = 0.0f;
    float v = 0.0f;

    for (std::vector<int> patch : patchesIndex) {

        for (int i = 0; i < tesselation; ++i) {

            for (int j = 0; j < tesselation; ++j) {

                // Calculate points for a patch
                Point a = calcPoint(u, v, patch);
                Point b = calcPoint(u, v + step, patch);
                Point c = calcPoint(u + step, v, patch);
                Point d = calcPoint(u + step, v + step, patch);

                // Store points of the triangles formed
                points.push_back(c);
                points.push_back(a);
                points.push_back(b);

                points.push_back(b);
                points.push_back(d);
                points.push_back(c);

                // Calculate normals for the triangles
                Point normal1 = calcNormal(c, a, b);
                Point normal2 = calcNormal(b, d, c);

                normals.push_back(normal1);
                normals.push_back(normal1);
                normals.push_back(normal1);

                normals.push_back(normal2);
                normals.push_back(normal2);
                normals.push_back(normal2);

                // Calculate texture coordinates
                texCoords.push_back(Point(u + step, v, 0));
                texCoords.push_back(Point(u, v, 0));
                texCoords.push_back(Point(u, v + step, 0));

                texCoords.push_back(Point(u, v + step, 0));
                texCoords.push_back(Point(u + step, v + step, 0));
                texCoords.push_back(Point(u + step, v, 0));

                v += step;
            }
            v = 0;
            u += step;
        }
        u = 0.0f;
        v = 0.0f;
    }

    return std::make_tuple(points, normals, texCoords);
}

Point Patches::calcNormal(Point& p1, Point& p2, Point& p3) {
    Point u = p2 - p1;
    Point v = p3 - p1;
    Point normal = cross(u, v);
    return normalize(normal);
}

Point Patches::cross(Point& a, Point& b) {
    return Point(a.getY() * b.getZ() - a.getZ() * b.getY(),
                 a.getZ() * b.getX() - a.getX() * b.getZ(),
                 a.getX() * b.getY() - a.getY() * b.getX());
}

Point Patches::normalize(Point& a) {
    float length = std::sqrt(a.getX() * a.getX() + a.getY() * a.getY() + a.getZ() * a.getZ());
    if (length > 0.0f) {
        return Point(a.getX() / length, a.getY() / length, a.getZ() / length);
    }
    return a;
}