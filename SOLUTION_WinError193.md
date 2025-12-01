# Решение ошибки WinError 193

## Проблема

```
OSError: [WinError 193] %1 is not a valid Win32 application
```

Эта ошибка означает, что DLL не может быть загружена из-за:
- Несовместимости архитектур (x86 vs x64)
- Отсутствующих зависимостей
- Повреждённой или неполной DLL

## Быстрое решение

### Шаг 1: Диагностика

Запустите скрипт диагностики:

```cmd
cd python_bindings
python check_dll.py
```

Скрипт покажет:
- Архитектуру Python (у вас 64-bit)
- Архитектуру DLL
- Совместимость
- Размер файла
- Результат попытки загрузки

### Шаг 2: Перекомпиляция DLL

Если архитектуры не совпадают или DLL повреждена, перекомпилируйте:

#### Вариант A: Используя скрипт (рекомендуется)

1. Откройте **x64 Native Tools Command Prompt for VS** (важно: x64!)
   - Или обычный Developer Command Prompt, но убедитесь, что выбрана x64 конфигурация

2. Перейдите в директорию проекта:
   ```cmd
   cd "C:\209\Documents 209\МФТИ\5-ый семестр\ООП\Cargo_5th_term"
   ```

3. Запустите сборку:
   ```cmd
   build_dll.bat
   ```

#### Вариант B: Через Visual Studio

1. Откройте Visual Studio
2. Откройте или создайте проект DLL
3. **ВАЖНО:** Выберите конфигурацию **x64**
   - В верхней панели: Debug → x64 (или Release → x64)
4. Убедитесь, что в настройках проекта:
   - Configuration Type = **Dynamic Library (.dll)**
   - Platform = **x64**
   - Preprocessor Definitions содержит **FLIGHT_SCHEDULE_EXPORTS**
5. Соберите проект (Build → Build Solution)

### Шаг 3: Проверка результата

После сборки DLL должна быть в:
```
Cargo_5th_term\x64\Debug\FlightScheduleAPI.dll
```

Скопируйте её в `python_bindings\`:
```cmd
copy x64\Debug\FlightScheduleAPI.dll python_bindings\
```

Или она будет найдена автоматически.

### Шаг 4: Проверка зависимостей

Если DLL всё ещё не загружается, установите **Visual C++ Redistributable**:

- [VC++ Redistributable для VS 2022 (x64)](https://aka.ms/vs/17/release/vc_redist.x64.exe)
- [VC++ Redistributable для VS 2019 (x64)](https://aka.ms/vs/16/release/vc_redist.x64.exe)

### Шаг 5: Запуск GUI

После успешной сборки:

```cmd
cd python_bindings
python flight_schedule_gui.py
```

## Частые вопросы

### Как узнать, какая архитектура у Python?

```cmd
python -c "import platform; print(platform.architecture())"
```

У вас должно быть `('64bit', 'WindowsPE')`

### Как проверить архитектуру DLL?

Используйте скрипт:
```cmd
python python_bindings\check_dll.py
```

Или вручную через dumpbin (если установлен Visual Studio):
```cmd
dumpbin /headers FlightScheduleAPI.dll | findstr machine
```

### DLL всё ещё не работает

1. Убедитесь, что используете **x64 Native Tools Command Prompt**
2. Проверьте, что все исходные файлы на месте
3. Попробуйте полную пересборку (Clean + Rebuild в Visual Studio)
4. Проверьте логи компиляции на ошибки

## Дополнительная помощь

- Подробная диагностика: `python_bindings/check_dll.py`
- Подробное руководство: `FIX_DLL_ISSUES.md`
- Руководство по сборке: `PYTHON_GUI_README.md`

