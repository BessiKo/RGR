#include "switch/switch.h"
#include "passwd/passwd.h"
#include "menu_utils/menu_utils.h"
#include <locale>

using namespace std;

int main() {
    setlocale(LC_ALL, "");

    try {
        // Проверяем, был ли ранее установлен пароль
        if (!IsPasswordSet()) {
            try {
                SetNewPassword(); // Установка нового пароля
            } catch (const exception& e) {
                cerr << "Ошибка при установке пароля: " << e.what() << '\n';
                return 1;
            }
        }

        // Цикл авторизации: пользователь пытается ввести пароль
        bool authenticated = false;
        for (int attempt = 0; attempt < MAX_PASSWORD_ATTEMPTS && !authenticated; ++attempt) {
            string password = GetHiddenInput("Введите пароль: ");

            if (VerifyPassword(password)) {
                authenticated = true;
            } else if (attempt < MAX_PASSWORD_ATTEMPTS - 1) {
                cerr << "Неверный пароль. Осталось попыток: "
                     << MAX_PASSWORD_ATTEMPTS - attempt - 1 << '\n';
            } else {
                cerr << "Превышено количество попыток" << '\n';
                return 1;
            }
        }

        // Основной цикл программы после успешной авторизации
        while (authenticated) {
            displayMainMenu(); // Показываем главное меню
            string userInput;
            getline(cin, userInput); // Получаем выбор пользователя
            MainMenu choice = parseMainMenu(userInput);

            if (choice == EXIT) {
                cout << "До встречи!" << '\n';
                break;
            }

            if (choice == INVALID) {
                cout << "Некорректный выбор. Попробуйте снова." << '\n';
                continue;
            }

            // Обработка команд главного меню
            if (choice == CHANGE_PASSWORD) {
                try {
                    ChangePassword(); // Смена пароля
                } catch (const exception& e) {
                    cerr << "Ошибка при смене пароля: " << e.what() << '\n';
                }
                continue;
            }

            if (choice == SHOW_FILE_CONTENT) {
                try {
                    ShowFileContent(); // Показ содержимого файла
                } catch (const exception& e) {
                    cerr << "Ошибка: " << e.what() << '\n';
                }
                continue;
            }

            // Если выбран процесс шифрования или дешифрования
            bool isEncrypt = (choice == ENCRYPT_TEXT || choice == ENCRYPT_FILE);

            // Переходим в подменю выбора метода
            while (true) {
                displaySubMenu(isEncrypt); // Отображаем подменю
                string subInput;
                getline(cin, subInput);
                SubMenu subChoice = parseSubMenu(subInput, isEncrypt);

                if (subChoice == BACK) {
                    break; // Возвращаемся в главное меню
                }

                if (subChoice == INVALID_SUB) {
                    cout << "Некорректный выбор. Попробуйте снова." << '\n';
                    continue;
                }

                // Выполняем операцию шифрования/дешифрования
                try {
                    switch (choice) {
                        case ENCRYPT_TEXT:
                            encryptText(subChoice);
                            break;
                        case DECRYPT_TEXT:
                            decryptText(subChoice);
                            break;
                        case ENCRYPT_FILE:
                            encryptFile(subChoice);
                            break;
                        case DECRYPT_FILE:
                            decryptFile(subChoice);
                            break;
                        default:
                            break;
                    }
                } catch (const exception& e) {
                    cerr << "Ошибка при обработке данных: " << e.what() << '\n';
                }
                break;
            }
        }
    } catch (const exception& e) {
        cerr << "Критическая ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}