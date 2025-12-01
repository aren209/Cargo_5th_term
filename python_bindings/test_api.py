"""
Простой тест для проверки работоспособности Python привязок
"""

import sys
import os

# Добавляем путь к модулям
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

try:
    print("Загрузка библиотеки...")
    from flight_schedule_lib import Schedule, Flight
    from datetime import datetime, timedelta
    
    print("✓ Библиотека загружена успешно!")
    
    # Тест создания расписания
    print("\nТест 1: Создание расписания...")
    schedule = Schedule()
    print("✓ Расписание создано")
    
    # Тест создания рейса
    print("\nТест 2: Создание рейса...")
    now = datetime.now()
    flight = Flight("TEST-001", "Москва", "Санкт-Петербург",
                   now + timedelta(hours=1),
                   now + timedelta(hours=2),
                   "A-001")
    print("✓ Рейс создан")
    print(f"  Номер: {flight.flight_number}")
    print(f"  Маршрут: {flight.departure_airport} -> {flight.destination_airport}")
    
    # Тест добавления рейса
    print("\nТест 3: Добавление рейса в расписание...")
    if schedule.add_flight(flight):
        print("✓ Рейс добавлен")
    else:
        print("✗ Ошибка добавления рейса")
    
    # Тест получения расписания
    print("\nТест 4: Получение расписания...")
    schedule_text = schedule.to_string()
    print("✓ Расписание получено")
    print(f"\nРасписание:\n{schedule_text}")
    
    # Тест валидности
    print("\nТест 5: Проверка валидности...")
    is_valid = schedule.is_valid()
    print(f"✓ Валидность: {is_valid}")
    
    print("\n" + "="*50)
    print("ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!")
    print("="*50)
    
except ImportError as e:
    print(f"\n✗ ОШИБКА: Не удалось загрузить библиотеку: {e}")
    print("\nВозможные причины:")
    print("1. DLL не скомпилирована или не найдена")
    print("2. DLL находится не в той директории")
    print("3. Несовместимость архитектуры (x86 vs x64)")
    sys.exit(1)
    
except Exception as e:
    print(f"\n✗ ОШИБКА: {e}")
    import traceback
    traceback.print_exc()
    sys.exit(1)

