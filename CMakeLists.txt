cmake_minimum_required(VERSION 3.10)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 17)

add_executable(PasswordManager
    main.cpp
    src/menu.cpp
    src/records.cpp
    src/generator.cpp
    src/crypto.cpp
)

target_include_directories(PasswordManager PRIVATE include)