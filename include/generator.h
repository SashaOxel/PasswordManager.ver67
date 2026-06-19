#pragma once
#include <string>

struct GenOptions {
    int length = 12;           // длина пароля
    bool useLetters = true;    // использовать буквы
    bool useDigits = true;     // использовать цифры
    bool useSymbols = false;   // использовать спецсимволы
    bool useUpper = true;      // использовать заглавные буквы
    bool useLower = true;      // использовать строчные буквы
    int count = 1;             // количество паролей для генерации
};

std::string generatePassword(const GenOptions& opts);