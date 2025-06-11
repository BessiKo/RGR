#!/bin/bash

set -e

echo "Penguin is waiting you at the end 🦦"

# Проверка зависимостей
compilerExists() {
    if command -v g++ &> /dev/null && command -v make &> /dev/null; then
        return 0
    else
        return 1
    fi
}

# Установка зависимостей
if ! compilerExists; then
    echo "Installing dependences..."
    if [ -f /etc/debian_version ]; then
        sudo apt install -y g++ make
    elif [ -f /etc/arch-release ]; then
        sudo pacman -S --noconfirm gcc make
    else
        echo "He will eat your ear 🦝"
        exit 1
    fi
fi

# Сборка
cd build
make install

# Пароль
echo -n "Задайте пароль: "
read -s passwd

countHash() {
    local passwd="$1"
    local hashValue=0
    local p=41
    local mod=1000000007

    for ((i=0; i<${#passwd}; i++)); do
        local c=$(printf '%d' "'${passwd:i:1}")
        hashValue=$(( (hashValue * p + c) % mod ))
    done

    echo "$hashValue"
}

hash_result=$(countHash "$passwd")

echo "$hash_result" | sudo tee /etc/password_ciphers >/dev/null

echo
echo "The end 🐧"