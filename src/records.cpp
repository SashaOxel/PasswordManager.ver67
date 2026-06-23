#include "../include/records.h"
#include "../include/crypto.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;

static vector<Record> records;
static string vaultFile;
static string masterKey;

const string VAULT_HEADER = "VAULT_OK";

static int readInt() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Введите число: ";
    }
    return val;
}

static void saveRecords() {
    stringstream ss;
    ss << VAULT_HEADER << "\n";
    for (const auto& rec : records) {
        ss << rec.service  << "\n";
        ss << rec.title    << "\n";
        ss << rec.login    << "\n";
        ss << rec.password << "\n";
        ss << rec.url      << "\n";
        ss << rec.note     << "\n";
    }
    string encrypted = encrypt(ss.str(), masterKey);
    ofstream file(vaultFile, ios::binary);
    file.write(encrypted.c_str(), encrypted.size());
}

static bool loadRecords() {
    records.clear();
    ifstream file(vaultFile, ios::binary);
    if (!file.is_open()) return false;

    string encrypted((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    string decrypted = decrypt(encrypted, masterKey);

    stringstream ss(decrypted);
    string header;
    getline(ss, header);
    if (header != VAULT_HEADER) {
        cout << "Неверный пароль!" << endl;
        return false;
    }

    Record rec;
    while (getline(ss, rec.service)) {
        if (rec.service.empty()) continue;
        if (!getline(ss, rec.title))    break;
        if (!getline(ss, rec.login))    break;
        if (!getline(ss, rec.password)) break;
        if (!getline(ss, rec.url))      break;
        if (!getline(ss, rec.note))     break;
        records.push_back(rec);
    }
    return true;
}

bool initRecords(const string& vaultName, const string& masterPassword, bool isNew) {
    vaultFile = "vault_" + vaultName + ".dat";
    masterKey = masterPassword;

    if (isNew) {
        ifstream test(vaultFile);
        if (test.is_open()) {
            test.close();
            cout << "Хранилище \"" << vaultName << "\" уже существует. Войдите или выберите другое имя." << endl;
            return false;
        }
        records.clear();
        saveRecords();
        return true;
    }

    return loadRecords();
}

void showAllRecords() {
    loadRecords();
    if (records.empty()) {
        cout << "Записей нет." << endl;
        return;
    }
    cout << "\n=== Список записей ===" << endl;
    for (size_t i = 0; i < records.size(); i++) {
        cout << "[" << i + 1 << "] " << records[i].service;
        if (!records[i].title.empty()) cout << " (" << records[i].title << ")";
        cout << endl;
        cout << "    Логин: "   << records[i].login    << endl;
        cout << "    Пароль: "  << records[i].password << endl;
        cout << "    URL: "     << records[i].url      << endl;
        cout << "    Заметка: " << records[i].note     << endl;
        cout << "------------------------" << endl;
    }
}

void addRecord() {
    loadRecords();
    Record rec;
    cout << "\n=== Добавление записи ===" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Сервис (обязательно): ";
    getline(cin, rec.service);
    while (rec.service.empty()) {
        cout << "Поле не может быть пустым. Сервис: ";
        getline(cin, rec.service);
    }

    cout << "Название: ";    getline(cin, rec.title);
    cout << "Логин: ";       getline(cin, rec.login);
    cout << "Пароль: ";      getline(cin, rec.password);
    cout << "URL: ";         getline(cin, rec.url);
    cout << "Заметка: ";     getline(cin, rec.note);

    for (size_t i = 0; i < records.size(); i++) {
        if (records[i].service == rec.service && records[i].login == rec.login) {
            cout << "Найдена запись с таким же сервисом и логином. Перезаписываем..." << endl;
            records[i] = rec;
            saveRecords();
            cout << "Запись обновлена!" << endl;
            return;
        }
    }

    records.push_back(rec);
    saveRecords();
    cout << "Запись добавлена!" << endl;
}

void editRecord() {
    loadRecords();
    if (records.empty()) {
        cout << "Нет записей." << endl;
        return;
    }
    showAllRecords();
    cout << "Номер записи: ";
    int index = readInt() - 1;
    if (index < 0 || index >= (int)records.size()) {
        cout << "Неверный номер." << endl;
        return;
    }
    Record& rec = records[index];
    cout << "\n=== Редактирование (Enter — оставить текущее) ===" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string tmp;

    cout << "Сервис [" << rec.service << "]: ";
    getline(cin, tmp);
    if (!tmp.empty()) rec.service = tmp;
    while (rec.service.empty()) {
        cout << "Поле не может быть пустым. Сервис: ";
        getline(cin, rec.service);
    }

    cout << "Название [" << rec.title << "]: ";
    getline(cin, tmp); if (!tmp.empty()) rec.title = tmp;

    cout << "Логин [" << rec.login << "]: ";
    getline(cin, tmp); if (!tmp.empty()) rec.login = tmp;

    cout << "Пароль [" << rec.password << "]: ";
    getline(cin, tmp); if (!tmp.empty()) rec.password = tmp;

    cout << "URL [" << rec.url << "]: ";
    getline(cin, tmp); if (!tmp.empty()) rec.url = tmp;

    cout << "Заметка [" << rec.note << "]: ";
    getline(cin, tmp); if (!tmp.empty()) rec.note = tmp;

    saveRecords();
    cout << "Запись обновлена!" << endl;
}

void deleteRecord() {
    loadRecords();
    if (records.empty()) {
        cout << "Нет записей." << endl;
        return;
    }
    showAllRecords();
    cout << "Номер записи для удаления: ";
    int index = readInt() - 1;
    if (index < 0 || index >= (int)records.size()) {
        cout << "Неверный номер." << endl;
        return;
    }
    cout << "Удалить \"" << records[index].service << "\"? (1/0): ";
    if (readInt() == 1) {
        records.erase(records.begin() + index);
        saveRecords();
        cout << "Запись удалена!" << endl;
    } else {
        cout << "Отменено." << endl;
    }
}

void printPassword() {
    loadRecords();
    if (records.empty()) {
        cout << "Нет записей." << endl;
        return;
    }
    showAllRecords();
    cout << "Номер записи: ";
    int index = readInt() - 1;
    if (index < 0 || index >= (int)records.size()) {
        cout << "Неверный номер." << endl;
        return;
    }
    cout << "\nПароль: " << records[index].password << endl;
}