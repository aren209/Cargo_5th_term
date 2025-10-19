# PowerShell скрипт для компиляции проекта
Write-Host "Запуск компиляции проекта Cargo_5th_term..." -ForegroundColor Green

# Находим Visual Studio
$vsPath = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
if (Test-Path $vsPath) {
    Write-Host "Найден Visual Studio 2022 Community" -ForegroundColor Yellow
    
    # Запускаем компиляцию через cmd
    $cmd = "call `"$vsPath`" && msbuild Cargo_5th_term.sln /p:Configuration=Debug /p:Platform=x64"
    $result = cmd /c $cmd
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "`n================================================" -ForegroundColor Green
        Write-Host "КОМПИЛЯЦИЯ ЗАВЕРШЕНА УСПЕШНО!" -ForegroundColor Green
        Write-Host "================================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "Запуск программы..." -ForegroundColor Yellow
        Write-Host ""
        
        # Запускаем исполняемый файл
        if (Test-Path "x64\Debug\Cargo_5th_term.exe") {
            & "x64\Debug\Cargo_5th_term.exe"
        } else {
            Write-Host "Исполняемый файл не найден!" -ForegroundColor Red
        }
    } else {
        Write-Host "`n================================================" -ForegroundColor Red
        Write-Host "ОШИБКИ КОМПИЛЯЦИИ!" -ForegroundColor Red
        Write-Host "================================================" -ForegroundColor Red
    }
} else {
    Write-Host "Visual Studio 2022 Community не найден!" -ForegroundColor Red
}

Write-Host "`nНажмите любую клавишу для выхода..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")


