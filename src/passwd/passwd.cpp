#include "passwd.h"
#include <iostream>
#include <fstream>
#include <string>
#include <termios.h>
#include <unistd.h>

using namespace std;

// Получает пароль от пользователя без отображения в консоли
string GetHiddenInput(const string& prompt) {
    cout << prompt;
    
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); // Получаем текущие настройки терминала
    newt = oldt;
    
    // Отключаем эхо-вывод (скрываем вводимые символы)
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Применяем новые настройки
    
    string password;
    getline(cin, password);
    
    // Восстанавливаем стандартные настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    cout << '\n';
    return password;
}

// Вычисляет хэш-значение пароля методом полиномиального хэширования
size_t CalculatePasswordHash(const string& password) {
    const size_t p = 41;      // Простое число для уменьшения коллизий
    const size_t mod = 1000000007; // Большой модуль для распределения
    size_t hash = 0;

    for (char c : password) {
        hash = (hash * p + static_cast<size_t>(c)) % mod;
    }

    return hash;
}

// Проверяет, имеет ли пользователь права на запись в файл паролей
bool HasWriteAccessToPasswordFile() {
    ofstream testFile(PASSWORD_FILE, ios::app);
    if (!testFile.is_open()) {
        return false; // Нет прав на запись
    }
    testFile.close();
    return true;     // Есть права
}

// Устанавливает новый пароль после проверки длины и подтверждения
void SetNewPassword() {
    if (!HasWriteAccessToPasswordFile()) {
        throw runtime_error("Недостаточно прав. Запустите программу через sudo.");
    }

    while (true) {
        string password = GetHiddenInput("Придумайте новый пароль (минимум 6 символов): ");

        if (password.length() < MIN_PASSWORD_LENGTH) {
            cerr << "Ошибка: Пароль должен содержать не менее "
                 << MIN_PASSWORD_LENGTH << " символов\n";
            continue;
        }

        string confirm = GetHiddenInput("Подтвердите пароль: ");
        
        if (password != confirm) {
            cerr << "Ошибка: Пароли не совпадают. Попробуйте снова.\n";
            continue;
        }

        ofstream file(PASSWORD_FILE, ios::trunc);
        if (!file) {
            throw runtime_error("Не удалось создать файл пароля");
        }

        file << CalculatePasswordHash(password);
        cout << "Пароль успешно установлен!\n";
        break;
    }
}

// Изменяет текущий пароль после его верификации
void ChangePassword() {
    if (!HasWriteAccessToPasswordFile()) {
        throw runtime_error("Недостаточно прав. Запустите программу через sudo.");
    }

    ifstream file(PASSWORD_FILE);
    size_t stored_hash;
    file >> stored_hash;

    int attemptsLeft = MAX_PASSWORD_ATTEMPTS;
    bool passwordVerified = false;

    while (attemptsLeft > 0 && !passwordVerified) {
        string current = GetHiddenInput("Введите текущий пароль: ");

        if (CalculatePasswordHash(current) == stored_hash) {
            passwordVerified = true;
        } else {
            attemptsLeft--;
            if (attemptsLeft > 0) {
                cerr << "Неверный пароль. Осталось попыток: " << attemptsLeft << '\n';
            }
        }
    }

    if (!passwordVerified) {
        cerr << "Не удалось изменить пароль. Возврат в главное меню." << '\n';
        return;
    }

    while (true) {
        string newPass = GetHiddenInput("Введите новый пароль: ");

        if (newPass.length() < MIN_PASSWORD_LENGTH) {
            cerr << "Ошибка: Пароль должен содержать не менее "
                 << MIN_PASSWORD_LENGTH << " символов\n";
            continue;
        }

        string confirm = GetHiddenInput("Подтвердите новый пароль: ");

        if (newPass != confirm) {
            cerr << "Ошибка: Новые пароли не совпадают\n";
            continue;
        }

        ofstream outFile(PASSWORD_FILE, ios::trunc);
        outFile << CalculatePasswordHash(newPass);
        cout << "Пароль успешно изменен!\n";
        return;
    }
}

// Проверяет, был ли установлен пароль ранее
bool IsPasswordSet() {
    ifstream file(PASSWORD_FILE);
    if (!file.is_open()) {
        return false; // Файл не существует или недоступен
    }
    return file.good() && file.peek() != ifstream::traits_type::eof();
}

// Проверяет, соответствует ли введённый пароль сохранённому хэшу
bool VerifyPassword(const string& password) {
    ifstream file(PASSWORD_FILE);
    size_t stored_hash;
    if (!(file >> stored_hash)) {
        throw runtime_error("Не удалось прочитать хэш из файла пароля");
    }

    return CalculatePasswordHash(password) == stored_hash;
}