#include "general.h"
#include <stdexcept>

// Преобразует строку в нижний регистр
string ToLower(const string& str) {
    string result = str;
    for (char &c : result) {
        c = tolower(c);
    }
    return result;
}

// Получает текстовый ввод от пользователя
string GetTextInput() {
    string text;
    cout << "Введите текст для шифрования (завершите пустой строкой):\n";

    while (true) {
        string line;
        getline(cin, line);

        if (line.empty()) break;

        text += line + "\n";
    }

    return text;
}

// Получает пароль от пользователя через безопасный ввод
string GetPasswordInput(const string& prompt) {
    string password;
    cout << prompt;
    getline(cin, password);

    if (password.empty()) {
        throw invalid_argument("Пароль не может быть пустым");
    }

    return password;
}

// Разбивает UTF-8 строку на отдельные символы
vector<string> SplitUtf8String(const string& str) {
    vector<string> symbols;

    for (size_t i = 0; i < str.size();) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        int char_len = 1;

        // Определяем длину UTF-8 символа по старшим битам
        if (c < 0x80) {
            char_len = 1;
        } else if ((c & 0xE0) == 0xC0) {
            char_len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            char_len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            char_len = 4;
        } else {
            throw runtime_error("Недопустимая UTF-8 последовательность");
        }

        // Проверяем, что данные не обрываются на середине символа
        if (i + char_len > str.size()) {
            throw runtime_error("Недопустимая длина UTF-8 последовательности");
        }

        // Сохраняем текущий символ как отдельную строку
        symbols.push_back(str.substr(i, char_len));
        i += char_len;
    }

    return symbols;
}