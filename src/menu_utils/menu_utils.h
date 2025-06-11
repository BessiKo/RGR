#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

// Перечисления для меню
enum MainMenu {
    CHANGE_PASSWORD = 0,
    ENCRYPT_TEXT = 1,
    DECRYPT_TEXT = 2,
    ENCRYPT_FILE = 3,
    DECRYPT_FILE = 4,
    SHOW_FILE_CONTENT = 5,
    EXIT = 6,
    INVALID = -1
};

enum SubMenu {
    MATRIX = 1,
    PERMUTATION = 2,
    GAMMA = 3,
    BACK = 4,
    INVALID_SUB = -1
};

// Функции работы с меню
MainMenu parseMainMenu(const std::string& input);
SubMenu parseSubMenu(const std::string& input, bool isEncrypt);

void displayMainMenu();
void displaySubMenu(bool isEncrypt);

// Работа с файлами
std::string GetFilename(const std::string& prompt);
void WriteFileWithNotification(const std::string& filename, const std::string& content);
void ShowFileContent();

#endif