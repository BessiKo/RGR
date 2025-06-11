#include "permutation.h"

// Символ, используемый для заполнения блока, если данных не хватает
const char FILL_CHARACTER = ' ';

// Получает обратную перестановку для заданной перестановки.
vector<int> GetInversePermutation(const vector<int>& permutation) {
    if (permutation.empty()) {
        throw runtime_error("Пустой вектор перестановки");
    }

    const int size = permutation.size();
    vector<int> inverse(size);
    vector<bool> used(size, false); // Проверка на повторяющиеся значения

    for (int i = 0; i < size; ++i) {
        int value = permutation[i];

        // Проверяем, что значение находится в допустимом диапазоне
        if (value < 1 || value > size) {
            throw runtime_error("Недопустимое значение перестановки");
        }

        // Проверяем, что значение еще не использовалось
        if (used[value - 1]) {
            throw runtime_error("Повторяющееся значение в перестановке");
        }

        used[value - 1] = true;
        inverse[value - 1] = i + 1; // Сохраняем позицию оригинального индекса
    }

    return inverse;
}

// Шифрует данные, используя фиксированную перестановку
vector<string> PermutationEncrypt(const vector<string>& symbols) {
    if (symbols.empty()) {
        throw runtime_error("Нечего шифровать");
    }

    vector<int> permutation = { 2, 1, 4, 3 };
    if (permutation.empty()) {
        throw runtime_error("Пустой вектор перестановки");
    }

    const int blockSize = permutation.size();
    const int symbolCount = symbols.size();

    vector<string> result;
    result.reserve(symbolCount); // предварительное выделение памяти

    // Обрабатываем данные по блокам
    for (int i = 0; i < symbolCount; i += blockSize) {
        // Создаем блок, заполненный пробелами
        vector<string> block(blockSize, string(1, FILL_CHARACTER));
        int remaining = symbolCount - i;
        int copyLength = (remaining < blockSize) ? remaining : blockSize;

        // Копируем данные в блок
        for (int j = 0; j < copyLength; ++j) {
            block[j] = symbols[i + j];
        }

        // Применяем перестановку к текущему блоку
        vector<string> permutedBlock(blockSize);
        for (int j = 0; j < blockSize; ++j) {
            permutedBlock[j] = block[permutation[j] - 1]; // Индексы начинаются с 1
        }

        // Добавляем обработанный блок к результату
        for (int j = 0; j < blockSize; ++j) {
            result.push_back(permutedBlock[j]);
        }
    }

    return result;
}

// Дешифрует данные, зашифрованные методом фиксированной перестановки
vector<string> PermutationDecrypt(const vector<string>& symbols) {
    vector<int> permutation = { 2, 1, 4, 3 };

    if (permutation.empty()) {
        throw runtime_error("Пустой вектор перестановки");
    }

    // Вычисляем обратную перестановку
    vector<int> inversePerm = GetInversePermutation(permutation);

    const int blockSize = permutation.size();
    const int symbolCount = symbols.size();

    vector<string> result;
    result.reserve(symbolCount);
    
    // Разбираем данные по блокам
    for (int i = 0; i < symbolCount; i += blockSize) {
        // Создаем блок, заполненный символом-заполнителем
        vector<string> block(blockSize, string(1, FILL_CHARACTER));
        int remaining = symbolCount - i;
        int copyLength = (remaining < blockSize) ? remaining : blockSize;

        // Заполняем блок данными
        for (int j = 0; j < copyLength; ++j) {
            block[j] = symbols[i + j];
        }

        // Применяем обратную перестановку
        vector<string> originalBlock(blockSize);
        for (int j = 0; j < blockSize; ++j) {
            originalBlock[j] = block[inversePerm[j] - 1]; // Перестановка 1-based
        }

        // Добавляем расшифрованный блок к результату
        for (int j = 0; j < blockSize; ++j) {
            result.push_back(originalBlock[j]);
        }
    }

    // Удаляем лишние символы заполнения с конца
    while (!result.empty() && result.back() == string(1, FILL_CHARACTER)) {
        result.pop_back();
    }

    return result;
}