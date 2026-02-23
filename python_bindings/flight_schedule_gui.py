"""
Графический интерфейс для системы управления расписанием авиаперевозок.

Использует модуль flight_schedule_lib для вызова C++ DLL. Обеспечивает вкладки:
Расписание, Рейсы, Самолёты, Аэропорты, Грузы, Пассажиры, Отчёты.
При запуске загружаются тестовые данные.

:requires: flight_schedule_lib (и собранная FlightScheduleAPI.dll)
"""

import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext
from datetime import datetime, timedelta
import sys
import os

# Добавляем путь к модулям
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

try:
    from flight_schedule_lib import (
        Schedule, Flight, Aircraft, Airport, Cargo, UrgentCargo, Passenger
    )
except ImportError as e:
    messagebox.showerror("Ошибка", f"Не удалось загрузить библиотеку: {e}\n"
                                   "Убедитесь, что DLL скомпилирована и находится в нужной директории.")
    sys.exit(1)


class FlightScheduleGUI:
    """Главное окно приложения: вкладки и работа с расписанием через Schedule."""

    def __init__(self, root):
        self.root = root
        self.root.title("Система управления расписанием авиаперевозок")
        self.root.geometry("1200x800")
        
        # Создаём расписание
        self.schedule = Schedule()
        
        # Хранилище данных
        self.aircrafts = {}
        self.airports = {}
        
        # Создаём интерфейс
        self.create_widgets()
        
        # Загружаем тестовые данные
        self.load_sample_data()
    
    def create_widgets(self):
        """Создание меню, панели вкладок и содержимого каждой вкладки."""
        
        # Создаём меню
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Файл", menu=file_menu)
        file_menu.add_command(label="Выход", command=self.root.quit)
        
        # Создаём панель вкладок
        notebook = ttk.Notebook(self.root)
        notebook.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Вкладка расписания
        self.create_schedule_tab(notebook)
        
        # Вкладка рейсов
        self.create_flights_tab(notebook)
        
        # Вкладка самолётов
        self.create_aircrafts_tab(notebook)
        
        # Вкладка аэропортов
        self.create_airports_tab(notebook)
        
        # Вкладка грузов
        self.create_cargo_tab(notebook)
        
        # Вкладка пассажиров
        self.create_passengers_tab(notebook)
        
        # Вкладка отчётов
        self.create_reports_tab(notebook)
    
    def create_schedule_tab(self, notebook):
        """Вкладка с общим расписанием"""
        frame = ttk.Frame(notebook)
        notebook.add(frame, text="Расписание")
        
        # Панель управления
        control_frame = ttk.Frame(frame)
        control_frame.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Button(control_frame, text="Обновить", command=self.refresh_schedule).pack(side=tk.LEFT, padx=2)
        ttk.Button(control_frame, text="Проверить валидность", command=self.validate_schedule).pack(side=tk.LEFT, padx=2)
        ttk.Button(control_frame, text="Исправить ошибки", command=self.fix_schedule).pack(side=tk.LEFT, padx=2)
        
        # Область вывода
        self.schedule_text = scrolledtext.ScrolledText(frame, wrap=tk.WORD, font=("Consolas", 10))
        self.schedule_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Обновляем расписание
        self.refresh_schedule()
    
    def create_flights_tab(self, notebook):
        """Вкладка для работы с рейсами"""
        frame = ttk.Frame(notebook)
        notebook.add(frame, text="Рейсы")
        
        # Форма добавления рейса
        form_frame = ttk.LabelFrame(frame, text="Добавить новый рейс")
        form_frame.pack(fill=tk.X, padx=5, pady=5)
        
        # Номер рейса
        ttk.Label(form_frame, text="Номер рейса:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.flight_number_entry = ttk.Entry(form_frame, width=30)
        self.flight_number_entry.grid(row=0, column=1, padx=5, pady=2)
        
        # Аэропорт отправления
        ttk.Label(form_frame, text="Аэропорт отправления:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.departure_airport_entry = ttk.Entry(form_frame, width=30)
        self.departure_airport_entry.grid(row=1, column=1, padx=5, pady=2)
        
        # Аэропорт назначения
        ttk.Label(form_frame, text="Аэропорт назначения:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        self.destination_airport_entry = ttk.Entry(form_frame, width=30)
        self.destination_airport_entry.grid(row=2, column=1, padx=5, pady=2)
        
        # Дата и время отправления
        ttk.Label(form_frame, text="Дата/время отправления (YYYY-MM-DD HH:MM):").grid(row=3, column=0, sticky=tk.W, padx=5, pady=2)
        self.departure_time_entry = ttk.Entry(form_frame, width=30)
        self.departure_time_entry.grid(row=3, column=1, padx=5, pady=2)
        self.departure_time_entry.insert(0, datetime.now().strftime("%Y-%m-%d %H:%M"))
        
        # Дата и время прибытия
        ttk.Label(form_frame, text="Дата/время прибытия (YYYY-MM-DD HH:MM):").grid(row=4, column=0, sticky=tk.W, padx=5, pady=2)
        self.arrival_time_entry = ttk.Entry(form_frame, width=30)
        self.arrival_time_entry.grid(row=4, column=1, padx=5, pady=2)
        self.arrival_time_entry.insert(0, (datetime.now() + timedelta(hours=2)).strftime("%Y-%m-%d %H:%M"))
        
        # Самолёт
        ttk.Label(form_frame, text="ID самолёта:").grid(row=5, column=0, sticky=tk.W, padx=5, pady=2)
        self.aircraft_id_entry = ttk.Entry(form_frame, width=30)
        self.aircraft_id_entry.grid(row=5, column=1, padx=5, pady=2)
        
        # Кнопка добавления
        ttk.Button(form_frame, text="Добавить рейс", command=self.add_flight).grid(row=6, column=0, columnspan=2, pady=5)
        
        # Список рейсов
        list_frame = ttk.LabelFrame(frame, text="Список рейсов")
        list_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Treeview для списка рейсов
        columns = ("Номер", "Откуда", "Куда", "Отправление", "Прибытие", "Самолёт")
        self.flights_tree = ttk.Treeview(list_frame, columns=columns, show="headings", height=15)
        
        for col in columns:
            self.flights_tree.heading(col, text=col)
            self.flights_tree.column(col, width=150)
        
        scrollbar = ttk.Scrollbar(list_frame, orient=tk.VERTICAL, command=self.flights_tree.yview)
        self.flights_tree.configure(yscrollcommand=scrollbar.set)
        
        self.flights_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Кнопка удаления
        ttk.Button(list_frame, text="Удалить выбранный рейс", command=self.remove_flight).pack(pady=5)
    
    def create_aircrafts_tab(self, notebook):
        """Вкладка для работы с самолётами"""
        frame = ttk.Frame(notebook)
        notebook.add(frame, text="Самолёты")
        
        # Форма добавления самолёта
        form_frame = ttk.LabelFrame(frame, text="Добавить новый самолёт")
        form_frame.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(form_frame, text="Номер самолёта:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.aircraft_number_entry = ttk.Entry(form_frame, width=30)
        self.aircraft_number_entry.grid(row=0, column=1, padx=5, pady=2)
        
        ttk.Label(form_frame, text="Максимальная грузоподъёмность (кг):").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.aircraft_max_payload_entry = ttk.Entry(form_frame, width=30)
        self.aircraft_max_payload_entry.grid(row=1, column=1, padx=5, pady=2)
        
        ttk.Button(form_frame, text="Добавить самолёт", command=self.add_aircraft).grid(row=2, column=0, columnspan=2, pady=5)
        
        # Список самолётов
        self.aircrafts_listbox = tk.Listbox(frame, height=15)
        self.aircrafts_listbox.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    def create_airports_tab(self, notebook):
        """Вкладка для работы с аэропортами"""
        frame = ttk.Frame(notebook)
        notebook.add(frame, text="Аэропорты")
        
        # Форма добавления аэропорта
        form_frame = ttk.LabelFrame(frame, text="Добавить новый аэропорт")
        form_frame.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(form_frame, text="Название аэропорта:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.airport_name_entry = ttk.Entry(form_frame, width=30)
        self.airport_name_entry.grid(row=0, column=1, padx=5, pady=2)
        
        ttk.Button(form_frame, text="Добавить аэропорт", command=self.add_airport).grid(row=1, column=0, columnspan=2, pady=5)
        
        # Список аэропортов
        self.airports_listbox = tk.Listbox(frame, height=15)
        self.airports_listbox.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    def create_cargo_tab(self, notebook):
        """Вкладка для работы с грузами"""
        frame = ttk.Frame(notebook)
        notebook.add(frame, text="Грузы")
        
        # Форма добавления груза
        form_frame = ttk.LabelFrame(frame, text="Добавить груз")
        form_frame.pack(fill=tk.X, padx=5, pady=5)
        
        self.cargo_type_var = tk.StringVar(value="обычный")
        ttk.Radiobutton(form_frame, text="Обычный", variable=self.cargo_type_var, value="обычный").grid(row=0, column=0, padx=5)
        ttk.Radiobutton(form_frame, text="Срочный", variable=self.cargo_type_var, value="срочный").grid(row=0, column=1, padx=5)
        
        ttk.Label(form_frame, text="Номер груза:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.cargo_number_entry = ttk.Entry(form_frame, width=30)
        self.cargo_number_entry.grid(row=1, column=1, padx=5, pady=2)
        
        ttk.Label(form_frame, text="Масса (кг):").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        self.cargo_mass_entry = ttk.Entry(form_frame, width=30)
        self.cargo_mass_entry.grid(row=2, column=1, padx=5, pady=2)
        
        ttk.Label(form_frame, text="Аэропорт отправления:").grid(row=3, column=0, sticky=tk.W, padx=5, pady=2)
        self.cargo_departure_entry = ttk.Entry(form_frame, width=30)
        self.cargo_departure_entry.grid(row=3, column=1, padx=5, pady=2)
        
        ttk.Label(form_frame, text="Аэропорт назначения:").grid(row=4, column=0, sticky=tk.W, padx=5, pady=2)
        self.cargo_destination_entry = ttk.Entry(form_frame, width=30)
        self.cargo_destination_entry.grid(row=4, column=1, padx=5, pady=2)
        
        ttk.Label(form_frame, text="Крайний срок доставки (YYYY-MM-DD HH:MM):").grid(row=5, column=0, sticky=tk.W, padx=5, pady=2)
        self.cargo_deadline_entry = ttk.Entry(form_frame, width=30)
        self.cargo_deadline_entry.grid(row=5, column=1, padx=5, pady=2)
        self.cargo_deadline_entry.insert(0, (datetime.now() + timedelta(days=7)).strftime("%Y-%m-%d %H:%M"))
        
        ttk.Button(form_frame, text="Добавить груз", command=self.add_cargo).grid(row=6, column=0, columnspan=2, pady=5)
        
        # Информационное поле
        self.cargo_info_text = scrolledtext.ScrolledText(frame, wrap=tk.WORD, height=10)
        self.cargo_info_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    def create_passengers_tab(self, notebook):
        """Вкладка для работы с пассажирами"""
        frame = ttk.Frame(notebook)
        notebook.add(frame, text="Пассажиры")
        
        # Форма добавления пассажира
        form_frame = ttk.LabelFrame(frame, text="Добавить пассажира")
        form_frame.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(form_frame, text="Номер пассажира:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.passenger_number_entry = ttk.Entry(form_frame, width=30)
        self.passenger_number_entry.grid(row=0, column=1, padx=5, pady=2)
        
        ttk.Label(form_frame, text="Имя:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.passenger_name_entry = ttk.Entry(form_frame, width=30)
        self.passenger_name_entry.grid(row=1, column=1, padx=5, pady=2)
        
        ttk.Label(form_frame, text="Аэропорт отправления:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        self.passenger_departure_entry = ttk.Entry(form_frame, width=30)
        self.passenger_departure_entry.grid(row=2, column=1, padx=5, pady=2)
        
        ttk.Label(form_frame, text="Аэропорт назначения:").grid(row=3, column=0, sticky=tk.W, padx=5, pady=2)
        self.passenger_destination_entry = ttk.Entry(form_frame, width=30)
        self.passenger_destination_entry.grid(row=3, column=1, padx=5, pady=2)
        
        ttk.Button(form_frame, text="Добавить пассажира", command=self.add_passenger).grid(row=4, column=0, columnspan=2, pady=5)
        
        # Информационное поле
        self.passenger_info_text = scrolledtext.ScrolledText(frame, wrap=tk.WORD, height=10)
        self.passenger_info_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    def create_reports_tab(self, notebook):
        """Вкладка для отчётов"""
        frame = ttk.Frame(notebook)
        notebook.add(frame, text="Отчёты")
        
        # Панель управления
        control_frame = ttk.Frame(frame)
        control_frame.pack(fill=tk.X, padx=5, pady=5)
        
        ttk.Label(control_frame, text="ID самолёта:").pack(side=tk.LEFT, padx=5)
        self.report_aircraft_entry = ttk.Entry(control_frame, width=20)
        self.report_aircraft_entry.pack(side=tk.LEFT, padx=5)
        
        ttk.Label(control_frame, text="Начало периода:").pack(side=tk.LEFT, padx=5)
        self.report_start_entry = ttk.Entry(control_frame, width=20)
        self.report_start_entry.insert(0, datetime.now().strftime("%Y-%m-%d %H:%M"))
        self.report_start_entry.pack(side=tk.LEFT, padx=5)
        
        ttk.Label(control_frame, text="Конец периода:").pack(side=tk.LEFT, padx=5)
        self.report_end_entry = ttk.Entry(control_frame, width=20)
        self.report_end_entry.insert(0, (datetime.now() + timedelta(days=7)).strftime("%Y-%m-%d %H:%M"))
        self.report_end_entry.pack(side=tk.LEFT, padx=5)
        
        ttk.Button(control_frame, text="Расписание самолёта", command=self.show_aircraft_schedule).pack(side=tk.LEFT, padx=5)
        ttk.Button(control_frame, text="Просроченные грузы", command=self.show_overdue_cargo).pack(side=tk.LEFT, padx=5)
        
        # Область вывода отчётов
        self.reports_text = scrolledtext.ScrolledText(frame, wrap=tk.WORD, font=("Consolas", 10))
        self.reports_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    # Методы для работы с данными
    
    def load_sample_data(self):
        """Загрузка тестовых данных"""
        try:
            # Добавляем несколько самолётов
            aircraft1 = Aircraft("A-001", 5000.0)
            aircraft2 = Aircraft("A-002", 8000.0)
            self.aircrafts["A-001"] = aircraft1
            self.aircrafts["A-002"] = aircraft2
            
            # Добавляем аэропорты
            airport1 = Airport("Шереметьево")
            airport2 = Airport("Домодедово")
            airport3 = Airport("Внуково")
            self.airports["Шереметьево"] = airport1
            self.airports["Домодедово"] = airport2
            self.airports["Внуково"] = airport3
            
            # Добавляем тестовые рейсы
            now = datetime.now()
            flight1 = Flight("FL-001", "Шереметьево", "Домодедово", 
                           now + timedelta(hours=1), now + timedelta(hours=2), "A-001")
            flight2 = Flight("FL-002", "Домодедово", "Внуково",
                           now + timedelta(hours=3), now + timedelta(hours=4), "A-002")
            
            self.schedule.add_flight(flight1)
            self.schedule.add_flight(flight2)
            
            self.refresh_schedule()
            self.refresh_flights()
            
        except Exception as e:
            messagebox.showwarning("Предупреждение", f"Ошибка при загрузке тестовых данных: {e}")
    
    def refresh_schedule(self):
        """Обновить отображение расписания"""
        try:
            schedule_text = self.schedule.to_string()
            self.schedule_text.delete(1.0, tk.END)
            self.schedule_text.insert(1.0, schedule_text)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось обновить расписание: {e}")
    
    def validate_schedule(self):
        """Проверить валидность расписания"""
        try:
            if self.schedule.is_valid():
                messagebox.showinfo("Результат", "Расписание корректно!")
            else:
                errors = self.schedule.get_validation_errors()
                error_msg = "\n".join(errors) if errors else "Обнаружены ошибки в расписании"
                messagebox.showerror("Ошибки валидации", error_msg)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось проверить расписание: {e}")
    
    def fix_schedule(self):
        """Исправить ошибки в расписании"""
        try:
            self.schedule.validate_and_fix()
            messagebox.showinfo("Результат", "Попытка исправления ошибок выполнена.")
            self.refresh_schedule()
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось исправить ошибки: {e}")
    
    def add_flight(self):
        """Добавить новый рейс"""
        try:
            flight_number = self.flight_number_entry.get().strip()
            departure = self.departure_airport_entry.get().strip()
            destination = self.destination_airport_entry.get().strip()
            dep_time_str = self.departure_time_entry.get().strip()
            arr_time_str = self.arrival_time_entry.get().strip()
            aircraft_id = self.aircraft_id_entry.get().strip()
            
            if not all([flight_number, departure, destination, dep_time_str, arr_time_str, aircraft_id]):
                messagebox.showerror("Ошибка", "Заполните все поля!")
                return
            
            dep_time = datetime.strptime(dep_time_str, "%Y-%m-%d %H:%M")
            arr_time = datetime.strptime(arr_time_str, "%Y-%m-%d %H:%M")
            
            flight = Flight(flight_number, departure, destination, dep_time, arr_time, aircraft_id)
            
            if self.schedule.add_flight(flight):
                messagebox.showinfo("Успех", "Рейс добавлен!")
                self.flight_number_entry.delete(0, tk.END)
                self.departure_airport_entry.delete(0, tk.END)
                self.destination_airport_entry.delete(0, tk.END)
                self.arrival_time_entry.delete(0, tk.END)
                self.aircraft_id_entry.delete(0, tk.END)
                self.refresh_flights()
                self.refresh_schedule()
            else:
                messagebox.showerror("Ошибка", "Не удалось добавить рейс!")
        except ValueError as e:
            messagebox.showerror("Ошибка", f"Неверный формат даты/времени: {e}")
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось добавить рейс: {e}")
    
    def refresh_flights(self):
        """Обновить список рейсов"""
        # Очищаем дерево
        for item in self.flights_tree.get_children():
            self.flights_tree.delete(item)
        
        # Здесь можно добавить логику для получения всех рейсов из расписания
        # Для этого нужно расширить API расписания
    
    def remove_flight(self):
        """Удалить выбранный рейс"""
        selected = self.flights_tree.selection()
        if not selected:
            messagebox.showwarning("Предупреждение", "Выберите рейс для удаления!")
            return
        
        item = self.flights_tree.item(selected[0])
        flight_number = item['values'][0]
        
        if messagebox.askyesno("Подтверждение", f"Удалить рейс {flight_number}?"):
            if self.schedule.remove_flight(flight_number):
                messagebox.showinfo("Успех", "Рейс удалён!")
                self.refresh_flights()
                self.refresh_schedule()
            else:
                messagebox.showerror("Ошибка", "Не удалось удалить рейс!")
    
    def add_aircraft(self):
        """Добавить новый самолёт"""
        try:
            number = self.aircraft_number_entry.get().strip()
            
            if not number:
                messagebox.showerror("Ошибка", "Введите номер самолёта!")
                return
            
            try:
                max_payload = float(self.aircraft_max_payload_entry.get())
            except ValueError:
                messagebox.showerror("Ошибка", "Неверное значение грузоподъёмности! Введите число.")
                return
            
            # Проверяем грузоподъёмность перед созданием самолёта
            if max_payload <= 0:
                messagebox.showerror(
                    "Ошибка", 
                    f"Грузоподъёмность самолёта должна быть положительным числом.\n"
                    f"Получено значение: {max_payload} кг"
                )
                return
            
            aircraft = Aircraft(number, max_payload)
            self.aircrafts[number] = aircraft
            self.aircrafts_listbox.insert(tk.END, f"{number}: грузоподъёмность {max_payload} кг")
            
            self.aircraft_number_entry.delete(0, tk.END)
            self.aircraft_max_payload_entry.delete(0, tk.END)
            
            messagebox.showinfo("Успех", "Самолёт добавлен!")
        except ValueError as e:
            # Обрабатываем ValueError из класса Aircraft (с информативным сообщением)
            messagebox.showerror("Ошибка", str(e))
        except Exception as e:
            # Обрабатываем другие исключения
            error_msg = str(e)
            if "Failed to create Aircraft" in error_msg:
                messagebox.showerror(
                    "Ошибка", 
                    "Не удалось создать самолёт. Возможные причины:\n"
                    "- Некорректные параметры самолёта\n"
                    "- Ошибка в библиотеке"
                )
            else:
                messagebox.showerror("Ошибка", f"Не удалось добавить самолёт: {error_msg}")
    
    def add_airport(self):
        """Добавить новый аэропорт"""
        try:
            name = self.airport_name_entry.get().strip()
            
            if not name:
                messagebox.showerror("Ошибка", "Введите название аэропорта!")
                return
            
            airport = Airport(name)
            self.airports[name] = airport
            self.airports_listbox.insert(tk.END, name)
            
            self.airport_name_entry.delete(0, tk.END)
            messagebox.showinfo("Успех", "Аэропорт добавлен!")
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось добавить аэропорт: {e}")
    
    def add_cargo(self):
        """Добавить новый груз"""
        try:
            number = self.cargo_number_entry.get().strip()
            mass = float(self.cargo_mass_entry.get())
            departure = self.cargo_departure_entry.get().strip()
            destination = self.cargo_destination_entry.get().strip()
            cargo_type = self.cargo_type_var.get()
            
            if not all([number, departure, destination]):
                messagebox.showerror("Ошибка", "Заполните все обязательные поля!")
                return
            
            now = datetime.now()
            
            if cargo_type == "срочный":
                deadline_str = self.cargo_deadline_entry.get().strip()
                deadline = datetime.strptime(deadline_str, "%Y-%m-%d %H:%M")
                cargo = UrgentCargo(number, mass, departure, destination, departure, now, deadline)
                info = f"Срочный груз {number} добавлен.\n{cargo.to_string()}"
                if cargo.is_overdue:
                    info += "\n⚠ ВНИМАНИЕ: Груз просрочен!"
            else:
                cargo = Cargo(number, mass, departure, destination, departure, now)
                info = f"Груз {number} добавлен.\n{cargo.to_string()}"
            
            self.cargo_info_text.insert(tk.END, info + "\n\n")
            self.cargo_info_text.see(tk.END)
            
            # Очищаем форму
            self.cargo_number_entry.delete(0, tk.END)
            self.cargo_mass_entry.delete(0, tk.END)
            self.cargo_departure_entry.delete(0, tk.END)
            self.cargo_destination_entry.delete(0, tk.END)
            
            messagebox.showinfo("Успех", "Груз добавлен!")
        except ValueError as e:
            messagebox.showerror("Ошибка", f"Неверный формат данных: {e}")
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось добавить груз: {e}")
    
    def add_passenger(self):
        """Добавить нового пассажира"""
        try:
            number = self.passenger_number_entry.get().strip()
            name = self.passenger_name_entry.get().strip()
            departure = self.passenger_departure_entry.get().strip()
            destination = self.passenger_destination_entry.get().strip()
            
            if not all([number, name, departure, destination]):
                messagebox.showerror("Ошибка", "Заполните все поля!")
                return
            
            passenger = Passenger(number, name, departure, destination)
            info = f"Пассажир добавлен.\n{passenger.to_string()}"
            
            self.passenger_info_text.insert(tk.END, info + "\n\n")
            self.passenger_info_text.see(tk.END)
            
            # Очищаем форму
            self.passenger_number_entry.delete(0, tk.END)
            self.passenger_name_entry.delete(0, tk.END)
            self.passenger_departure_entry.delete(0, tk.END)
            self.passenger_destination_entry.delete(0, tk.END)
            
            messagebox.showinfo("Успех", "Пассажир добавлен!")
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось добавить пассажира: {e}")
    
    def show_aircraft_schedule(self):
        """Показать расписание самолёта"""
        try:
            aircraft_id = self.report_aircraft_entry.get().strip()
            
            if not aircraft_id:
                messagebox.showerror("Ошибка", "Введите ID самолёта!")
                return
            
            start_str = self.report_start_entry.get().strip()
            end_str = self.report_end_entry.get().strip()
            
            if start_str and end_str:
                start_time = datetime.strptime(start_str, "%Y-%m-%d %H:%M")
                end_time = datetime.strptime(end_str, "%Y-%m-%d %H:%M")
                schedule_text = self.schedule.get_schedule_for_aircraft_in_range(aircraft_id, start_time, end_time)
            else:
                schedule_text = self.schedule.get_schedule_for_aircraft(aircraft_id)
            
            flight_time = self.schedule.get_total_flight_time(aircraft_id)
            schedule_text += f"\nОбщее время полётов: {flight_time:.2f} часов\n"
            
            self.reports_text.delete(1.0, tk.END)
            self.reports_text.insert(1.0, schedule_text)
        except ValueError as e:
            messagebox.showerror("Ошибка", f"Неверный формат даты/времени: {e}")
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось получить расписание: {e}")
    
    def show_overdue_cargo(self):
        """Показать просроченные срочные грузы"""
        try:
            report = self.schedule.get_overdue_urgent_cargo_report()
            self.reports_text.delete(1.0, tk.END)
            self.reports_text.insert(1.0, report)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось получить отчёт: {e}")


def main():
    """Главная функция"""
    root = tk.Tk()
    app = FlightScheduleGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()

