@echo off
REM Сборка автономного EXE для Python GUI через PyInstaller
REM Запускать из корневой папки проекта Cargo_5th_term

setlocal

REM Переходим в директорию скрипта (корень проекта)
cd /d "%~dp0"

echo [1/4] Проверка наличия Python...
python --version >nul 2>&1
if errorlevel 1 (
    echo Ошибка: Python не найден в PATH.
    echo Установите 64-битный Python 3.7+ и добавьте его в PATH.
    goto :end
)

echo [2/4] Создание (или использование) виртуального окружения .venv_deploy...
if not exist ".venv_deploy" (
    python -m venv .venv_deploy
)

call ".venv_deploy\Scripts\activate.bat"
if errorlevel 1 (
    echo Ошибка: не удалось активировать виртуальное окружение.
    goto :end
)

echo [3/4] Установка / обновление PyInstaller...
python -m pip install --upgrade pip >nul
pip install pyinstaller >nul

echo [4/4] Сборка EXE через PyInstaller...
cd "python_bindings"

REM ВАЖНО:
REM Перед запуском этого скрипта скопируйте собранную DLL
REM FlightScheduleAPI.dll в папку python_bindings.
REM Тогда PyInstaller упакует её вместе с EXE.

if not exist "FlightScheduleAPI.dll" (
    echo Ошибка: файл FlightScheduleAPI.dll не найден в папке python_bindings.
    echo Сначала выполните build_dll.bat в корне проекта, затем
    echo скопируйте собранную FlightScheduleAPI.dll сюда.
    goto :end
)

pyinstaller --noconsole --onefile ^
    --name FlightScheduleGUI ^
    --add-binary "FlightScheduleAPI.dll;." ^
    flight_schedule_gui.py

echo.
echo Готово.
echo Исполняемый файл: python_bindings\dist\FlightScheduleGUI.exe
echo Его можно запускать на компьютере без установленного Python.

:end
endlocal
pause

