# Инструкция: Пересборка DLL для x64

## Проблема

```
RuntimeError: Architecture mismatch: DLL is 32-bit (x86) but Python is 64-bit.
```

Ваша DLL собрана для 32-bit, но нужна для 64-bit.

## Решение: 3 способа

### Способ 1: Использование x64 Native Tools Command Prompt (РЕКОМЕНДУЕТСЯ)

1. **Откройте меню Пуск** и найдите:
   - `x64 Native Tools Command Prompt for VS 2022` (или 2019)
   - Или: `Developer Command Prompt for VS 2022` → затем выберите x64

2. В открывшемся окне командной строки перейдите в директорию проекта:
   ```cmd
   cd "C:\209\Documents 209\МФТИ\5-ый семестр\ООП\Cargo_5th_term"
   ```

3. Запустите скрипт сборки:
   ```cmd
   build_dll.bat
   ```

4. Дождитесь завершения сборки. Вы должны увидеть:
   ```
   КОМПИЛЯЦИЯ ЗАВЕРШЕНА УСПЕШНО!
   DLL создана: x64\Debug\FlightScheduleAPI.dll
   ```

5. Проверьте архитектуру:
   ```cmd
   cd python_bindings
   python check_dll.py
   ```
   
   Должно показать: `✓ Архитектуры совместимы`

### Способ 2: Через Visual Studio (если у вас есть проект)

1. Откройте Visual Studio
2. Откройте решение или создайте новый проект DLL
3. **ВАЖНО:** В верхней панели выберите:
   - Configuration: `Debug` (или `Release`)
   - Platform: `x64` (не Win32!)

4. Убедитесь в настройках проекта:
   - Правой кнопкой на проект → Properties
   - Configuration Properties → General:
     - Configuration Type = **Dynamic Library (.dll)**
   - Configuration Properties → C/C++ → Preprocessor:
     - Preprocessor Definitions = добавить `FLIGHT_SCHEDULE_EXPORTS`

5. Добавьте все исходные файлы в проект:
   - FlightScheduleAPI.cpp
   - Schedule.cpp, Flight.cpp, Aircraft.cpp
   - Airport.cpp, Cargo.cpp, UrgentCargo.cpp, Passenger.cpp
   - И все соответствующие .h файлы

6. Build → Build Solution (или F7)

7. DLL будет создана в `x64\Debug\FlightScheduleAPI.dll`

### Способ 3: Ручная компиляция через командную строку

1. Откройте **x64 Native Tools Command Prompt for VS 2022**

2. Перейдите в директорию проекта:
   ```cmd
   cd "C:\209\Documents 209\МФТИ\5-ый семестр\ООП\Cargo_5th_term"
   ```

3. Создайте директорию для выходного файла:
   ```cmd
   if not exist "x64\Debug" mkdir x64\Debug
   ```

4. Скомпилируйте DLL:
   ```cmd
   cl /LD /EHsc /I. /D FLIGHT_SCHEDULE_EXPORTS /MDd /Zi /Od /W3 /std:c++17 FlightScheduleAPI.cpp Schedule.cpp Flight.cpp Aircraft.cpp Airport.cpp Cargo.cpp UrgentCargo.cpp Passenger.cpp /Fe:x64\Debug\FlightScheduleAPI.dll /link /DLL /MACHINE:X64
   ```

## После сборки

1. **Проверьте результат:**
   ```cmd
   python python_bindings\check_dll.py
   ```

2. **Если всё хорошо, запустите GUI:**
   ```cmd
   cd python_bindings
   python flight_schedule_gui.py
   ```

## Устранение проблем

### "cl is not recognized"

**Проблема:** Не инициализировано окружение Visual Studio.

**Решение:** Используйте **x64 Native Tools Command Prompt** вместо обычной командной строки.

### "Cannot open include file"

**Проблема:** Не найдены заголовочные файлы.

**Решение:** Убедитесь, что вы находитесь в директории `Cargo_5th_term` и все .h файлы там есть.

### DLL всё ещё 32-bit

**Проблема:** Скрипт всё ещё использует x86 окружение.

**Решение:** 
1. Удалите старую DLL
2. Убедитесь, что используете **x64 Native Tools Command Prompt**
3. Пересоберите заново

### "LINK : fatal error LNK1120"

**Проблема:** Ошибки линковки, возможно отсутствуют некоторые объектные файлы.

**Решение:** Убедитесь, что все .cpp файлы включены в команду компиляции.

## Проверка успешности

После сборки проверьте:

```cmd
python python_bindings\check_dll.py
```

Ожидаемый результат:
```
Архитектура DLL: x64
Архитектура Python: 64bit
  ✓ Архитектуры совместимы
  ✓ DLL успешно загружена!
```

Если видите это - всё готово! Можете запускать GUI.

