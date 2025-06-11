#include "menu_utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <filesystem>
#include <unistd.h>
#include <pwd.h>
#include <limits>
#include <cctype>

namespace fs = filesystem;

// Преобразует строку в нижний регистр для нормализации пользовательского ввода
static string ToLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return tolower(c); });
    return result;
}

// Проверяет, является ли путь абсолютным (путь начинается с '/')
bool isAbsolutePath(const string& path) {
    return !path.empty() && path[0] == '/';
}

// Создаёт отсутствующие директории при необходимости
static void EnsureDirectoryExists(const fs::path& dirPath) {
    if (!fs::exists(dirPath.parent_path())) {
        cout << "Директория " << dirPath.parent_path()
                  << " не существует. Создать её? (да/нет): ";
        string choice;
        getline(cin, choice);

        string lowerChoice = ToLower(choice);
        if (lowerChoice == "да" || lowerChoice == "yes" || lowerChoice == "д" || lowerChoice == "y") {
            try {
                fs::create_directories(dirPath.parent_path());
                cout << "Директории созданы: " << dirPath.parent_path() << "\n";
            } catch (...) {
                throw runtime_error("Не удалось создать директории");
            }
        } else {
            throw runtime_error("Отменено пользователем");
        }
    }
}

// отображает главное меню программы
void displayMainMenu() {
    cout << "\nГлавное меню:\n"
              << "0. Сменить пароль\n"
              << "1. Зашифровать текст\n"
              << "2. Расшифровать текст\n"
              << "3. Зашифровать файл\n"
              << "4. Расшифровать файл\n"
              << "5. Показать содержимое файла\n"
              << "6. Выход\n"
              << "Ваш выбор: ";
}

// Отображает подменю шифрования или дешифрования
void displaySubMenu(bool isEncrypt) {
    cout << (isEncrypt ? "\nМетоды шифрования:" : "\nМетоды дешифрования:") << "\n"
              << "1. " << (isEncrypt ? "Матричное шифрование" : "Матричное дешифрование") << "\n"
              << "2. Фиксированная перестановка\n"
              << "3. Гаммирование\n"
              << "4. Назад\n"
              << "Ваш выбор: ";
}

// Разбирает ввод пользователя из главного меню
MainMenu parseMainMenu(const string& input) {
    string value = ToLower(input);

    if (value == "0" || value == "сменить пароль") return CHANGE_PASSWORD;
    if (value == "1" || value == "зашифровать текст") return ENCRYPT_TEXT;
    if (value == "2" || value == "расшифровать текст") return DECRYPT_TEXT;
    if (value == "3" || value == "зашифровать файл") return ENCRYPT_FILE;
    if (value == "4" || value == "расшифровать файл") return DECRYPT_FILE;
    if (value == "5" || value == "показать содержимое файла") return SHOW_FILE_CONTENT;
    if (value == "6" || value == "выход") return EXIT;

    return INVALID;
}

// Разбирает ввод пользователя из подменю шифрования/дешифрования
SubMenu parseSubMenu(const string& input, bool isEncrypt) {
    string value = ToLower(input);

    if (value == "1" ||
        (isEncrypt && value == "матричное шифрование") ||
        (!isEncrypt && value == "матричное дешифрование")) {
        return MATRIX;
    }

    if (value == "2" || value == "фиксированная перестановка") return PERMUTATION;
    if (value == "3" || value == "гаммирование") return GAMMA;
    if (value == "4" || value == "назад") return BACK;

    return INVALID_SUB;
}

// Получает домашнюю директорию текущего пользователя.
string getHomeDirectory() {
    if (const char* homeDir = getenv("SUDO_USER")) {
        if (struct passwd* pw = getpwnam(homeDir)) {
            if (pw->pw_dir) return pw->pw_dir;
        }
    }

    if (struct passwd* pw = getpwuid(getuid())) {
        if (pw->pw_dir) return pw->pw_dir;
    }

    throw runtime_error("Не удалось определить домашнюю директорию");
}

// запрашивает у пользователя имя файла с валидацией
string GetFilename(const string& prompt) {
    static const regex validPathRegex(R"([^\x00-\x1F\"*:<>?|]+)");

    while (true) {
        cout << prompt << " (будет сохранено в ~/): ";
        string filename;
        getline(cin, filename);

        // Обработка ошибок ввода
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr << "Ошибка ввода. Пожалуйста, попробуйте снова.\n";
            continue;
        }

        // Проверка на пустое имя
        if (filename.empty()) {
            cerr << "Имя файла не может быть пустым. Пожалуйста, введите снова.\n";
            continue;
        }

        // Проверка на абсолютный путь
        if (isAbsolutePath(filename)) {
            cerr << "Ошибка: путь должен быть относительным (без '/'). Пример: folder/file.txt\n";
            continue;
        }

        // Проверка формата имени файла
        if (!regex_match(filename, validPathRegex)) {
            cerr << "Ошибка: используйте только допустимые символы для имени файла.\n";
            continue;
        }

        // Формируем полный путь
        string fullPath;
        try {
            fullPath = getHomeDirectory() + "/" + filename;
        } catch (const exception& e) {
            cerr << "Ошибка получения домашней директории: " << e.what() << "\n";
            continue;
        }

        // Проверяем существование входного файла
        if (prompt.find("исходного") != string::npos ||
            prompt.find("зашифрованного") != string::npos) {
            if (!fs::exists(fullPath)) {
                cerr << "Файл не существует: " << fullPath << ". Попробуйте снова.\n";
                continue;
            }
        } else {
            // Для выходного файла предлагаем создать недостающие директории
            try {
                EnsureDirectoryExists(fullPath);
            } catch (const exception&) {
                cerr << "Введите другой путь.\n";
                continue;
            }
        }

        return fullPath;
    }
}

// Записывает данные в файл и выводит уведомление
void WriteFileWithNotification(const string& filename, const string& content) {
    ofstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Не удалось создать файл: " + filename);
    }

    if (!file.write(content.data(), content.size())) {
        throw runtime_error("Ошибка записи в файл: " + filename);
    }

    cout << "Файл успешно сохранён по пути: " << filename << '\n';
}

// Показывает содержимое указанного файла
void ShowFileContent() {
    try {
        string filename = GetFilename("Введите имя файла");
        ifstream file(filename);

        if (!file.is_open()) {
            throw runtime_error("Не удалось открыть файл: " + filename);
        }

        cout << "\nСодержимое файла " << filename << ":\n"
                  << "--------------------------------\n";

        string line;
        while (getline(file, line)) {
            cout << line << "\n";
        }

        cout << "--------------------------------\n";
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}