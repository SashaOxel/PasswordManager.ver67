#include "../include/menu.h"
#include "../include/records.h"
#include "../include/generator.h"
#include "../include/crypto.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;


static int readInt() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Введите число: ";
    }
    return val;
}

static string readLine() {
    string str;
    if (cin.peek() == '\n') {
        cin.ignore();
    }
    getline(cin, str);
    return str;
}

static void recordsMenu() {
    int choice;
    do {
        cout << "\n=== Записи ===" << endl;
        cout << "1. Показать все записи" << endl;
        cout << "2. Добавить запись" << endl;
        cout << "3. Редактировать запись" << endl;
        cout << "4. Удалить запись" << endl;
        cout << "5. Вывести пароль" << endl;
        cout << "0. Назад" << endl;
        cout << "> ";
        choice = readInt();

        switch (choice) {
            case 1: showAllRecords(); break;
            case 2: addRecord(); break;
            case 3: editRecord(); break;
            case 4: deleteRecord(); break;
            case 5: printPassword(); break;
            case 0: break;
            default: cout << "Неверный выбор." << endl;
        }
    } while (choice != 0);
}

// Обновленное меню генерации пароля
static void generatorMenu() {
    int choice;
    do {
        cout << "\n=== Генерация пароля ===" << endl;
        cout << "1. Быстрая генерация (стандартные настройки)" << endl;
        cout << "2. Настраиваемая генерация" << endl;
        cout << "3. Сгенерировать несколько паролей" << endl;
        cout << "0. Назад" << endl;
        cout << "> ";
        choice = readInt();

        switch (choice) {
            case 1: {
                // Быстрая генерация
                GenOptions opts;
                opts.length = 12;
                opts.useLetters = true;
                opts.useUpper = true;
                opts.useLower = true;
                opts.useDigits = true;
                opts.useSymbols = false;
                
                string pass = generatePassword(opts);
                cout << "\nСгенерированный пароль: " << pass << endl;
                break;
            }
            case 2: {
                // Настраиваемая генерация
                GenOptions opts;
                
                cout << "\n=== Настройка параметров ===" << endl;
                cout << "Длина пароля (4-64, по умолчанию 12): ";
                opts.length = readInt();
                if (opts.length < 4) opts.length = 4;
                if (opts.length > 64) opts.length = 64;
                
                cout << "Использовать заглавные буквы? (1/0, по умолчанию 1): ";
                opts.useUpper = readInt() != 0;
                
                cout << "Использовать строчные буквы? (1/0, по умолчанию 1): ";
                opts.useLower = readInt() != 0;
                
                opts.useLetters = opts.useUpper || opts.useLower;
                
                cout << "Использовать цифры? (1/0, по умолчанию 1): ";
                opts.useDigits = readInt() != 0;
                
                cout << "Использовать спецсимволы? (1/0, по умолчанию 0): ";
                opts.useSymbols = readInt() != 0;
                
                // Проверка
                if (!opts.useLetters && !opts.useDigits && !opts.useSymbols) {
                    cout << "Ошибка: выберите хотя бы один тип символов! Использую стандартные настройки." << endl;
                    opts.useLetters = true;
                    opts.useUpper = true;
                    opts.useLower = true;
                    opts.useDigits = true;
                    opts.useSymbols = false;
                }
                
                string pass = generatePassword(opts);
                cout << "\nСгенерированный пароль: " << pass << endl;
                break;
            }
            case 3: {
                // Генерация нескольких паролей
                GenOptions opts;
                
                cout << "\n=== Настройка параметров ===" << endl;
                cout << "Длина пароля (4-64, по умолчанию 12): ";
                opts.length = readInt();
                if (opts.length < 4) opts.length = 4;
                if (opts.length > 64) opts.length = 64;
                
                cout << "Использовать заглавные буквы? (1/0, по умолчанию 1): ";
                opts.useUpper = readInt() != 0;
                
                cout << "Использовать строчные буквы? (1/0, по умолчанию 1): ";
                opts.useLower = readInt() != 0;
                
                opts.useLetters = opts.useUpper || opts.useLower;
                
                cout << "Использовать цифры? (1/0, по умолчанию 1): ";
                opts.useDigits = readInt() != 0;
                
                cout << "Использовать спецсимволы? (1/0, по умолчанию 0): ";
                opts.useSymbols = readInt() != 0;
                
                cout << "Количество паролей: ";
                int count = readInt();
                if (count < 1) count = 1;
                if (count > 20) count = 20;
                
                // Проверка
                if (!opts.useLetters && !opts.useDigits && !opts.useSymbols) {
                    cout << "Ошибка: выберите хотя бы один тип символов! Использую стандартные настройки." << endl;
                    opts.useLetters = true;
                    opts.useUpper = true;
                    opts.useLower = true;
                    opts.useDigits = true;
                    opts.useSymbols = false;
                }
                
                cout << "\n=== Сгенерированные пароли ===" << endl;
                for (int i = 0; i < count; i++) {
                    string pass = generatePassword(opts);
                    cout << i + 1 << ". " << pass << endl;
                }
                break;
            }
            case 0: break;
            default: cout << "Неверный выбор." << endl;
        }
    } while (choice != 0);
}

static void encryptionMenu() {
    int choice;
    do {
        cout << "\n=== Шифрование файлов ===" << endl;
        cout << "Алгоритм: Бегущий ключ" << endl;
        cout << "1. Зашифровать файл" << endl;
        cout << "2. Расшифровать файл" << endl;
        cout << "0. Назад" << endl;
        cout << "> ";
        choice = readInt();

        switch (choice) {
            case 1: {
                string path, key;
                cout << "Путь к файлу: ";
                cin >> path;
                cout << "Ключ: ";
                cin >> key;
                encryptFile(path, key);
                break;
            }
            case 2: {
                string path, key;
                cout << "Путь к файлу: ";
                cin >> path;
                cout << "Ключ: ";
                cin >> key;
                decryptFile(path, key);
                break;
            }
            case 0: break;
            default: cout << "Неверный выбор." << endl;
        }
    } while (choice != 0);
}

static void workMenu() {
    int choice;
    do {
        cout << "\n=== Главное меню ===" << endl;
        cout << "1. Записи" << endl;
        cout << "2. Генерация пароля" << endl;
        cout << "3. Шифрование файлов" << endl;
        cout << "0. Выйти из хранилища" << endl;
        cout << "> ";
        choice = readInt();

        switch (choice) {
            case 1: recordsMenu(); break;
            case 2: generatorMenu(); break;
            case 3: encryptionMenu(); break;
            case 0: break;
            default: cout << "Неверный выбор." << endl;
        }
    } while (choice != 0);
}

void runMainMenu() {
    int choice;
    do {
        cout << "\n=== Менеджер паролей ===" << endl;
        cout << "1. Войти" << endl;
        cout << "2. Создать новое хранилище" << endl;
        cout << "0. Выход" << endl;
        cout << "> ";
        choice = readInt();

        switch (choice) {
            case 1: {
                string name, master;
                cout << "Имя хранилища: ";
                cin >> name;
                cout << "Мастер-пароль: ";
                cin >> master;
                if (initRecords(name, master, false))
                    workMenu();
                break;
            }
            case 2: {
                string name, master;
                cout << "Имя хранилища: ";
                cin >> name;
                cout << "Придумайте мастер-пароль: ";
                cin >> master;
                if (initRecords(name, master, true))
                    workMenu();
                break;
            }
            case 0:
                cout << "До свидания!" << endl;
                break;
            default:
                cout << "Неверный выбор." << endl;
        }
    } while (choice != 0);
}