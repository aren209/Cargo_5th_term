#include "Aircraft.h"
#include "FlightScheduleException.h"
#include <iostream>
#include <cassert>

/**
 * @brief Тест обработки исключения InvalidAircraftException
 * 
 * Проверяет, что при попытке создать самолёт с отрицательной
 * или нулевой грузоподъёмностью выбрасывается исключение.
 */
bool testInvalidAircraftException() {
    std::cout << "=== Тест обработки исключения InvalidAircraftException ===" << std::endl;
    
    bool allTestsPassed = true;
    
    // Тест 1: Отрицательная грузоподъёмность в конструкторе
    std::cout << "Тест 1: Создание самолёта с отрицательной грузоподъёмностью... ";
    try {
        Aircraft aircraft("TEST-001", -100.0);
        std::cout << "ПРОВАЛЕН (исключение не было выброшено)" << std::endl;
        allTestsPassed = false;
    } catch (const InvalidAircraftException& e) {
        std::cout << "ПРОЙДЕН" << std::endl;
        std::cout << "  Сообщение об ошибке: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ПРОВАЛЕН (выброшено другое исключение: " << e.what() << ")" << std::endl;
        allTestsPassed = false;
    } catch (...) {
        std::cout << "ПРОВАЛЕН (выброшено неизвестное исключение)" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 2: Нулевая грузоподъёмность в конструкторе
    std::cout << "Тест 2: Создание самолёта с нулевой грузоподъёмностью... ";
    try {
        Aircraft aircraft("TEST-002", 0.0);
        std::cout << "ПРОВАЛЕН (исключение не было выброшено)" << std::endl;
        allTestsPassed = false;
    } catch (const InvalidAircraftException& e) {
        std::cout << "ПРОЙДЕН" << std::endl;
        std::cout << "  Сообщение об ошибке: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ПРОВАЛЕН (выброшено другое исключение: " << e.what() << ")" << std::endl;
        allTestsPassed = false;
    } catch (...) {
        std::cout << "ПРОВАЛЕН (выброшено неизвестное исключение)" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 3: Отрицательная грузоподъёмность в setMaxPayload
    std::cout << "Тест 3: Установка отрицательной грузоподъёмности через setMaxPayload... ";
    try {
        Aircraft aircraft("TEST-003", 1000.0);  // Сначала создаём с корректной грузоподъёмностью
        aircraft.setMaxPayload(-50.0);  // Пытаемся установить отрицательную
        std::cout << "ПРОВАЛЕН (исключение не было выброшено)" << std::endl;
        allTestsPassed = false;
    } catch (const InvalidAircraftException& e) {
        std::cout << "ПРОЙДЕН" << std::endl;
        std::cout << "  Сообщение об ошибке: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ПРОВАЛЕН (выброшено другое исключение: " << e.what() << ")" << std::endl;
        allTestsPassed = false;
    } catch (...) {
        std::cout << "ПРОВАЛЕН (выброшено неизвестное исключение)" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 4: Корректная грузоподъёмность (не должна выбрасывать исключение)
    std::cout << "Тест 4: Создание самолёта с корректной грузоподъёмностью... ";
    try {
        Aircraft aircraft("TEST-004", 5000.0);
        assert(aircraft.getMaxPayload() == 5000.0);
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ПРОВАЛЕН (выброшено исключение: " << e.what() << ")" << std::endl;
        allTestsPassed = false;
    } catch (...) {
        std::cout << "ПРОВАЛЕН (выброшено неизвестное исключение)" << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}

int main() {
    std::cout << "Запуск тестов обработки исключений..." << std::endl;
    std::cout << std::endl;
    
    bool result = testInvalidAircraftException();
    
    std::cout << std::endl;
    if (result) {
        std::cout << "=== ВСЕ ТЕСТЫ ПРОЙДЕНЫ ===" << std::endl;
        return 0;
    } else {
        std::cout << "=== НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ ===" << std::endl;
        return 1;
    }
}

