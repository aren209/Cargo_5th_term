# Руководство по сборке и использованию Python GUI

## Обзор

Данный документ описывает процесс сборки DLL из C++ кода и создания Python графического интерфейса для системы управления расписанием авиаперевозок.

## Структура проекта

```
Cargo_5th_term/
├── FlightScheduleAPI.h          # Заголовочный файл C API
├── FlightScheduleAPI.cpp        # Реализация C API
├── python_bindings/
│   ├── __init__.py
│   ├── flight_schedule_lib.py   # Python привязки (ctypes)
│   └── flight_schedule_gui.py   # Графический интерфейс
├── [остальные C++ файлы]
└── docs/дополнительно/          # Руководства (этот файл и др.)
```

## Требования

### Для сборки C++ DLL:
- Visual Studio 2019 или 2022 (Community, Professional или Enterprise)
- Windows SDK
- Компилятор MSVC (C++17 или выше)

### Для Python GUI:
- Python 3.7 или выше
- tkinter (обычно входит в стандартную установку Python)

## Шаг 1: Сборка DLL

### Вариант A: Использование Visual Studio

1. Откройте Visual Studio
2. Создайте новый проект:
   - File → New → Project
   - Выберите "Dynamic-Link Library (DLL)"
   - Назовите проект `FlightScheduleAPI`
   - Убедитесь, что выбрана платформа x64

3. Добавьте существующие файлы в проект:
   - Правой кнопкой на проект → Add → Existing Item
   - Добавьте все файлы `.cpp` и `.h` из папки Cargo_5th_term:
     - FlightScheduleAPI.cpp
     - Schedule.cpp, Schedule.h
     - Flight.cpp, Flight.h
     - Aircraft.cpp, Aircraft.h
     - Airport.cpp, Airport.h
     - Cargo.cpp, Cargo.h
     - UrgentCargo.cpp, UrgentCargo.h
     - Passenger.cpp, Passenger.h

4. Настройте свойства проекта:
   - Правой кнопкой на проект → Properties
   - Configuration Properties → C/C++ → General
     - Additional Include Directories: добавьте путь к папке с исходниками
   - Configuration Properties → C/C++ → Preprocessor
     - Preprocessor Definitions: добавьте `FLIGHT_SCHEDULE_EXPORTS`

5. Соберите проект:
   - Build → Build Solution (или F7)
   - DLL будет создана в `x64/Debug/FlightScheduleAPI.dll` (или `x64/Release/`)

### Вариант B: Использование командной строки

Создайте файл `build_dll.bat`:

```batch
@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
cl /LD /EHsc /I. /D FLIGHT_SCHEDULE_EXPORTS ^
   FlightScheduleAPI.cpp Schedule.cpp Flight.cpp Aircraft.cpp ^
   Airport.cpp Cargo.cpp UrgentCargo.cpp Passenger.cpp ^
   /Fe:FlightScheduleAPI.dll /link
```

## Шаг 2: Размещение DLL

Скопируйте `FlightScheduleAPI.dll` в одну из следующих директорий:
- `Cargo_5th_term/x64/Debug/` (для Debug сборки)
- `Cargo_5th_term/x64/Release/` (для Release сборки)
- `Cargo_5th_term/python_bindings/` (рядом с Python скриптами)
- Любая директория в PATH

## Шаг 3: Запуск Python GUI

1. Откройте командную строку или PowerShell
2. Перейдите в директорию с Python скриптами:
   ```cmd
   cd Cargo_5th_term\python_bindings
   ```

3. Запустите GUI:
   ```cmd
   python flight_schedule_gui.py
   ```

   Или из корневой директории проекта:
   ```cmd
   python Cargo_5th_term\python_bindings\flight_schedule_gui.py
   ```

## Использование GUI

### Основные функции:

1. **Вкладка "Расписание"**
   - Просмотр всего расписания
   - Проверка валидности расписания
   - Автоматическое исправление ошибок

2. **Вкладка "Рейсы"**
   - Добавление новых рейсов
   - Просмотр списка рейсов
   - Удаление рейсов

3. **Вкладка "Самолёты"**
   - Добавление самолётов с указанием грузоподъёмности

4. **Вкладка "Аэропорты"**
   - Добавление аэропортов

5. **Вкладка "Грузы"**
   - Добавление обычных и срочных грузов
   - Просмотр информации о грузах

6. **Вкладка "Пассажиры"**
   - Добавление пассажиров
   - Просмотр информации о пассажирах

7. **Вкладка "Отчёты"**
   - Получение расписания для конкретного самолёта
   - Просмотр расписания за определённый период
   - Отчёт о просроченных срочных грузах

## Устранение неполадок

### Ошибка "Could not find FlightScheduleAPI.dll"

**Решение:**
- Убедитесь, что DLL скомпилирована и находится в нужной директории
- Проверьте, что путь к DLL указан правильно в `flight_schedule_lib.py`
- Можно скопировать DLL в директорию с Python скриптами

### Ошибка "Failed to load DLL"

**Решение:**
- Убедитесь, что используется 64-битная версия Python (если DLL собрана для x64)
- Проверьте, что все необходимые зависимости доступны
- Попробуйте перекомпилировать DLL в режиме Release

### Ошибки кодировки (кириллица отображается неправильно)

**Решение:**
- Убедитесь, что консоль использует UTF-8:
  ```cmd
  chcp 65001
  ```
- В Python скриптах используется UTF-8 по умолчанию

### Ошибки компиляции C++

**Решение:**
- Убедитесь, что все необходимые заголовочные файлы включены
- Проверьте, что используется стандарт C++17 или выше
- Убедитесь, что макрос `FLIGHT_SCHEDULE_EXPORTS` определён при компиляции

## Устранение проблем

При ошибках загрузки DLL (архитектура x86/x64, WinError 193) см. [УСТРАНЕНИЕ_ПРОБЛЕМ.md](../УСТРАНЕНИЕ_ПРОБЛЕМ.md).

## Дополнительная информация

### API функций

Все функции C API определены в `FlightScheduleAPI.h`. Основные группы:

- **Schedule API**: работа с расписанием рейсов
- **Flight API**: управление отдельными рейсами
- **Aircraft API**: управление самолётами
- **Airport API**: управление аэропортами
- **Cargo API**: управление грузами
- **UrgentCargo API**: управление срочными грузами
- **Passenger API**: управление пассажирами

### Python классы

Python модуль `flight_schedule_lib.py` предоставляет Python-классы:
- `Schedule` - расписание рейсов
- `Flight` - рейс
- `Aircraft` - самолёт
- `Airport` - аэропорт
- `Cargo` - груз
- `UrgentCargo` - срочный груз
- `Passenger` - пассажир

### Пример использования Python API

```python
from flight_schedule_lib import Schedule, Flight
from datetime import datetime, timedelta

# Создаём расписание
schedule = Schedule()

# Создаём рейс
now = datetime.now()
flight = Flight("FL-001", "Шереметьево", "Домодедово",
                now + timedelta(hours=1),
                now + timedelta(hours=2),
                "A-001")

# Добавляем рейс в расписание
schedule.add_flight(flight)

# Получаем расписание
print(schedule.to_string())
```

## Лицензия

Этот проект является частью учебного задания по ООП.

