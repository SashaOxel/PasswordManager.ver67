#include "../include/crypto.h"
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

string encrypt(const string& data, const string& key) {
    if (key.empty()) return data;
    string result(data.size(), 0);
    unsigned char running = 0;
    for (size_t i = 0; i < data.size(); i++) {
        result[i] = (char)((unsigned char)data[i] ^ (unsigned char)key[i % key.size()] ^ running);
        running = (unsigned char)result[i];
    }
    return result;
}

string decrypt(const string& data, const string& key) {
    if (key.empty()) return data;
    string result(data.size(), 0);
    unsigned char running = 0;
    for (size_t i = 0; i < data.size(); i++) {
        result[i] = (char)((unsigned char)data[i] ^ (unsigned char)key[i % key.size()] ^ running);
        running = (unsigned char)data[i];
    }
    return result;
}

void encryptFile(const string& path, const string& key) {
    ifstream in(path, ios::binary);
    if (!in.is_open()) {
        cout << "Ошибка: файл не найден." << endl;
        return;
    }
    string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    string result = encrypt(data, key);
    string outPath = path + ".enc";
    ofstream out(outPath, ios::binary);
    out.write(result.c_str(), result.size());
    cout << "Зашифровано -> " << outPath << endl;
}

void decryptFile(const string& path, const string& key) {
    ifstream in(path, ios::binary);
    if (!in.is_open()) {
        cout << "Ошибка: файл не найден." << endl;
        return;
    }
    string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    string result = decrypt(data, key);
    string outPath = path;
    if (outPath.size() > 4 && outPath.substr(outPath.size() - 4) == ".enc")
        outPath = outPath.substr(0, outPath.size() - 4);
    else
        outPath += ".dec";
    ofstream out(outPath, ios::binary);
    out.write(result.c_str(), result.size());
    cout << "Расшифровано -> " << outPath << endl;
}