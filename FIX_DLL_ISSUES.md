# Решение проблем с загрузкой DLL

## Ошибка: "not a valid Win32 application" (WinError 193)

Эта ошибка обычно означает одно из следующего:

### 1. Несовместимость архитектур (x86 vs x64)

**Проблема:** DLL собрана для другой архитектуры, чем Python.

**Решение:**

1. Проверьте архитектуру Python:
   ```cmd
   python -c "import platform; print(platform.architecture())"
   ```

2. Убедитесь, что DLL собрана для той же архитектуры (скорее всего нужна x64)

3. Перекомпилируйте DLL:
   ```cmd
   # Откройте Developer Command Prompt for VS
   cd "C:\209\Documents 209\МФТИ\5-ый семестр\ООП\Cargo_5th_term"
   build_dll.bat
   ```

   **Важно:** Убедитесь, что используете x64 конфигурацию в Visual Studio или скрипте сборки.

### 2. Отсутствуют зависимости (Visual C++ Runtime)

**Проблема:** DLL требует Visual C++ Runtime библиотеки, которых нет в системе.

**Решение:**

1. Скачайте и установите **Visual C++ Redistributable**:
   - Для Visual Studio 2022: [VC++ Redistributable x64](https://aka.ms/vs/17/release/vc_redist.x64.exe)
   - Для Visual Studio 2019: [VC++ Redistributable x64](https://aka.ms/vs/16/release/vc_redist.x64.exe)

2. Убедитесь, что установлена версия, соответствующая версии компилятора, которым собрана DLL.

### 3. DLL повреждена или неполная

**Проблема:** Файл DLL повреждён или сборка не завершилась успешно.

**Решение:**

1. Удалите старую DLL
2. Очистите проект в Visual Studio (Build → Clean Solution)
3. Перекомпилируйте заново
4. Проверьте, что файл имеет разумный размер (обычно > 100 KB)

### 4. Неправильная конфигурация сборки

**Проблема:** DLL собрана с неправильными настройками.

**Решение:**

#### Способ 1: Через Visual Studio

1. Откройте проект в Visual Studio
2. В конфигураторе выберите **x64** платформу
3. Убедитесь, что:
   - Configuration Properties → General → Configuration Type = **Dynamic Library (.dll)**
   - C/C++ → Preprocessor → Preprocessor Definitions содержит **FLIGHT_SCHEDULE_EXPORTS**
4. Соберите проект (Build → Build Solution)

#### Способ 2: Через командную строку

1. Откройте **Developer Command Prompt for VS** (x64 Native Tools Command Prompt)
2. Перейдите в директорию проекта
3. Запустите скрипт сборки:
   ```cmd
   build_dll.bat
   ```

## Диагностика

Используйте скрипт для диагностики:

```cmd
cd python_bindings
python check_dll.py
```

Этот скрипт покажет:
- Архитектуру Python и DLL
- Совместимость архитектур
- Размер файла DLL
- Результат попытки загрузки
- Наличие экспортированных функций

## Ручная проверка архитектуры DLL

Если у вас установлен Visual Studio, можете использовать dumpbin:

```cmd
dumpbin /headers FlightScheduleAPI.dll | findstr /i "machine"
```

Ищите:
- `8664 machine (x64)` - для 64-bit
- `14C machine (x86)` - для 32-bit

## Типичные сценарии

### Сценарий 1: Python 64-bit, DLL 32-bit

**Ошибка:** "not a valid Win32 application"

**Решение:** Перекомпилируйте DLL для x64:
```cmd
# В Visual Studio выберите x64 конфигурацию
# Или используйте x64 Native Tools Command Prompt
```

### Сценарий 2: DLL требует VC++ Runtime

**Ошибка:** "The specified module could not be found" или "not a valid Win32 application"

**Решение:** Установите Visual C++ Redistributable для соответствующей версии.

### Сценарий 3: DLL слишком маленькая (< 10 KB)

**Проблема:** Сборка не завершилась успешно или файл повреждён.

**Решение:** 
1. Проверьте логи компиляции на наличие ошибок
2. Пересоберите проект полностью (Clean + Rebuild)

## Быстрая проверка

1. Проверьте Python:
   ```cmd
   python -c "import sys; print('64-bit' if sys.maxsize > 2**32 else '32-bit')"
   ```

2. Проверьте DLL (используя check_dll.py):
   ```cmd
   python python_bindings/check_dll.py
   ```

3. Если архитектуры не совпадают - перекомпилируйте DLL

4. Если DLL не загружается - установите VC++ Redistributable

## Дополнительные ресурсы

- [Microsoft Visual C++ Redistributable](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist)
- [Требования к архитектуре DLL](https://docs.microsoft.com/en-us/cpp/build/configuring-programs-for-64-bit-visual-cpp)

