# Расчетно-графическое работа по программированию

Программа шифрования/дешифрования текстовых данных и файлов, разработанная в рамках расчетно-графической работы (РГР).  
Позволяет использовать различные алгоритмы шифрования через динамически подключаемые библиотеки.

## Основные цели проекта:

- Реализация приложения с поддержкой динамических библиотек (`.so`).
- Предоставление пользовательского интерфейса для шифрования/дешифрования текста и файлов.
- Демонстрация работы алгоритмов шифрования в учебных целях.

## Поддерживаемые алгоритмы шифрования:

- **Матричное шифрование**: заполнение матрицы по спирали из центра.
- **Фиксированная перестановка**: перестановка символов по заданному шаблону.
- **Гаммирование (XOR)**: побайтовое шифрование с использованием гаммы.

## Возможности

- Шифрование и дешифрование:
  - Вводимого пользователем текста
  - Текстовых и бинарных файлов
- Генерация ключей для гаммирования
- Поддержка UTF-8 (включая кириллицу)
- Безопасный ввод пароля
- Многоязычный интерфейс (русский)

## Архитектура

Проект разделён на модули:

- `switch/` — загрузка и работа с динамическими библиотеками
- `file/` — работа с файлами
- `menu_utils/` — навигация по меню
- `passwd/` — управление паролем
- `general/` — общие функции (например, разбиение строки на символы)
- `lib/gamma/`, `lib/matrix/`, `lib/permutation/` — реализация алгоритмов шифрования


## Требования к системе

- **ОС**: Linux (Ubuntu, Arch и др.)
- **Компилятор**: g++ (версия 7+)
- **Инструменты**: make, ldconfig

## Установка и сборка

```bash
# 1. Клонирование репозитория
git clone https://github.com/BessiKo/RGR.git
cd RGR

# 2. Сборка проекта
chmod +x install.sh
./install.sh
```

## Использование

После установки запустите:
```bash
ciphers
```

Далее следуйте инструкциям в консоли:
- Выберите действие: шифрование/дешифрование текста или файла
- Выберите метод шифрования
- Введите данные или укажите путь к файлу
- Получите результат

## Примеры использования:

### Шифрование текста:

1. Выберите опцию `1` (Шифровать текст)
2. Выберите метод шифрования, например `3` (Гаммирование)
3. Введите текст и ключ
4. Результат будет сохранён в указанный файл

### Дешифрование файла:

1. Выберите `4` (Дешифровать файл)
2. Укажите путь к зашифрованному файлу и ключ
3. Выберите метод дешифрования, совпадающий с тем, который использовался для шифрования