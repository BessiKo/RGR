#include "file.h"

// Проверяет, существует ли файл и является ли он именно файлом, а не директорией
bool FileExists(const string& filename) {
    struct stat info;
    
    if (stat(filename.c_str(), &info) != 0) {
        return false; // Файл не существует
    } else if (S_ISDIR(info.st_mode)) {
        cerr << "Ошибка: '" << filename << "' — это директория, а не файл.\n";
        return false; // Это директория, а не обычный файл
    }

    return true; // Это корректный файл
}

// Считывает содержимое файла в строку
string ReadFile(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }

    streamsize size = file.tellg(); // Получаем размер файла
    if (size <= 0) {
        return ""; // Файл пустой
    }

    file.seekg(0, ios::beg); // Перемещаемся в начало файла
    string buffer(size, '\0'); // Выделяем память под содержимое

    if (!file.read(&buffer[0], size)) {
        throw runtime_error("Ошибка чтения из файла: " + filename);
    }

    return buffer;
}

// Записывает строку в файл
void WriteFile(const string& filename, const string& content) {
    ofstream file(filename, ios::binary);

    if (!file.is_open()) {
        throw runtime_error("Не удалось создать файл: " + filename);
    }

    if (!file.write(content.data(), content.size())) {
        throw runtime_error("Ошибка записи в файл: " + filename);
    }
}

// Записывает вектор строк в файл
void WriteFileVector(const string& filename, const vector<string>& content) {
    ofstream file(filename, ios::binary);

    if (!file.is_open()) {
        throw runtime_error("Не удалось создать файл: " + filename);
    }

    for (const auto& str : content) {
        if (!file.write(str.data(), str.size())) {
            throw runtime_error("Ошибка записи в файл: " + filename);
        }
    }
}