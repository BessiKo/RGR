#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

extern "C" {
    int CalculateMatrixSize(int dataSize);
    vector<string> MatrixEncrypt(const vector<string>& symbols);
    vector<string> MatrixDecrypt(const vector<string>& symbols);
}

#endif