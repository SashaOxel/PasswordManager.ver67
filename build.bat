@echo off
chcp 65001 >nul

echo Сборка PasswordManager...

g++ -std=c++17 -O2 -Wall ^
    main.cpp ^
    src/crypto.cpp ^
    src/generator.cpp ^
    src/menu.cpp ^
    src/records.cpp ^
    -I include ^
    -o PasswordManager.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Сборка успешна! Запустите: PasswordManager.exe
) else (
    echo.
    echo Ошибка сборки. Убедитесь, что g++ установлен и доступен в PATH.
)
