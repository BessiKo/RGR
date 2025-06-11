#include "matrix.h"
#include <cmath>

// Символ, используемый для заполнения пустых ячеек матрицы.
const char FILL_CHARACTER = ' ';

// Вычисляет размер квадратной матрицы на основе количества входных символов.
int CalculateMatrixSize(int dataSize) {
    if (dataSize <= 0) {
        throw runtime_error("Неверный размер данных для матрицы");
    }
    return static_cast<int>(ceil(sqrt(dataSize)));
}

// Шифрование текста путём заполнения матрицы по спирали от центра
vector<string> MatrixEncrypt(const vector<string>& symbols) {
    // Проверяем входные данные
    if (symbols.empty()) {
        throw runtime_error("Нечего шифровать");
    }

    // Определяем размер квадратной матрицы
    const int matrixSize = CalculateMatrixSize(static_cast<int>(symbols.size()));
    const int centerIndex = matrixSize / 2;

    // Инициализируем матрицу, заполняя её пустыми символами
    vector<vector<string>> matrix(matrixSize,
        vector<string>(matrixSize, string(1, FILL_CHARACTER)));

    int row = centerIndex, col = centerIndex;
    int spiralStep = 0, symbolIndex = 0;

    // Заполняем матрицу по спирали
    for (int spiralRound = 0, direction = 0; symbolIndex < static_cast<int>(symbols.size()); spiralRound++, direction++) {
        if (spiralRound % 2 == 0) {
            spiralStep++; // Увеличиваем шаг через один круг
        }

        switch (direction % 4) {
            case 0: // Направление: влево
                for (int m = 0; m < spiralStep && symbolIndex < static_cast<int>(symbols.size()); m++) {
                    if (col < 0) break;
                    matrix[row][col--] = symbols[symbolIndex++];
                }
                break;
            case 1: // Направление: вверх
                for (int m = 0; m < spiralStep && symbolIndex < static_cast<int>(symbols.size()); m++) {
                    if (row < 0) break;
                    matrix[row--][col] = symbols[symbolIndex++];
                }
                break;
            case 2: // Направление: вправо
                for (int m = 0; m < spiralStep && symbolIndex < static_cast<int>(symbols.size()); m++) {
                    if (col >= matrixSize) break;
                    matrix[row][col++] = symbols[symbolIndex++];
                }
                break;
            case 3: // Направление: вниз
                for (int m = 0; m < spiralStep && symbolIndex < static_cast<int>(symbols.size()); m++) {
                    if (row >= matrixSize) break;
                    matrix[row++][col] = symbols[symbolIndex++];
                }
                break;
        }
    }

    // Формируем выходной вектор — чтение матрицы слева направо, сверху вниз
    vector<string> encrypted;
    encrypted.reserve(matrixSize * matrixSize); // Оптимизация: предварительное выделение памяти
    for (int r = 0; r < matrixSize; ++r) {
        for (int c = 0; c < matrixSize; ++c) {
            encrypted.push_back(matrix[r][c]);
        }
    }

    return encrypted;
}

// Дешифрование текста, зашифрованного методом матричного шифрования.
vector<string> MatrixDecrypt(const vector<string>& symbols) {
    // Проверяем входные данные
    if (symbols.empty()) {
        throw runtime_error("Пустые входные данные для расшифровки");
    }

    // Определяем размер матрицы
    const int matrixSize = CalculateMatrixSize(static_cast<int>(symbols.size()));
    const int centerIndex = matrixSize / 2;

    // Заполняем матрицу данными из входного вектора
    vector<vector<string>> matrix(matrixSize, vector<string>(matrixSize));
    int flatIndex = 0;
    for (int r = 0; r < matrixSize; ++r) {
        for (int c = 0; c < matrixSize; ++c) {
            if (flatIndex < static_cast<int>(symbols.size())) {
                matrix[r][c] = symbols[flatIndex++];
            } else {
                matrix[r][c] = string(1, FILL_CHARACTER);
            }
        }
    }

    // Восстанавливаем исходный порядок следования символов, двигаясь по спирали
    vector<string> decrypted;
    decrypted.reserve(symbols.size());

    int row = centerIndex, col = centerIndex, spiralStep = 0, symbolCount = 0;

    for (int spiralRound = 0, direction = 0; symbolCount < static_cast<int>(symbols.size()); spiralRound++, direction++) {
        if (spiralRound % 2 == 0) {
            spiralStep++;
        }

        switch (direction % 4) {
            case 0: // Влево
                for (int m = 0; m < spiralStep && symbolCount < static_cast<int>(symbols.size()); m++) {
                    if (col < 0) break;
                    decrypted.push_back(matrix[row][col--]);
                    symbolCount++;
                }
                break;
            case 1: // Вверх
                for (int m = 0; m < spiralStep && symbolCount < static_cast<int>(symbols.size()); m++) {
                    if (row < 0) break;
                    decrypted.push_back(matrix[row--][col]);
                    symbolCount++;
                }
                break;
            case 2: // Вправо
                for (int m = 0; m < spiralStep && symbolCount < static_cast<int>(symbols.size()); m++) {
                    if (col >= matrixSize) break;
                    decrypted.push_back(matrix[row][col++]);
                    symbolCount++;
                }
                break;
            case 3: // Вниз
                for (int m = 0; m < spiralStep && symbolCount < static_cast<int>(symbols.size()); m++) {
                    if (row >= matrixSize) break;
                    decrypted.push_back(matrix[row++][col]);
                    symbolCount++;
                }
                break;
        }
    }

    return decrypted;
}