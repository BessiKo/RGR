#include "gamma.h"
#include <random>
#include <stdexcept>
#include <limits>
#include <cctype>


const int MIN_KEY_LENGTH = 5;   //  минимальная длина ключа
const int MAX_KEY_LENGTH = 20;  // максимальная длина ключа
const int MIN_CHAR = 33;        // Минимальное значение ASCII для печатаемых символов
const int MAX_CHAR = 127;       // Максимальное значение ASCII для печатаемых символов


// Генерация случайного гамма-кода.
// Генератор создает гамму произвольной длины от MIN_KEY_LENGTH до MAX_KEY_LENGTH
// Ключи состоят либо из буквенных символов (A-Z, a-z), либо цифр (0-9)
string generateGammaKey() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> lenDist(MIN_KEY_LENGTH, MAX_KEY_LENGTH);  // Распределение длины
    uniform_int_distribution<int> charDist(MIN_CHAR, MAX_CHAR);             // Символьная область
    uniform_int_distribution<int> typeDist(0, 1);                           // Тип символа (буква или цифра)

    string key;
    int length = lenDist(gen);
    key.reserve(length);    // Предварительно резервируем пространство под строку

    for (int i = 0; i < length; i++) {
        int charCode = charDist(gen);   // Генерируем случайный ASCII-код
        if (typeDist(gen)) {
            key += static_cast<char>(charCode);       // Буква
        } else {
            // Преобразуем число в соответствующую цифру ('0'-'9')
            char digit = static_cast<char>(charCode % 10 + '0');
            key += digit;                            // Цифра
        }
    }

    return key;
}


// Основная функция шифрования текста методом гаммирования (XOR)
string GammaEncrypt(const string& text, string& key) {
    if (text.empty()) {
        throw invalid_argument("Текст для шифрования не может быть пустым");
    }

    if (key.empty()) {
        cout << "Хотите сгенерировать ключ? (да/нет): ";
        string userChoice;
        getline(cin, userChoice);
        
        if (userChoice == "да") {
            key = generateGammaKey();   // Автоматически создаем ключ
            cout << "Сгенерированный ключ: " << key << endl;
        } else {
            throw runtime_error("Гамма ключ не может быть пустым");
        }
    }

    string result;
    result.reserve(text.size());

    // Проходим по каждому символу открытого текста
    const size_t keyLen = key.size();
    for (size_t i = 0; i < text.size(); ++i) {
        // Выполняем операцию XOR текущего символа текста с соответствующим символом ключа
        result.push_back(text[i] ^ key[i % keyLen]);
    }

    return result;
}

// Функция расшифровки ранее зашифрованного текста
string GammaDecrypt(const string& text, const string& key) {
    if (text.empty()) {
        throw invalid_argument("Текст для дешифрования не может быть пустым");
    }
    if (key.empty()) {
        throw invalid_argument("Гамма ключ не может быть пустым");
    }

    // Поскольку XOR обратимая операция, дешифровка совпадает с процедурой шифрования
    return GammaEncrypt(text, const_cast<string&>(key));
}