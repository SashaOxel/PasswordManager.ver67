#pragma once
#include <string>

std::string encrypt(const std::string& data, const std::string& key);
std::string decrypt(const std::string& data, const std::string& key);
void encryptFile(const std::string& path, const std::string& key);
void decryptFile(const std::string& path, const std::string& key);