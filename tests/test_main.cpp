#include <iostream>
#include <string>
#include <algorithm>
#include <windows.h>

#include "../include/crypto.h"
#include "../include/generator.h"

// Минималистичный тест-раннер
static int g_passed = 0;
static int g_failed = 0;

static void check(bool ok, const std::string& name) {
    if (ok) {
        std::cout << "[ОК]   " << name << "\n";
        ++g_passed;
    } else {
        std::cout << "[FAIL] " << name << "\n";
        ++g_failed;
    }
}

// Вспомогательные функции для генератора
static bool allIn(const std::string& s, const std::string& alphabet) {
    for (char c : s)
        if (alphabet.find(c) == std::string::npos) return false;
    return true;
}

static bool anyIn(const std::string& s, const std::string& alphabet) {
    for (char c : s)
        if (alphabet.find(c) != std::string::npos) return true;
    return false;
}

// Тесты шифрования
static void testCrypto() {
    std::cout << "\n--- Шифрование ---\n";

    {
        std::string plain = "Hello, World! 123";
        std::string key   = "secret";
        check(decrypt(encrypt(plain, key), key) == plain,
              "Шифрование и расшифрование возвращают исходный текст");
    }

    {
        std::string plain = "no change";
        check(encrypt(plain, "") == plain, "Пустой ключ: шифрование не меняет данные");
        check(decrypt(plain, "") == plain, "Пустой ключ: расшифрование не меняет данные");
    }

    {
        check(encrypt("", "key").empty(), "Пустые данные: шифрование возвращает пустую строку");
        check(decrypt("", "key").empty(), "Пустые данные: расшифрование возвращает пустую строку");
    }

    {
        std::string plain = "password";
        check(encrypt(plain, "key1") != encrypt(plain, "key2"),
              "Разные ключи дают разные шифртексты");
    }

    {
        std::string plain = "exactly sixteen!";
        std::string enc   = encrypt(plain, "k");
        check(enc.size() == plain.size(), "Длина шифртекста совпадает с длиной исходного текста");
    }

    {
        std::string plain = "hi";
        std::string key   = "a_very_long_key_here";
        check(decrypt(encrypt(plain, key), key) == plain,
              "Ключ длиннее данных: шифрование корректно");
    }

    {
        std::string plain(8, '\0');
        std::string key = "k";
        check(decrypt(encrypt(plain, key), key) == plain,
              "Бинарные данные с нулевыми байтами: шифрование корректно");
    }
}

// Тесты генератора паролей
static void testGenerator() {
    std::cout << "\n--- Генератор паролей ---\n";

    const std::string UPPER   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string LOWER   = "abcdefghijklmnopqrstuvwxyz";
    const std::string DIGITS  = "0123456789";
    const std::string SYMBOLS = "!@#$%^&*()_+-=[]{}|;:,.<>?";

    // Стандартные настройки: длина 12
    {
        GenOptions o;
        o.length = 12;
        std::string p = generatePassword(o);
        check(p.size() == 12, "Стандартные настройки: длина пароля равна 12");
    }

    // Длина 32
    {
        GenOptions o;
        o.length = 32;
        std::string p = generatePassword(o);
        check(p.size() == 32, "Длина пароля 32: результат содержит ровно 32 символа");
    }

    // Только строчные буквы
    {
        GenOptions o;
        o.length     = 20;
        o.useUpper   = false;
        o.useLower   = true;
        o.useLetters = true;
        o.useDigits  = false;
        o.useSymbols = false;
        std::string p = generatePassword(o);
        check(p.size() == 20 && allIn(p, LOWER),
              "Только строчные буквы: все символы пароля строчные");
    }

    // Только заглавные буквы
    {
        GenOptions o;
        o.length     = 20;
        o.useUpper   = true;
        o.useLower   = false;
        o.useLetters = true;
        o.useDigits  = false;
        o.useSymbols = false;
        std::string p = generatePassword(o);
        check(p.size() == 20 && allIn(p, UPPER),
              "Только заглавные буквы: все символы пароля заглавные");
    }

    // Только цифры
    {
        GenOptions o;
        o.length     = 20;
        o.useLetters = false;
        o.useUpper   = false;
        o.useLower   = false;
        o.useDigits  = true;
        o.useSymbols = false;
        std::string p = generatePassword(o);
        check(p.size() == 20 && allIn(p, DIGITS),
              "Только цифры: все символы пароля цифры");
    }

    // Только спецсимволы
    {
        GenOptions o;
        o.length     = 20;
        o.useLetters = false;
        o.useUpper   = false;
        o.useLower   = false;
        o.useDigits  = false;
        o.useSymbols = true;
        std::string p = generatePassword(o);
        check(p.size() == 20 && allIn(p, SYMBOLS),
              "Только спецсимволы: все символы пароля специальные");
    }

    // Со спецсимволами: пароль длиной 50 должен содержать хотя бы один
    {
        GenOptions o;
        o.length     = 50;
        o.useLetters = true;
        o.useUpper   = true;
        o.useLower   = true;
        o.useDigits  = true;
        o.useSymbols = true;
        std::string p = generatePassword(o);
        check(anyIn(p, SYMBOLS), "Все типы включены: в пароле длиной 50 есть хотя бы один спецсимвол");
        check(anyIn(p, DIGITS),  "Все типы включены: в пароле длиной 50 есть хотя бы одна цифра");
    }

    // Ничего не выбрано — возвращает строку с ошибкой (не пустую)
    {
        GenOptions o;
        o.useLetters = false;
        o.useUpper   = false;
        o.useLower   = false;
        o.useDigits  = false;
        o.useSymbols = false;
        std::string p = generatePassword(o);
        check(!p.empty(), "Ничего не выбрано: функция возвращает сообщение об ошибке");
    }

    // Два вызова подряд дают разные пароли (вероятностно; длина 16)
    {
        GenOptions o;
        o.length = 16;
        check(generatePassword(o) != generatePassword(o),
              "Два последовательных вызова дают разные пароли");
    }
}

// ──────────────────────────────────────────────
// main
// ──────────────────────────────────────────────
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "=============================\n";
    std::cout << "  Тесты менеджера паролей\n";
    std::cout << "=============================\n";

    testCrypto();
    testGenerator();

    std::cout << "\n=============================\n";
    std::cout << "Пройдено: " << g_passed << "\n";
    std::cout << "Провалено: " << g_failed << "\n";
    std::cout << "=============================\n";

    return g_failed == 0 ? 0 : 1;
}
