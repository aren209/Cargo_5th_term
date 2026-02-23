# Изменения в проекте

## Выполненные исправления

### 1. Упорядочивание структуры проекта

Файлы проекта были реорганизованы по директориям:
- `include/` - все заголовочные файлы (.h)
- `src/` - все исходные файлы (.cpp)
- `test/` - тестовые файлы (test_*.cpp)

### 2. Создан файл build_dll.bat

Создан скрипт `build_dll.bat` для автоматической сборки DLL библиотеки. Скрипт:
- Проверяет наличие компилятора Visual Studio
- Компилирует все исходные файлы с правильными путями включения
- Создаёт DLL в директории `x64\Debug\`
- Копирует DLL в `python_bindings\` для удобства

### 3. Добавлена обработка исключений

#### Создан класс исключений

Создан файл `include/FlightScheduleException.h` с классами:
- `FlightScheduleException` - базовый класс исключений системы
- `InvalidAircraftException` - исключение для некорректных данных самолёта

#### Добавлена проверка грузоподъёмности самолёта

В класс `Aircraft` добавлена проверка на отрицательную или нулевую грузоподъёмность:
- В конструкторе `Aircraft(const std::string& number, double maxPayload)`
- В методе `setMaxPayload(double maxPayload)`

При попытке создать самолёт с некорректной грузоподъёмностью выбрасывается исключение `InvalidAircraftException` с понятным сообщением об ошибке.

#### Обновлена обработка исключений в API

В функции `Aircraft_Create` в `FlightScheduleAPI.cpp` заменён `catch(...)` на конкретные типы исключений:
- `catch (const InvalidAircraftException& e)` - для некорректной грузоподъёмности
- `catch (const FlightScheduleException& e)` - для других исключений системы
- `catch (...)` - для всех остальных исключений (на случай непредвиденных ошибок)

### 4. Создан тест для проверки обработки исключений

Создан файл `test/test_exception.cpp` с тестами:
- Создание самолёта с отрицательной грузоподъёмностью
- Создание самолёта с нулевой грузоподъёмностью
- Установка отрицательной грузоподъёмности через `setMaxPayload`
- Создание самолёта с корректной грузоподъёмностью (не должно выбрасывать исключение)

## Использование

### Сборка DLL

Запустите скрипт сборки из "x64 Native Tools Command Prompt for VS":
```cmd
build_dll.bat
```

### Запуск тестов

Для компиляции и запуска теста обработки исключений:
```cmd
cl /EHsc /I. /Iinclude /std:c++17 test\test_exception.cpp src\Aircraft.cpp /Fe:test_exception.exe /link
test_exception.exe
```

## Структура проекта

```
Cargo_5th_term/
├── include/          # Заголовочные файлы
│   ├── Aircraft.h
│   ├── Airport.h
│   ├── Cargo.h
│   ├── Flight.h
│   ├── FlightScheduleAPI.h
│   ├── FlightScheduleException.h  # НОВЫЙ
│   ├── Passenger.h
│   ├── Schedule.h
│   └── UrgentCargo.h
├── src/              # Исходные файлы
│   ├── Aircraft.cpp
│   ├── Airport.cpp
│   ├── Cargo.cpp
│   ├── Flight.cpp
│   ├── FlightScheduleAPI.cpp
│   ├── main.cpp
│   ├── Passenger.cpp
│   ├── Schedule.cpp
│   └── UrgentCargo.cpp
├── test/             # Тестовые файлы
│   ├── test_cargo.cpp
│   ├── test_exception.cpp  # НОВЫЙ
│   └── test_hierarchy.cpp
├── build_dll.bat     # НОВЫЙ - скрипт сборки DLL
└── python_bindings/  # Python интерфейс
```

