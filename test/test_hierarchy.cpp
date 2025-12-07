#include <iostream>
#include <cassert>
#include <ctime>
#include <memory>
#include <windows.h>
#include <clocale>

// Включаем все наши классы
#include "Cargo.h"
#include "UrgentCargo.h"
#include "Passenger.h"
#include "Airport.h"
#include "Aircraft.h"
#include "Flight.h"
#include "Schedule.h"

bool runUrgentCargoTests() {
    std::cout << "=== Тестирование класса UrgentCargo ===" << std::endl;
    
    bool allTestsPassed = true;
    
    try {
        // Тест 1: Конструктор по умолчанию
        std::cout << "Тест 1: Конструктор по умолчанию... ";
        UrgentCargo urgentCargo1;
        assert(urgentCargo1.getCargoNumber() == "");
        assert(urgentCargo1.getMass() == 0.0);
        assert(urgentCargo1.getDeadline() == 0);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 2: Конструктор с параметрами
        std::cout << "Тест 2: Конструктор с параметрами... ";
        std::time_t now = std::time(nullptr);
        std::time_t deadline = now + 24 * 60 * 60; // +1 день
        UrgentCargo urgentCargo2("UC001", 100.0, "SVO", "LED", "SVO", now, deadline);
        assert(urgentCargo2.getCargoNumber() == "UC001");
        assert(urgentCargo2.getMass() == 100.0);
        assert(urgentCargo2.getDeadline() == deadline);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 3: Проверка срочности
        std::cout << "Тест 3: Проверка срочности... ";
        assert(!urgentCargo2.isOverdue());
        assert(!urgentCargo2.isUrgent()); // Не срочный, так как до крайнего срока больше 24 часов
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 4: Создание срочного груза из обычного
        std::cout << "Тест 4: Создание срочного груза из обычного... ";
        Cargo cargo("C001", 150.0, "SVO", "LED", "SVO", now);
        UrgentCargo urgentCargo3(cargo, deadline);
        assert(urgentCargo3.getCargoNumber() == "C001");
        assert(urgentCargo3.getMass() == 150.0);
        assert(urgentCargo3.getDeadline() == deadline);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 5: toString с информацией о крайнем сроке
        std::cout << "Тест 5: toString с информацией о крайнем сроке... ";
        std::string str = urgentCargo2.toString();
        assert(str.find("URGENT") != std::string::npos);
        assert(str.find("UC001") != std::string::npos);
        std::cout << "ПРОЙДЕН" << std::endl;
        
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

bool runPassengerTests() {
    std::cout << "=== Тестирование класса Passenger ===" << std::endl;
    
    bool allTestsPassed = true;
    
    try {
        // Тест 1: Конструктор по умолчанию
        std::cout << "Тест 1: Конструктор по умолчанию... ";
        Passenger passenger1;
        assert(passenger1.getPassengerNumber() == "");
        assert(passenger1.getName() == "");
        assert(passenger1.getMass() == 80.0); // Фиксированная масса
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 2: Конструктор с параметрами
        std::cout << "Тест 2: Конструктор с параметрами... ";
        Passenger passenger2("P001", "Иван Иванов", "SVO", "LED");
        assert(passenger2.getPassengerNumber() == "P001");
        assert(passenger2.getName() == "Иван Иванов");
        assert(passenger2.getDepartureAirport() == "SVO");
        assert(passenger2.getDestinationAirport() == "LED");
        assert(passenger2.getMass() == 80.0);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 3: Валидация
        std::cout << "Тест 3: Валидация... ";
        assert(passenger2.isValid());
        
        Passenger invalidPassenger("", "Имя", "SVO", "LED");
        assert(!invalidPassenger.isValid());
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 4: toString
        std::cout << "Тест 4: toString... ";
        std::string str = passenger2.toString();
        assert(str.find("P001") != std::string::npos);
        assert(str.find("Иван Иванов") != std::string::npos);
        assert(str.find("80.0") != std::string::npos);
        std::cout << "ПРОЙДЕН" << std::endl;
        
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

bool runAircraftTests() {
    std::cout << "=== Тестирование класса Aircraft ===" << std::endl;
    
    bool allTestsPassed = true;
    
    try {
        // Тест 1: Конструктор
        std::cout << "Тест 1: Конструктор... ";
        Aircraft aircraft("A001", 1000.0);
        assert(aircraft.getAircraftNumber() == "A001");
        assert(aircraft.getMaxPayload() == 1000.0);
        assert(aircraft.getCurrentPayload() == 0.0);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 2: Добавление груза
        std::cout << "Тест 2: Добавление груза... ";
        auto cargo = std::make_shared<Cargo>("C001", 100.0, "SVO", "LED", "SVO", std::time(nullptr));
        bool added = aircraft.addCargo(cargo);
        assert(added);
        assert(aircraft.getCurrentPayload() == 100.0);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 3: Добавление пассажира
        std::cout << "Тест 3: Добавление пассажира... ";
        auto passenger = std::make_shared<Passenger>("P001", "Иван", "SVO", "LED");
        bool passengerAdded = aircraft.addPassenger(passenger);
        assert(passengerAdded);
        assert(aircraft.getCurrentPayload() == 180.0); // 100 + 80
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 4: Проверка грузоподъёмности
        std::cout << "Тест 4: Проверка грузоподъёмности... ";
        auto heavyCargo = std::make_shared<Cargo>("C002", 900.0, "SVO", "LED", "SVO", std::time(nullptr));
        bool heavyAdded = aircraft.addCargo(heavyCargo);
        assert(!heavyAdded); // Не должен поместиться
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 5: toString
        std::cout << "Тест 5: toString... ";
        std::string str = aircraft.toString();
        assert(str.find("A001") != std::string::npos);
        assert(str.find("1000.00") != std::string::npos);
        std::cout << "ПРОЙДЕН" << std::endl;
        
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

bool runFlightTests() {
    std::cout << "=== Тестирование класса Flight ===" << std::endl;
    
    bool allTestsPassed = true;
    
    try {
        // Тест 1: Конструктор
        std::cout << "Тест 1: Конструктор... ";
        std::time_t now = std::time(nullptr);
        std::time_t departure = now + 3600; // +1 час
        std::time_t arrival = now + 7200;   // +2 часа
        Flight flight("F001", "SVO", "LED", departure, arrival, "A001");
        assert(flight.getFlightNumber() == "F001");
        assert(flight.getDepartureAirport() == "SVO");
        assert(flight.getDestinationAirport() == "LED");
        assert(flight.getAircraftId() == "A001");
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 2: Продолжительность полёта
        std::cout << "Тест 2: Продолжительность полёта... ";
        assert(flight.getFlightDuration() == 3600); // 1 час в секундах
        assert(flight.getFlightDurationHours() == 1.0);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 3: Создание обратного рейса
        std::cout << "Тест 3: Создание обратного рейса... ";
        Flight returnFlight = flight.createReturnFlight();
        assert(returnFlight.getDepartureAirport() == "LED");
        assert(returnFlight.getDestinationAirport() == "SVO");
        assert(returnFlight.getAircraftId() == "A001");
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 4: Валидация
        std::cout << "Тест 4: Валидация... ";
        assert(flight.isValid());
        
        Flight invalidFlight("", "SVO", "LED", departure, arrival, "A001");
        assert(!invalidFlight.isValid());
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 5: toString
        std::cout << "Тест 5: toString... ";
        std::string str = flight.toString();
        assert(str.find("F001") != std::string::npos);
        assert(str.find("SVO") != std::string::npos);
        assert(str.find("LED") != std::string::npos);
        std::cout << "ПРОЙДЕН" << std::endl;
        
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

bool runScheduleTests() {
    std::cout << "=== Тестирование класса Schedule ===" << std::endl;
    
    bool allTestsPassed = true;
    
    try {
        // Тест 1: Создание расписания
        std::cout << "Тест 1: Создание расписания... ";
        Schedule schedule;
        assert(schedule.getTotalFlights() == 0);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 2: Добавление рейсов
        std::cout << "Тест 2: Добавление рейсов... ";
        std::time_t now = std::time(nullptr);
        auto flight1 = std::make_shared<Flight>("F001", "SVO", "LED", now + 3600, now + 7200, "A001");
        auto flight2 = std::make_shared<Flight>("F002", "LED", "SVO", now + 10800, now + 14400, "A001");
        
        schedule.addFlight(flight1);
        schedule.addFlight(flight2);
        assert(schedule.getTotalFlights() == 2);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 3: Поиск рейса
        std::cout << "Тест 3: Поиск рейса... ";
        auto foundFlight = schedule.findFlight("F001");
        assert(foundFlight != nullptr);
        assert(foundFlight->getFlightNumber() == "F001");
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 4: Рейсы самолёта
        std::cout << "Тест 4: Рейсы самолёта... ";
        auto aircraftFlights = schedule.getFlightsByAircraft("A001");
        assert(aircraftFlights.size() == 2);
        std::cout << "ПРОЙДЕН" << std::endl;
        
        // Тест 5: Валидация расписания
        std::cout << "Тест 5: Валидация расписания... ";
        assert(schedule.isValid());
        std::cout << "ПРОЙДЕН" << std::endl;
        
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

void demonstrateHierarchy() {
    std::cout << "\n=== Демонстрация работы иерархии классов ===" << std::endl;
    
    // Создаём аэропорты
    auto airport1 = std::make_shared<Airport>("Шереметьево (SVO)");
    auto airport2 = std::make_shared<Airport>("Пулково (LED)");
    
    // Создаём самолёт
    auto aircraft = std::make_shared<Aircraft>("A001", 2000.0);
    
    // Создаём грузы
    std::time_t now = std::time(nullptr);
    auto cargo1 = std::make_shared<Cargo>("C001", 100.0, "SVO", "LED", "SVO", now);
    auto urgentCargo = std::make_shared<UrgentCargo>("UC001", 50.0, "SVO", "LED", "SVO", now, now + 24 * 60 * 60);
    
    // Создаём пассажиров
    auto passenger1 = std::make_shared<Passenger>("P001", "Иван Иванов", "SVO", "LED");
    auto passenger2 = std::make_shared<Passenger>("P002", "Петр Петров", "SVO", "LED");
    
    // Загружаем самолёт
    aircraft->addCargo(cargo1);
    aircraft->addUrgentCargo(urgentCargo);
    aircraft->addPassenger(passenger1);
    aircraft->addPassenger(passenger2);
    
    // Создаём рейс
    std::time_t departure = now + 3600;
    std::time_t arrival = now + 7200;
    auto flight = std::make_shared<Flight>("F001", "SVO", "LED", departure, arrival, "A001");
    
    // Создаём расписание
    Schedule schedule;
    schedule.addFlight(flight);
    
    // Выводим информацию
    std::cout << "Аэропорт 1: " << airport1->toString() << std::endl;
    std::cout << "Аэропорт 2: " << airport2->toString() << std::endl;
    std::cout << "Самолёт: " << aircraft->toString() << std::endl;
    std::cout << "Рейс: " << flight->toString() << std::endl;
    std::cout << "Расписание: " << schedule.toString() << std::endl;
    
    // Демонстрируем создание обратного рейса
    Flight returnFlight = flight->createReturnFlight();
    std::cout << "Обратный рейс: " << returnFlight.toString() << std::endl;
}

int testHierarchyMain() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "Программа тестирования иерархии классов авиаперевозок" << std::endl;
    std::cout << "=====================================================" << std::endl;
    
    bool allTestsPassed = true;
    
    // Запускаем тесты для каждого класса
    allTestsPassed &= runUrgentCargoTests();
    allTestsPassed &= runPassengerTests();
    allTestsPassed &= runAircraftTests();
    allTestsPassed &= runFlightTests();
    allTestsPassed &= runScheduleTests();
    
    if (allTestsPassed) {
        std::cout << "\n=== ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО! ===" << std::endl;
    } else {
        std::cout << "\n=== НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ! ===" << std::endl;
        return 1;
    }
    
    // Демонстрируем работу иерархии классов
    demonstrateHierarchy();
    
    return 0;
}
