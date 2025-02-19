//
// Created by mikep on 18-04-2024.
//

#include "MatrixOperations.h"
#include "math.h"
#include <iostream>


std::vector<std::vector<float>>
MatrixOperations::MultiplyMatrices(std::vector<std::vector<float>> matrixA, std::vector<std::vector<float>> &matrixB) {

    int linesA = matrixA.size(); // Linhas da Matriz A
    int collumsA = matrixA[0].size(); // Colunas da Matriz A

    int linesB = matrixB.size(); // Linhas da Matriz B
    int collumsB = matrixB[0].size(); // Colunas da Matriz B

    if (collumsA == linesB) // Verificar se as matrizes são compativeis para a multiplicação
    {
        std::vector<std::vector<float>> result(linesA, std::vector<float>(collumsB,0)); // Criar um vertor de vetores para a Matriz resultante da multiplicação

        for (int i = 0; i < linesA; ++i) {
            for (int j = 0; j < collumsB; ++j) {
                for (int k = 0; k < collumsA; ++k) {
                    result[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }
        return result;
    }
    else // Caso as Matrizes não sejam compativeis
    {
        std::cout << "Error: The dimensions of the matrices are not compatible for multiplication. CollumsA = " << collumsA << "; linesB = " << linesB << std::endl;
        return std::vector<std::vector<float>>();
    }
}

std::vector<float> MatrixOperations::normalizeMatrix(std::vector<float> matrix) {
    float n = sqrt(matrix[0] * matrix[0] + matrix[1] * matrix[1] + matrix[2] * matrix[2]);
    return {(matrix[0] / n) , (matrix[1] / n) , (matrix[2] /n)};
}

std::vector<float> MatrixOperations::crossMatrix(std::vector<float> matrix1, std::vector<float> matrix2)
{
    return {(matrix1[1] * matrix2[2] - matrix1[2] * matrix2[1]),
            (matrix1[2] * matrix2[0] - matrix1[0] * matrix2[2]),
            (matrix1[0] * matrix2[1] - matrix1[1] * matrix2[0])};
}