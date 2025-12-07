@echo off
REM Скрипт для сборки DLL библиотеки FlightScheduleAPI
REM Требуется: Visual Studio с x64 Native Tools Command Prompt

echo ============================================
echo Сборка FlightScheduleAPI.dll
echo ============================================

REM Проверяем наличие компилятора
where cl >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ОШИБКА: Компилятор cl не найден!
    echo Пожалуйста, запустите этот скрипт из "x64 Native Tools Command Prompt for VS"
    echo или выполните: "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    pause
    exit /b 1
)

REM Создаём директорию для выходного файла
if not exist "x64\Debug" mkdir x64\Debug

REM Компилируем DLL
echo Компиляция исходных файлов...
cl /LD /EHsc /I. /Iinclude /D FLIGHT_SCHEDULE_EXPORTS /MDd /Zi /Od /W3 /std:c++17 ^
   src\FlightScheduleAPI.cpp ^
   src\Schedule.cpp ^
   src\Flight.cpp ^
   src\Aircraft.cpp ^
   src\Airport.cpp ^
   src\Cargo.cpp ^
   src\UrgentCargo.cpp ^
   src\Passenger.cpp ^
   /Fe:x64\Debug\FlightScheduleAPI.dll ^
   /link /DLL /MACHINE:X64

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ОШИБКА: Компиляция завершилась с ошибками!
    pause
    exit /b 1
)

REM Копируем DLL в python_bindings для удобства
if exist "x64\Debug\FlightScheduleAPI.dll" (
    copy /Y "x64\Debug\FlightScheduleAPI.dll" "python_bindings\FlightScheduleAPI.dll" >nul
    echo.
    echo ============================================
    echo КОМПИЛЯЦИЯ ЗАВЕРШЕНА УСПЕШНО!
    echo DLL создана: x64\Debug\FlightScheduleAPI.dll
    echo DLL скопирована: python_bindings\FlightScheduleAPI.dll
    echo ============================================
) else (
    echo.
    echo ОШИБКА: DLL не была создана!
    pause
    exit /b 1
)

pause

