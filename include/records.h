#pragma once
#include <string>

struct Record {
    std::string service;
    std::string title;
    std::string login;
    std::string password;
    std::string url;
    std::string note;
};

bool initRecords(const std::string& vaultName, const std::string& masterPassword, bool isNew);
void showAllRecords();
void addRecord();
void editRecord();
void deleteRecord();
void printPassword();