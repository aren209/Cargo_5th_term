@echo off
chcp 65001 >nul
echo Генерация документации C++ (Doxygen)...
where doxygen >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Doxygen не найден в PATH. Установите: https://www.doxygen.nl/download.html
    echo Либо запустите: "C:\Program Files\doxygen\bin\doxygen.exe" Doxyfile
    exit /b 1
)
doxygen Doxyfile
if %ERRORLEVEL% equ 0 (
    echo Готово. Откройте docs\cpp\html\index.html
) else (
    echo Ошибка генерации.
    exit /b 1
)
