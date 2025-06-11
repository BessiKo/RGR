#include "switch.h"
#include "file/file.h"
#include "general/general.h"
#include "menu_utils/menu_utils.h"

using namespace std;

// Загружает динамическую библиотеку.
void* loadLibrary(const string& libName) {
    void* handle = dlopen(libName.c_str(), RTLD_LAZY);
    if (!handle) {
        // cerr << "Ошибка загрузки библиотеки: " << dlerror() << '\n';
        throw runtime_error("Не удалось загрузить библиотеку: " + libName);
    }
    return handle;
}

// Шифрует текст, используя выбранный алгоритм шифрования
void encryptText(int subCode) {
    try {
        string outputFilePath = GetFilename("Введите имя выходного файла: ");
        string text = GetTextInput();

        if (subCode == MATRIX) {
            void* matrixHandle = loadLibrary("build/libmatrix.so");
            auto matrixEncrypt = getFunction<vector<string>(*)(const vector<string>&)>(matrixHandle, "MatrixEncrypt");
            WriteFileVector(outputFilePath, matrixEncrypt(SplitUtf8String(text)));
            dlclose(matrixHandle);
        } else if (subCode == PERMUTATION) {
            void* permHandle = loadLibrary("build/libpermutation.so");
            auto permEncrypt = getFunction<vector<string>(*)(const vector<string>&)>(permHandle, "PermutationEncrypt");
            WriteFileVector(outputFilePath, permEncrypt(SplitUtf8String(text)));
            dlclose(permHandle);
        } else if (subCode == GAMMA) {
            void* gammaHandle = loadLibrary("build/libgamma.so");
            auto gammaEncrypt = getFunction<string(*)(const string&, const string&)>(gammaHandle, "GammaEncrypt");
            string encryptionKey;
            cout << "Введите ключ: ";
            getline(cin, encryptionKey);
            WriteFile(outputFilePath, gammaEncrypt(text, encryptionKey));
            dlclose(gammaHandle);
        }

        cout << "Файл успешно сохранён по пути: " << outputFilePath << '\n';

    } catch (const exception& e) {
        // cerr << "Ошибка при шифровании текста: " << e.what() << '\n';
        throw;
    }
}

// Дешифрует текст, используя выбранный алгоритм дешифрования.
void decryptText(int subCode) {
    try {
        string outputFilePath = GetFilename("Введите имя выходного файла: ");
        string text = GetTextInput();

        if (subCode == MATRIX) {
            void* matrixHandle = loadLibrary("build/libmatrix.so");
            auto matrixDecrypt = getFunction<vector<string>(*)(const vector<string>&)>(matrixHandle, "MatrixDecrypt");
            WriteFileVector(outputFilePath, matrixDecrypt(SplitUtf8String(text)));
            dlclose(matrixHandle);
        } else if (subCode == PERMUTATION) {
            void* permHandle = loadLibrary("build/libpermutation.so");
            auto permDecrypt = getFunction<vector<string>(*)(const vector<string>&)>(permHandle, "PermutationDecrypt");
            WriteFileVector(outputFilePath, permDecrypt(SplitUtf8String(text)));
            dlclose(permHandle);
        } else if (subCode == GAMMA) {
            void* gammaHandle = loadLibrary("build/libgamma.so");
            auto gammaDecrypt = getFunction<string(*)(const string&, const string&)>(gammaHandle, "GammaDecrypt");
            string decryptionKey;
            cout << "Введите ключ: ";
            getline(cin, decryptionKey);
            WriteFile(outputFilePath, gammaDecrypt(text, decryptionKey));
            dlclose(gammaHandle);
        }

        cout << "Файл успешно дешифрован и сохранён по пути: " << outputFilePath << '\n';

    } catch (const exception& e) {
        // cerr << "Ошибка при дешифровании текста: " << e.what() << '\n';
        throw;
    }
}

// Шифрует содержимое файла выбранным алгоритмом.
void encryptFile(int subCode) {
    try {
        string inputFilePath = GetFilename("Введите путь к исходному файлу: ");
        string outputFilePath = GetFilename("Введите имя выходного файла: ");

        if (!FileExists(inputFilePath)) {
            throw runtime_error("Файл не существует: " + inputFilePath);
        }

        string fileContent = ReadFile(inputFilePath);

        if (fileContent.empty()) {
            throw runtime_error("Исходный файл пустой.");
        }

        if (subCode == MATRIX) {
            void* matrixHandle = loadLibrary("build/libmatrix.so");
            auto matrixEncrypt = getFunction<vector<string>(*)(const vector<string>&)>(matrixHandle, "MatrixEncrypt");
            WriteFileVector(outputFilePath, matrixEncrypt(SplitUtf8String(fileContent)));
            dlclose(matrixHandle);
        } else if (subCode == PERMUTATION) {
            void* permHandle = loadLibrary("build/libpermutation.so");
            auto permEncrypt = getFunction<vector<string>(*)(const vector<string>&)>(permHandle, "PermutationEncrypt");
            WriteFileVector(outputFilePath, permEncrypt(SplitUtf8String(fileContent)));
            dlclose(permHandle);
        } else if (subCode == GAMMA) {
            void* gammaHandle = loadLibrary("build/libgamma.so");
            auto gammaEncrypt = getFunction<string(*)(const string&, string&)>(gammaHandle, "GammaEncrypt");
            string encryptionKey;
            cout << "Введите ключ: ";
            getline(cin, encryptionKey);
            WriteFile(outputFilePath, gammaEncrypt(fileContent, encryptionKey));
            dlclose(gammaHandle);
        }

        cout << "Файл успешно зашифрован и сохранён по пути: " << outputFilePath << '\n';

    } catch (const exception& e) {
        // cerr << "Ошибка при шифровании файла: " << e.what() << '\n';
        throw;
    }
}

// Дешифрует содержимое файла выбранным алгоритмом.
void decryptFile(int subCode) {
    try {
        string inputFilePath = GetFilename("Введите путь к зашифрованному файлу: ");
        string outputFilePath = GetFilename("Введите имя выходного файла: ");
        string encryptedContent = ReadFile(inputFilePath);

        if (encryptedContent.empty()) {
            throw runtime_error("Файл пустой.");
        }

        if (subCode == MATRIX) {
            void* matrixHandle = loadLibrary("build/libmatrix.so");
            auto matrixDecrypt = getFunction<vector<string>(*)(const vector<string>&)>(matrixHandle, "MatrixDecrypt");
            WriteFileVector(outputFilePath, matrixDecrypt(SplitUtf8String(encryptedContent)));
            dlclose(matrixHandle);
        } else if (subCode == PERMUTATION) {
            void* permHandle = loadLibrary("build/libpermutation.so");
            auto permDecrypt = getFunction<vector<string>(*)(const vector<string>&)>(permHandle, "PermutationDecrypt");
            WriteFileVector(outputFilePath, permDecrypt(SplitUtf8String(encryptedContent)));
            dlclose(permHandle);
        } else if (subCode == GAMMA) {
            void* gammaHandle = loadLibrary("build/libgamma.so");
            auto gammaDecrypt = getFunction<string(*)(const string&, const string&)>(gammaHandle, "GammaDecrypt");
            string decryptionKey;
            cout << "Введите ключ: ";
            getline(cin, decryptionKey);
            WriteFile(outputFilePath, gammaDecrypt(encryptedContent, decryptionKey));
            dlclose(gammaHandle);
        }

        cout << "Файл успешно расшифрован и сохранён по пути: " << outputFilePath << '\n';

    } catch (const exception& e) {
        // cerr << "Ошибка при дешифровании файла: " << e.what() << '\n';
        throw;
    }
}