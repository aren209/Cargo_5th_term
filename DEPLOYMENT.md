## Развёртывание и запуск без установленного Python

Этот файл описывает, как получить **один исполнимый файл `.exe`**, который запускает приложение с расписанием авиаперевозок **на любом Windows‑компьютере (x64)**, даже если там **нет установленного Python и нет ваших библиотек**.

### 1. Разработческая машина (где вы собираете exe)

На вашей машине (где у вас исходники) должны быть:

- 64‑битный **Python 3.7+**, добавленный в `PATH`;
- **Visual Studio 2019/2022 (x64)** для сборки C++ DLL;
- исходный проект `Cargo_5th_term`.

Дальше делаем всё из корня проекта `Cargo_5th_term`.

### 2. Сначала собрать C++ DLL

1. Откройте терминал (PowerShell или CMD) в корне `Cargo_5th_term`.
2. Выполните:
   ```cmd
   build_dll.bat
   ```
3. После успешной сборки у вас появится файл `FlightScheduleAPI.dll` (обычно в `x64\Release` или `x64\Debug`).
4. Скопируйте эту DLL в папку `python_bindings`:
   ```cmd
   copy x64\Release\FlightScheduleAPI.dll python_bindings\
   ```
   (если у вас Debug‑сборка, укажите соответствующую папку).

### 3. Сборка автономного exe через PyInstaller

Для развёртывания используется **PyInstaller** (как рекомендует лекция по развёртыванию).

1. В том же корне `Cargo_5th_term` запустите:
   ```cmd
   build_exe.bat
   ```
2. Скрипт сам:
   - проверит, что Python доступен;
   - создаст (или переиспользует) виртуальное окружение `.venv_deploy`;
   - установит в него `pyinstaller`;
   - перейдёт в `python_bindings` и вызовет:
     ```cmd
     pyinstaller --noconsole --onefile ^
         --name FlightScheduleGUI ^
         --add-binary "FlightScheduleAPI.dll;." ^
         flight_schedule_gui.py
     ```
3. В конце сборки готовый exe будет лежать по пути:
   ```text
   python_bindings\dist\FlightScheduleGUI.exe
   ```

### 4. Запуск на «чужом» компьютере без Python

На целевом компьютере (где ничего не установлено):

1. Скопируйте на него файл:
   ```text
   python_bindings\dist\FlightScheduleGUI.exe
   ```
   (можно в любую папку, например на рабочий стол).
2. Дважды кликните по `FlightScheduleGUI.exe` — откроется графический интерфейс приложения.
3. Никакой установки Python, `pip` и т.п. на этом компьютере **не требуется**: всё уже упаковано внутрь exe PyInstaller‑ом.

### 5. Если exe не запускается (по лекции)

Если при запуске на чужом ПК exe жалуется на отсутствие каких‑то DLL:

- откройте `FlightScheduleGUI.exe` в утилите **Dependencies** (или Dependency Walker);
- посмотрите, какие DLL помечены как *not found*;
- либо:
  - установите соответствующий Microsoft Visual C++ Redistributable, либо
  - положите недостающие DLL рядом с `FlightScheduleGUI.exe`.

В типичном случае при стандартной Windows и актуальном VC++ Redistributable программа запустится без дополнительных шагов.

