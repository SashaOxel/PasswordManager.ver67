@echo off
chcp 65001 >nul

echo Сборка тестов...

g++ -std=c++17 -O2 -Wall ^
    tests/test_main.cpp ^
    tests/tests.cpp ^
    src/crypto.cpp ^
    src/generator.cpp ^
    -I include ^
    -o tests/run_tests.exe

if %ERRORLEVEL% NEQ 0 (
    echo Ошибка сборки тестов.
    exit /b 1
)

echo Запуск тестов...
echo.
tests\run_tests.exe
exit /b %ERRORLEVEL%
