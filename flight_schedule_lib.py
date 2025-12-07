"""
Python bindings for Flight Schedule C++ library using ctypes.
Provides a Python interface to the C++ flight schedule system.
"""

import ctypes
import os
import sys
import platform
from datetime import datetime
from typing import Optional, List, Tuple

# Определяем путь к DLL в зависимости от платформы
if sys.platform == 'win32':
    DLL_NAME = 'FlightScheduleAPI.dll'
else:
    DLL_NAME = 'libFlightScheduleAPI.so'

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
                return 'unknown'
    except Exception:
        return 'unknown'

# Попытка загрузить DLL из разных мест
_dll_path = None
possible_paths = [
    os.path.join(os.path.dirname(__file__), '..', 'x64', 'Debug', DLL_NAME),
    os.path.join(os.path.dirname(__file__), '..', 'x64', 'Release', DLL_NAME),
    os.path.join(os.path.dirname(__file__), DLL_NAME),
    DLL_NAME
]

for path in possible_paths:
    abs_path = os.path.abspath(path)
    if os.path.exists(abs_path):
        _dll_path = abs_path
        break

if not _dll_path:
    error_msg = f"Could not find {DLL_NAME}. Please build the C++ library first.\n"
    error_msg += f"Searched in:\n"
    for path in possible_paths:
        abs_path = os.path.abspath(path)
        error_msg += f"  - {abs_path}\n"
    raise FileNotFoundError(error_msg)

# Проверяем архитектуру Python
python_arch = platform.architecture()[0]
is_64bit_python = sys.maxsize > 2**32 or python_arch == '64bit'

# Проверяем архитектуру DLL
dll_arch = check_dll_architecture(_dll_path)
if dll_arch == 'x86' and is_64bit_python:
    raise RuntimeError(
        f"Architecture mismatch: DLL is 32-bit (x86) but Python is 64-bit.\n"
        f"DLL path: {_dll_path}\n"
        f"Please rebuild the DLL for x64 platform."
    )
elif dll_arch == 'x64' and not is_64bit_python:
    raise RuntimeError(
        f"Architecture mismatch: DLL is 64-bit (x64) but Python is 32-bit.\n"
        f"DLL path: {_dll_path}\n"
        f"Please use 64-bit Python or rebuild the DLL for x86 platform."
    )

# Загружаем DLL
try:
    _lib = ctypes.CDLL(_dll_path)
except OSError as e:
    error_code = e.winerror if hasattr(e, 'winerror') else None
    error_msg = f"Failed to load {DLL_NAME} from {_dll_path}\n"
    error_msg += f"Error: {e}\n"
    
    if error_code == 193:  # %1 is not a valid Win32 application
        error_msg += "\nThis usually means:\n"
        error_msg += "1. DLL architecture doesn't match Python (x86 vs x64)\n"
        error_msg += "2. DLL is corrupted or incomplete\n"
        error_msg += "3. DLL requires dependencies that are missing\n"
        error_msg += f"\nDLL architecture: {dll_arch}\n"
        error_msg += f"Python architecture: {python_arch}\n"
        
        # Проверяем размер файла
        if os.path.exists(_dll_path):
            size = os.path.getsize(_dll_path)
            error_msg += f"DLL size: {size} bytes\n"
            if size < 1024:  # Очень маленький файл
                error_msg += "WARNING: DLL file seems too small. It might be incomplete.\n"
    
    raise RuntimeError(error_msg)
except Exception as e:
    raise RuntimeError(f"Failed to load {DLL_NAME} from {_dll_path}: {e}")

# Определяем типы
Handle = ctypes.c_void_p
c_char_p = ctypes.c_char_p
c_int = ctypes.c_int
c_double = ctypes.c_double
c_longlong = ctypes.c_longlong

BUFFER_SIZE = 8192

# ============================================
# Schedule API
# ============================================

_lib.Schedule_Create.restype = Handle
_lib.Schedule_Create.argtypes = []

_lib.Schedule_Destroy.restype = None
_lib.Schedule_Destroy.argtypes = [Handle]

_lib.Schedule_AddFlight.restype = c_int
_lib.Schedule_AddFlight.argtypes = [Handle, Handle]

_lib.Schedule_RemoveFlight.restype = c_int
_lib.Schedule_RemoveFlight.argtypes = [Handle, c_char_p]

_lib.Schedule_FindFlight.restype = Handle
_lib.Schedule_FindFlight.argtypes = [Handle, c_char_p]

_lib.Schedule_IsValid.restype = c_int
_lib.Schedule_IsValid.argtypes = [Handle]

_lib.Schedule_GetValidationErrors.restype = None
_lib.Schedule_GetValidationErrors.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Schedule_ValidateAndFix.restype = None
_lib.Schedule_ValidateAndFix.argtypes = [Handle]

_lib.Schedule_ToString.restype = None
_lib.Schedule_ToString.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Schedule_GetScheduleForAircraft.restype = None
_lib.Schedule_GetScheduleForAircraft.argtypes = [Handle, c_char_p, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Schedule_GetScheduleForAircraftInRange.restype = None
_lib.Schedule_GetScheduleForAircraftInRange.argtypes = [Handle, c_char_p, c_longlong, c_longlong, 
                                                        ctypes.POINTER(ctypes.c_char), c_int]

_lib.Schedule_GetOverdueUrgentCargoReport.restype = None
_lib.Schedule_GetOverdueUrgentCargoReport.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Schedule_GetTotalFlightTime.restype = c_double
_lib.Schedule_GetTotalFlightTime.argtypes = [Handle, c_char_p]

_lib.Schedule_GetTotalFlights.restype = c_int
_lib.Schedule_GetTotalFlights.argtypes = [Handle]

# ============================================
# Flight API
# ============================================

_lib.Flight_Create.restype = Handle
_lib.Flight_Create.argtypes = [c_char_p, c_char_p, c_char_p, c_longlong, c_longlong, c_char_p]

_lib.Flight_Destroy.restype = None
_lib.Flight_Destroy.argtypes = [Handle]

_lib.Flight_GetFlightNumber.restype = None
_lib.Flight_GetFlightNumber.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Flight_GetDepartureAirport.restype = None
_lib.Flight_GetDepartureAirport.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Flight_GetDestinationAirport.restype = None
_lib.Flight_GetDestinationAirport.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Flight_GetDepartureTime.restype = c_longlong
_lib.Flight_GetDepartureTime.argtypes = [Handle]

_lib.Flight_GetArrivalTime.restype = c_longlong
_lib.Flight_GetArrivalTime.argtypes = [Handle]

_lib.Flight_GetAircraftId.restype = None
_lib.Flight_GetAircraftId.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Flight_IsCompleted.restype = c_int
_lib.Flight_IsCompleted.argtypes = [Handle]

_lib.Flight_ToString.restype = None
_lib.Flight_ToString.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Flight_IsValid.restype = c_int
_lib.Flight_IsValid.argtypes = [Handle]

_lib.Flight_GetFlightDurationHours.restype = c_double
_lib.Flight_GetFlightDurationHours.argtypes = [Handle]

# ============================================
# Aircraft API
# ============================================

_lib.Aircraft_Create.restype = Handle
_lib.Aircraft_Create.argtypes = [c_char_p, c_double]

_lib.Aircraft_Destroy.restype = None
_lib.Aircraft_Destroy.argtypes = [Handle]

_lib.Aircraft_GetAircraftNumber.restype = None
_lib.Aircraft_GetAircraftNumber.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Aircraft_GetMaxPayload.restype = c_double
_lib.Aircraft_GetMaxPayload.argtypes = [Handle]

_lib.Aircraft_GetCurrentPayload.restype = c_double
_lib.Aircraft_GetCurrentPayload.argtypes = [Handle]

_lib.Aircraft_GetAvailableCapacity.restype = c_double
_lib.Aircraft_GetAvailableCapacity.argtypes = [Handle]

_lib.Aircraft_ToString.restype = None
_lib.Aircraft_ToString.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

# ============================================
# Airport API
# ============================================

_lib.Airport_Create.restype = Handle
_lib.Airport_Create.argtypes = [c_char_p]

_lib.Airport_Destroy.restype = None
_lib.Airport_Destroy.argtypes = [Handle]

_lib.Airport_GetName.restype = None
_lib.Airport_GetName.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Airport_ToString.restype = None
_lib.Airport_ToString.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

# ============================================
# Cargo API
# ============================================

_lib.Cargo_Create.restype = Handle
_lib.Cargo_Create.argtypes = [c_char_p, c_double, c_char_p, c_char_p, c_char_p, c_longlong]

_lib.Cargo_Destroy.restype = None
_lib.Cargo_Destroy.argtypes = [Handle]

_lib.Cargo_GetCargoNumber.restype = None
_lib.Cargo_GetCargoNumber.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Cargo_GetMass.restype = c_double
_lib.Cargo_GetMass.argtypes = [Handle]

_lib.Cargo_ToString.restype = None
_lib.Cargo_ToString.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

# ============================================
# UrgentCargo API
# ============================================

_lib.UrgentCargo_Create.restype = Handle
_lib.UrgentCargo_Create.argtypes = [c_char_p, c_double, c_char_p, c_char_p, c_char_p, c_longlong, c_longlong]

_lib.UrgentCargo_Destroy.restype = None
_lib.UrgentCargo_Destroy.argtypes = [Handle]

_lib.UrgentCargo_IsOverdue.restype = c_int
_lib.UrgentCargo_IsOverdue.argtypes = [Handle]

_lib.UrgentCargo_ToString.restype = None
_lib.UrgentCargo_ToString.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

# ============================================
# Passenger API
# ============================================

_lib.Passenger_Create.restype = Handle
_lib.Passenger_Create.argtypes = [c_char_p, c_char_p, c_char_p, c_char_p]

_lib.Passenger_Destroy.restype = None
_lib.Passenger_Destroy.argtypes = [Handle]

_lib.Passenger_GetPassengerNumber.restype = None
_lib.Passenger_GetPassengerNumber.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Passenger_GetName.restype = None
_lib.Passenger_GetName.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]

_lib.Passenger_ToString.restype = None
_lib.Passenger_ToString.argtypes = [Handle, ctypes.POINTER(ctypes.c_char), c_int]


def _get_string(func, handle, *args):
    """Вспомогательная функция для получения строк из DLL"""
    buffer = ctypes.create_string_buffer(BUFFER_SIZE)
    func(handle, *args, buffer, BUFFER_SIZE)
    result = buffer.value.decode('utf-8', errors='ignore')
    return result.rstrip('\x00')


def _to_bytes(s: str) -> bytes:
    """Преобразует строку в bytes для передачи в C API"""
    return s.encode('utf-8') if isinstance(s, str) else s


def datetime_to_timestamp(dt: datetime) -> int:
    """Преобразует datetime в Unix timestamp"""
    return int(dt.timestamp())


def timestamp_to_datetime(ts: int) -> datetime:
    """Преобразует Unix timestamp в datetime"""
    return datetime.fromtimestamp(ts)


# ============================================
# Python классы-обёртки
# ============================================

class Schedule:
    """Python класс для работы с расписанием рейсов"""
    
    def __init__(self):
        self._handle = _lib.Schedule_Create()
        if not self._handle:
            raise RuntimeError("Failed to create Schedule")
    
    def __del__(self):
        if hasattr(self, '_handle') and self._handle:
            _lib.Schedule_Destroy(self._handle)
    
    def add_flight(self, flight: 'Flight') -> bool:
        """Добавить рейс в расписание"""
        return _lib.Schedule_AddFlight(self._handle, flight._handle) != 0
    
    def remove_flight(self, flight_number: str) -> bool:
        """Удалить рейс из расписания"""
        return _lib.Schedule_RemoveFlight(self._handle, _to_bytes(flight_number)) != 0
    
    def find_flight(self, flight_number: str) -> Optional['Flight']:
        """Найти рейс по номеру"""
        handle = _lib.Schedule_FindFlight(self._handle, _to_bytes(flight_number))
        if handle:
            return Flight._from_handle(handle)
        return None
    
    def is_valid(self) -> bool:
        """Проверить корректность расписания"""
        return _lib.Schedule_IsValid(self._handle) != 0
    
    def get_validation_errors(self) -> List[str]:
        """Получить список ошибок валидации"""
        errors_str = _get_string(_lib.Schedule_GetValidationErrors, self._handle)
        return [e for e in errors_str.split('\n') if e.strip()]
    
    def validate_and_fix(self):
        """Проверить и исправить ошибки в расписании"""
        _lib.Schedule_ValidateAndFix(self._handle)
    
    def to_string(self) -> str:
        """Получить строковое представление расписания"""
        return _get_string(_lib.Schedule_ToString, self._handle)
    
    def get_schedule_for_aircraft(self, aircraft_id: str) -> str:
        """Получить расписание для самолёта"""
        return _get_string(_lib.Schedule_GetScheduleForAircraft, self._handle, _to_bytes(aircraft_id))
    
    def get_schedule_for_aircraft_in_range(self, aircraft_id: str, 
                                           start_time: datetime, end_time: datetime) -> str:
        """Получить расписание самолёта в диапазоне времени"""
        start_ts = datetime_to_timestamp(start_time)
        end_ts = datetime_to_timestamp(end_time)
        return _get_string(_lib.Schedule_GetScheduleForAircraftInRange, 
                          self._handle, _to_bytes(aircraft_id), start_ts, end_ts)
    
    def get_overdue_urgent_cargo_report(self) -> str:
        """Получить отчёт о просроченных срочных грузах"""
        return _get_string(_lib.Schedule_GetOverdueUrgentCargoReport, self._handle)
    
    def get_total_flight_time(self, aircraft_id: str) -> float:
        """Получить общее время полётов самолёта"""
        return _lib.Schedule_GetTotalFlightTime(self._handle, _to_bytes(aircraft_id))
    
    def get_total_flights(self) -> int:
        """Получить общее количество рейсов"""
        return _lib.Schedule_GetTotalFlights(self._handle)


class Flight:
    """Python класс для работы с рейсом"""
    
    def __init__(self, number: str, departure: str, destination: str,
                 dep_time: datetime, arr_time: datetime, aircraft_id: str):
        dep_ts = datetime_to_timestamp(dep_time)
        arr_ts = datetime_to_timestamp(arr_time)
        self._handle = _lib.Flight_Create(
            _to_bytes(number), _to_bytes(departure), _to_bytes(destination),
            dep_ts, arr_ts, _to_bytes(aircraft_id)
        )
        if not self._handle:
            raise RuntimeError("Failed to create Flight")
    
    @classmethod
    def _from_handle(cls, handle: Handle) -> 'Flight':
        """Создать Flight из существующего handle (внутренний метод)"""
        obj = cls.__new__(cls)
        obj._handle = handle
        return obj
    
    def __del__(self):
        if hasattr(self, '_handle') and self._handle:
            _lib.Flight_Destroy(self._handle)
    
    @property
    def flight_number(self) -> str:
        """Номер рейса"""
        return _get_string(_lib.Flight_GetFlightNumber, self._handle)
    
    @property
    def departure_airport(self) -> str:
        """Аэропорт отправления"""
        return _get_string(_lib.Flight_GetDepartureAirport, self._handle)
    
    @property
    def destination_airport(self) -> str:
        """Аэропорт назначения"""
        return _get_string(_lib.Flight_GetDestinationAirport, self._handle)
    
    @property
    def departure_time(self) -> datetime:
        """Время отправления"""
        ts = _lib.Flight_GetDepartureTime(self._handle)
        return timestamp_to_datetime(ts)
    
    @property
    def arrival_time(self) -> datetime:
        """Время прибытия"""
        ts = _lib.Flight_GetArrivalTime(self._handle)
        return timestamp_to_datetime(ts)
    
    @property
    def aircraft_id(self) -> str:
        """Идентификатор самолёта"""
        return _get_string(_lib.Flight_GetAircraftId, self._handle)
    
    @property
    def is_completed(self) -> bool:
        """Завершён ли рейс"""
        return _lib.Flight_IsCompleted(self._handle) != 0
    
    @property
    def duration_hours(self) -> float:
        """Продолжительность рейса в часах"""
        return _lib.Flight_GetFlightDurationHours(self._handle)
    
    def is_valid(self) -> bool:
        """Проверить корректность данных рейса"""
        return _lib.Flight_IsValid(self._handle) != 0
    
    def to_string(self) -> str:
        """Получить строковое представление рейса"""
        return _get_string(_lib.Flight_ToString, self._handle)


class Aircraft:
    """Python класс для работы с самолётом"""
    
    def __init__(self, number: str, max_payload: float):
        # Проверяем корректность грузоподъёмности перед вызовом C API
        if max_payload <= 0:
            raise ValueError(
                f"Грузоподъёмность самолёта должна быть положительным числом. "
                f"Получено значение: {max_payload} кг"
            )
        
        self._handle = _lib.Aircraft_Create(_to_bytes(number), c_double(max_payload))
        if not self._handle:
            raise RuntimeError(
                "Не удалось создать самолёт. Возможные причины:\n"
                "- Некорректные параметры самолёта\n"
                "- Ошибка в библиотеке"
            )
    
    def __del__(self):
        if hasattr(self, '_handle') and self._handle:
            _lib.Aircraft_Destroy(self._handle)
    
    @property
    def aircraft_number(self) -> str:
        """Номер самолёта"""
        return _get_string(_lib.Aircraft_GetAircraftNumber, self._handle)
    
    @property
    def max_payload(self) -> float:
        """Максимальная грузоподъёмность"""
        return _lib.Aircraft_GetMaxPayload(self._handle)
    
    @property
    def current_payload(self) -> float:
        """Текущая загрузка"""
        return _lib.Aircraft_GetCurrentPayload(self._handle)
    
    @property
    def available_capacity(self) -> float:
        """Доступная грузоподъёмность"""
        return _lib.Aircraft_GetAvailableCapacity(self._handle)
    
    def to_string(self) -> str:
        """Получить строковое представление самолёта"""
        return _get_string(_lib.Aircraft_ToString, self._handle)


class Airport:
    """Python класс для работы с аэропортом"""
    
    def __init__(self, name: str):
        self._handle = _lib.Airport_Create(_to_bytes(name))
        if not self._handle:
            raise RuntimeError("Failed to create Airport")
    
    def __del__(self):
        if hasattr(self, '_handle') and self._handle:
            _lib.Airport_Destroy(self._handle)
    
    @property
    def name(self) -> str:
        """Название аэропорта"""
        return _get_string(_lib.Airport_GetName, self._handle)
    
    def to_string(self) -> str:
        """Получить строковое представление аэропорта"""
        return _get_string(_lib.Airport_ToString, self._handle)


class Cargo:
    """Python класс для работы с грузом"""
    
    def __init__(self, number: str, mass: float, departure: str, 
                 destination: str, current: str, arrival: datetime):
        arr_ts = datetime_to_timestamp(arrival)
        self._handle = _lib.Cargo_Create(
            _to_bytes(number), c_double(mass), _to_bytes(departure),
            _to_bytes(destination), _to_bytes(current), arr_ts
        )
        if not self._handle:
            raise RuntimeError("Failed to create Cargo")
    
    def __del__(self):
        if hasattr(self, '_handle') and self._handle:
            _lib.Cargo_Destroy(self._handle)
    
    @property
    def cargo_number(self) -> str:
        """Номер груза"""
        return _get_string(_lib.Cargo_GetCargoNumber, self._handle)
    
    @property
    def mass(self) -> float:
        """Масса груза"""
        return _lib.Cargo_GetMass(self._handle)
    
    def to_string(self) -> str:
        """Получить строковое представление груза"""
        return _get_string(_lib.Cargo_ToString, self._handle)


class UrgentCargo:
    """Python класс для работы со срочным грузом"""
    
    def __init__(self, number: str, mass: float, departure: str, 
                 destination: str, current: str, arrival: datetime, deadline: datetime):
        arr_ts = datetime_to_timestamp(arrival)
        deadline_ts = datetime_to_timestamp(deadline)
        self._handle = _lib.UrgentCargo_Create(
            _to_bytes(number), c_double(mass), _to_bytes(departure),
            _to_bytes(destination), _to_bytes(current), arr_ts, deadline_ts
        )
        if not self._handle:
            raise RuntimeError("Failed to create UrgentCargo")
    
    def __del__(self):
        if hasattr(self, '_handle') and self._handle:
            _lib.UrgentCargo_Destroy(self._handle)
    
    @property
    def is_overdue(self) -> bool:
        """Просрочен ли груз"""
        return _lib.UrgentCargo_IsOverdue(self._handle) != 0
    
    def to_string(self) -> str:
        """Получить строковое представление срочного груза"""
        return _get_string(_lib.UrgentCargo_ToString, self._handle)


class Passenger:
    """Python класс для работы с пассажиром"""
    
    def __init__(self, number: str, name: str, departure: str, destination: str):
        self._handle = _lib.Passenger_Create(
            _to_bytes(number), _to_bytes(name), 
            _to_bytes(departure), _to_bytes(destination)
        )
        if not self._handle:
            raise RuntimeError("Failed to create Passenger")
    
    def __del__(self):
        if hasattr(self, '_handle') and self._handle:
            _lib.Passenger_Destroy(self._handle)
    
    @property
    def passenger_number(self) -> str:
        """Номер пассажира"""
        return _get_string(_lib.Passenger_GetPassengerNumber, self._handle)
    
    @property
    def name(self) -> str:
        """Имя пассажира"""
        return _get_string(_lib.Passenger_GetName, self._handle)
    
    def to_string(self) -> str:
        """Получить строковое представление пассажира"""
        return _get_string(_lib.Passenger_ToString, self._handle)

