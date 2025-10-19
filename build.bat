@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
msbuild Cargo_5th_term.sln /p:Configuration=Debug /p:Platform=x64
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ================================================
    echo КОМПИЛЯЦИЯ ЗАВЕРШЕНА УСПЕШНО!
    echo ================================================
    echo.
    echo Запуск программы...
    echo.
    x64\Debug\Cargo_5th_term.exe
) else (
    echo.
    echo ================================================
    echo ОШИБКИ КОМПИЛЯЦИИ!
    echo ================================================
)
pause
