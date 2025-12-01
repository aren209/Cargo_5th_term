"""
Скрипт для диагностики проблем с DLL
"""

import os
import sys
import platform
import ctypes

def check_dll_architecture(dll_path):
    """Проверяет архитектуру DLL (x86 или x64)"""
    try:
        with open(dll_path, 'rb') as f:
            f.seek(0x3C)  # PE header offset
            pe_offset = int.from_bytes(f.read(4), 'little')
            f.seek(pe_offset + 4)  # Skip PE signature
            machine = int.from_bytes(f.read(2), 'little')
            # 0x8664 = x64, 0x14C = x86
            if machine == 0x8664:
                return 'x64'
            elif machine == 0x14C:
                return 'x86'
            else:
                return f'unknown (0x{machine:X})'
    except Exception as e:
        return f'error: {e}'

def check_dll_dependencies(dll_path):
    """Проверяет зависимости DLL (требует dumpbin или другой инструмент)"""
    try:
        # Попытка загрузить через ctypes для проверки зависимостей
        lib = ctypes.WinDLL(dll_path)
        return "DLL loaded successfully"
    except OSError as e:
        return f"Failed to load: {e}"
    except Exception as e:
        return f"Error: {e}"

def main():
    dll_name = 'FlightScheduleAPI.dll'
    
    print("=" * 60)
    print("Диагностика FlightScheduleAPI.dll")
    print("=" * 60)
    print()
    
    # Информация о Python
    print("Информация о Python:")
    print(f"  Версия: {sys.version}")
    print(f"  Архитектура: {platform.architecture()[0]}")
    print(f"  Платформа: {platform.machine()}")
    print(f"  64-bit: {sys.maxsize > 2**32}")
    print()
    
    # Поиск DLL
    possible_paths = [
        os.path.join(os.path.dirname(__file__), '..', 'x64', 'Debug', dll_name),
        os.path.join(os.path.dirname(__file__), '..', 'x64', 'Release', dll_name),
        os.path.join(os.path.dirname(__file__), dll_name),
        dll_name
    ]
    
    print("Поиск DLL:")
    found_paths = []
    for path in possible_paths:
        abs_path = os.path.abspath(path)
        exists = os.path.exists(abs_path)
        status = "✓ НАЙДЕНА" if exists else "✗ не найдена"
        print(f"  {status}: {abs_path}")
        if exists:
            found_paths.append(abs_path)
    
    print()
    
    if not found_paths:
        print("ОШИБКА: DLL не найдена ни в одном из указанных мест!")
        print("\nРешение:")
        print("1. Скомпилируйте DLL используя build_dll.bat или Visual Studio")
        print("2. Убедитесь, что DLL находится в одной из указанных директорий")
        return 1
    
    # Проверка первой найденной DLL
    dll_path = found_paths[0]
    print(f"Проверка DLL: {dll_path}")
    print()
    
    # Размер файла
    size = os.path.getsize(dll_path)
    print(f"Размер файла: {size:,} байт ({size / 1024:.2f} KB)")
    if size < 1024:
        print("  ⚠ ПРЕДУПРЕЖДЕНИЕ: Файл слишком мал, возможно повреждён!")
    print()
    
    # Архитектура DLL
    dll_arch = check_dll_architecture(dll_path)
    python_arch = platform.architecture()[0]
    print(f"Архитектура DLL: {dll_arch}")
    print(f"Архитектура Python: {python_arch}")
    
    if dll_arch == 'x86' and python_arch == '64bit':
        print("  ✗ НЕСОВМЕСТИМОСТЬ: DLL 32-bit, Python 64-bit")
        print("\nРешение: Перекомпилируйте DLL для x64 платформы")
        return 1
    elif dll_arch == 'x64' and python_arch != '64bit':
        print("  ✗ НЕСОВМЕСТИМОСТЬ: DLL 64-bit, Python 32-bit")
        print("\nРешение: Используйте 64-bit Python или перекомпилируйте DLL для x86")
        return 1
    elif dll_arch.startswith('unknown'):
        print(f"  ⚠ ПРЕДУПРЕЖДЕНИЕ: Не удалось определить архитектуру ({dll_arch})")
    else:
        print("  ✓ Архитектуры совместимы")
    print()
    
    # Попытка загрузки
    print("Попытка загрузки DLL...")
    try:
        lib = ctypes.CDLL(dll_path)
        print("  ✓ DLL успешно загружена!")
        
        # Проверяем наличие основных функций
        print("\nПроверка экспортированных функций...")
        try:
            lib.Schedule_Create
            print("  ✓ Schedule_Create найдена")
        except AttributeError:
            print("  ✗ Schedule_Create не найдена")
        
        try:
            lib.Flight_Create
            print("  ✓ Flight_Create найдена")
        except AttributeError:
            print("  ✗ Flight_Create не найдена")
            
    except OSError as e:
        error_code = getattr(e, 'winerror', None)
        print(f"  ✗ Ошибка загрузки: {e}")
        if error_code == 193:
            print("\n  Это означает, что DLL не может быть загружена.")
            print("  Возможные причины:")
            print("    1. Несовместимость архитектур (x86 vs x64)")
            print("    2. Отсутствуют зависимости (Visual C++ Runtime)")
            print("    3. DLL повреждена или неполная")
            print("\n  Решение:")
            print("    1. Перекомпилируйте DLL используя build_dll.bat")
            print("    2. Установите Visual C++ Redistributable для вашей версии")
            print("    3. Убедитесь, что используется правильная платформа (x64 для 64-bit Python)")
        return 1
    except Exception as e:
        print(f"  ✗ Неожиданная ошибка: {e}")
        return 1
    
    print()
    print("=" * 60)
    print("Диагностика завершена успешно!")
    print("=" * 60)
    return 0

if __name__ == '__main__':
    sys.exit(main())

