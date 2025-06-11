#ifndef SWITCH_H
#define SWITCH_H

#include <iostream>
#include <vector>
#include <string>
#include <dlfcn.h>

using namespace std;

// Загрузка библиотеки
void* loadLibrary(const string& libName);

// Получение функции из библиотеки
template<typename Func>
Func getFunction(void* handle, const string& funcName) {
    Func func = reinterpret_cast<Func>(dlsym(handle, funcName.c_str()));
    if (!func) {
        cerr << "Ошибка получения функции: " << dlerror() << '\n';
        throw runtime_error("Не удалось получить функцию: " + funcName);
    }
    return func;
}

// Функции шифрования/дешифрования текста и файлов
void encryptText(int subCode);
void decryptText(int subCode);
void encryptFile(int subCode);
void decryptFile(int subCode);

#endif