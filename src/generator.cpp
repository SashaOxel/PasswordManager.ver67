#include "../include/generator.h"
#include <iostream>
#include <random>
#include <ctime>
#include <string>

using namespace std;

// Глобальный генератор случайных чисел
static random_device rd;
static mt19937 gen(rd());

string generatePassword(const GenOptions& opts) {
    // Проверка: хоть что-то должно быть выбрано
    if (!opts.useLetters && !opts.useDigits && !opts.useSymbols) {
        return "Ошибка: выберите хотя бы один тип символов!";
    }
    
    // Собираем набор символов
    string chars;
    
    // Буквы
    if (opts.useLetters) {
        if (opts.useUpper) {
            chars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        }
        if (opts.useLower) {
            chars += "abcdefghijklmnopqrstuvwxyz";
        }
    }
    
    // Цифры
    if (opts.useDigits) {
        chars += "0123456789";
    }
    
    // Спецсимволы
    if (opts.useSymbols) {
        chars += "!@#$%^&*()_+-=[]{}|;:,.<>?";
    }
    
    // Если набор пустой, возвращаем ошибку
    if (chars.empty()) {
        return "Ошибка: нет доступных символов для генерации!";
    }
    
    // Генерируем пароль
    string password;
    uniform_int_distribution<> dist(0, chars.size() - 1);
    
    for (int i = 0; i < opts.length; ++i) {
        password += chars[dist(gen)];
    }
    
    return password;
}

// Функция для интерактивной генерации с параметрами
string generatePasswordInteractive() {
    GenOptions opts;
    
    cout << "\n=== Настройки генерации пароля ===" << endl;
    
    // Длина
    cout << "Длина пароля (по умолчанию 12): ";
    string input;
    getline(cin, input);
    if (!input.empty()) {
        opts.length = stoi(input);
    }
    
    // Заглавные буквы
    cout << "Использовать заглавные буквы? (y/n, по умолчанию y): ";
    getline(cin, input);
    if (input == "n" || input == "N") opts.useUpper = false;
    
    // Строчные буквы
    cout << "Использовать строчные буквы? (y/n, по умолчанию y): ";
    getline(cin, input);
    if (input == "n" || input == "N") opts.useLower = false;
    
    // Буквы в целом (если отключили и заглавные и строчные)
    opts.useLetters = opts.useUpper || opts.useLower;
    
    // Цифры
    cout << "Использовать цифры? (y/n, по умолчанию y): ";
    getline(cin, input);
    if (input == "n" || input == "N") {
        opts.useDigits = false;
    } else if (input.empty() || input == "y" || input == "Y") {
        opts.useDigits = true;
    }
    
    // Спецсимволы
    cout << "Использовать спецсимволы? (y/n, по умолчанию n): ";
    getline(cin, input);
    if (input == "y" || input == "Y") {
        opts.useSymbols = true;
    } else if (input.empty() || input == "n" || input == "N") {
        opts.useSymbols = false;
    }
    
    // Количество паролей
    cout << "Сколько паролей сгенерировать? (по умолчанию 1): ";
    getline(cin, input);
    if (!input.empty()) {
        opts.count = stoi(input);
    }
    
    // Генерируем пароли
    cout << "\n=== Сгенерированные пароли ===" << endl;
    for (int i = 0; i < opts.count; i++) {
        string password = generatePassword(opts);
        cout << i + 1 << ". " << password << endl;
    }
    
    // Возвращаем первый пароль (для использования в записи)
    return generatePassword(opts);
}

// Функция для быстрой генерации пароля (для добавления в запись)
string generatePasswordFast(int length = 12) {
    GenOptions opts;
    opts.length = length;
    opts.useLetters = true;
    opts.useDigits = true;
    opts.useSymbols = false;
    opts.useUpper = true;
    opts.useLower = true;
    return generatePassword(opts);
}