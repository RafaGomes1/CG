//
// Created by mikep on 18-04-2024.
//

#ifndef CG_FASE_3_MATRIXOPERATIONS_H
#define CG_FASE_3_MATRIXOPERATIONS_H

#include <vector>

class MatrixOperations {

public:
    static std::vector<std::vector<float>> MultiplyMatrices(std::vector<std::vector<float>> matrix1, std::vector<std::vector<float>>& matrix2);
    static std::vector<float> normalizeMatrix(std::vector<float> matrix);
    static std::vector<float> crossMatrix(std::vector<float> matrix1, std::vector<float> matrix2);
};


#endif //CG_FASE_3_MATRIXOPERATIONS_H
