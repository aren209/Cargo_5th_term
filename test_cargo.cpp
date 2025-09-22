#include "Cargo.h"
#include <iostream>
#include <cassert>
#include <ctime>
#include <clocale>
#include <windows.h>


bool runCargoTests() {
    std::cout << "=== Запуск тестов класса Cargo ===" << std::endl;
    
    bool allTestsPassed = true;
    
    // Тест 1: Конструктор по умолчанию
    std::cout << "Тест 1: Конструктор по умолчанию... ";
    try {
        Cargo cargo1;
        assert(cargo1.getCargoNumber() == "");
        assert(cargo1.getMass() == 0.0);
        assert(cargo1.getDepartureAirport() == "");
        assert(cargo1.getDestinationAirport() == "");
        assert(cargo1.getCurrentAirport() == "");
        assert(cargo1.getArrivalTime() == 0);
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 2: Конструктор с параметрами
    std::cout << "Тест 2: Конструктор с параметрами... ";
    try {
        std::time_t now = std::time(nullptr);
        Cargo cargo2("C001", 150.5, "SVO", "LED", "SVO", now);
        assert(cargo2.getCargoNumber() == "C001");
        assert(cargo2.getMass() == 150.5);
        assert(cargo2.getDepartureAirport() == "SVO");
        assert(cargo2.getDestinationAirport() == "LED");
        assert(cargo2.getCurrentAirport() == "SVO");
        assert(cargo2.getArrivalTime() == now);
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 3: Конструктор копирования
    std::cout << "Тест 3: Конструктор копирования... ";
    try {
        std::time_t now = std::time(nullptr);
        Cargo cargo3("C002", 200.0, "LED", "SVO", "LED", now);
        Cargo cargo4(cargo3);
        assert(cargo4.getCargoNumber() == cargo3.getCargoNumber());
        assert(cargo4.getMass() == cargo3.getMass());
        assert(cargo4.getDepartureAirport() == cargo3.getDepartureAirport());
        assert(cargo4.getDestinationAirport() == cargo3.getDestinationAirport());
        assert(cargo4.getCurrentAirport() == cargo3.getCurrentAirport());
        assert(cargo4.getArrivalTime() == cargo3.getArrivalTime());
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 4: Оператор присваивания
    std::cout << "Тест 4: Оператор присваивания... ";
    try {
        std::time_t now = std::time(nullptr);
        Cargo cargo5("C003", 300.0, "SVO", "LED", "SVO", now);
        Cargo cargo6;
        cargo6 = cargo5;
        assert(cargo6.getCargoNumber() == cargo5.getCargoNumber());
        assert(cargo6.getMass() == cargo5.getMass());
        assert(cargo6.getDepartureAirport() == cargo5.getDepartureAirport());
        assert(cargo6.getDestinationAirport() == cargo5.getDestinationAirport());
        assert(cargo6.getCurrentAirport() == cargo5.getCurrentAirport());
        assert(cargo6.getArrivalTime() == cargo5.getArrivalTime());
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 5: Операторы сравнения по массе
    std::cout << "Тест 5: Операторы сравнения по массе... ";
    try {
        Cargo cargo7("C004", 100.0, "SVO", "LED", "SVO", 0);
        Cargo cargo8("C005", 200.0, "LED", "SVO", "LED", 0);
        Cargo cargo9("C006", 100.0, "SVO", "LED", "SVO", 0);
        
        // Тестируем операторы сравнения
        assert(cargo7 < cargo8);   
        assert(cargo8 > cargo7);   
        assert(cargo7 <= cargo8);  
        assert(cargo8 >= cargo7);  
        assert(cargo7 == cargo9);  
        assert(cargo7 != cargo8);  
        assert(cargo7 <= cargo9);  
        assert(cargo7 >= cargo9);  
        
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 6: Метод moveToAirport
    std::cout << "Тест 6: Метод moveToAirport... ";
    try {
        Cargo cargo10("C007", 150.0, "SVO", "LED", "SVO", 0);
        cargo10.moveToAirport("LED");
        assert(cargo10.getCurrentAirport() == "LED");
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 7: Метод hasReachedDestination
    std::cout << "Тест 7: Метод hasReachedDestination... ";
    try {
        Cargo cargo11("C008", 200.0, "SVO", "LED", "SVO", 0);
        Cargo cargo12("C009", 250.0, "SVO", "LED", "LED", 0);
        
        assert(!cargo11.hasReachedDestination()); // Не достиг места назначения
        assert(cargo12.hasReachedDestination());  // Достиг места назначения
        
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 8: Метод toString
    std::cout << "Тест 8: Метод toString... ";
    try {
        std::time_t now = std::time(nullptr);
        Cargo cargo13("C010", 175.5, "SVO", "LED", "SVO", now);
        std::string str = cargo13.toString();
        
        // Проверяем, что строка содержит основные элементы
        assert(str.find("C010") != std::string::npos);
        assert(str.find("175.50") != std::string::npos);
        assert(str.find("SVO") != std::string::npos);
        assert(str.find("LED") != std::string::npos);
        
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 9: Метод isValid
    std::cout << "Тест 9: Метод isValid... ";
    try {
        // Валидный груз
        Cargo cargo14("C011", 100.0, "SVO", "LED", "SVO", 0);
        assert(cargo14.isValid());
        
        // пустой номер
        Cargo cargo15("", 100.0, "SVO", "LED", "SVO", 0);
        assert(!cargo15.isValid());
        
        // отрицательная масса
        Cargo cargo16("C012", -50.0, "SVO", "LED", "SVO", 0);
        assert(!cargo16.isValid());
        
        // одинаковые аэропорты
        Cargo cargo17("C013", 100.0, "SVO", "SVO", "SVO", 0);
        assert(!cargo17.isValid());
        
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    // Тест 10: Сеттеры и геттеры
    std::cout << "Тест 10: Сеттеры и геттеры... ";
    try {
        Cargo cargo18;
        std::time_t now = std::time(nullptr);
        
        cargo18.setCargoNumber("C014");
        cargo18.setMass(300.0);
        cargo18.setDepartureAirport("LED");
        cargo18.setDestinationAirport("SVO");
        cargo18.setCurrentAirport("LED");
        cargo18.setArrivalTime(now);
        
        assert(cargo18.getCargoNumber() == "C014");
        assert(cargo18.getMass() == 300.0);
        assert(cargo18.getDepartureAirport() == "LED");
        assert(cargo18.getDestinationAirport() == "SVO");
        assert(cargo18.getCurrentAirport() == "LED");
        assert(cargo18.getArrivalTime() == now);
        
        std::cout << "ПРОЙДЕН" << std::endl;
    } catch (...) {
        std::cout << "ПРОВАЛЕН" << std::endl;
        allTestsPassed = false;
    }
    
    return allTestsPassed;
}


void demonstrateCargo() {
    std::cout << "\n=== Демонстрация работы класса Cargo ===" << std::endl;
    
    // Создаем несколько грузов
    std::time_t now = std::time(nullptr);
    Cargo cargo1("C001", 150.5, "SVO", "LED", "SVO", now);
    Cargo cargo2("C002", 200.0, "LED", "SVO", "LED", now);
    Cargo cargo3("C003", 100.0, "SVO", "LED", "SVO", now);
    
    // Выводим информацию о грузах
    std::cout << "Груз 1: " << cargo1.toString() << std::endl;
    std::cout << "Груз 2: " << cargo2.toString() << std::endl;
    std::cout << "Груз 3: " << cargo3.toString() << std::endl;
    
    // Демонстрируем сравнение по массе
    std::cout << "\nСравнение по массе:" << std::endl;
    std::cout << "Груз 1 < Груз 2: " << (cargo1 < cargo2 ? "true" : "false") << std::endl;
    std::cout << "Груз 1 == Груз 3: " << (cargo1 == cargo3 ? "true" : "false") << std::endl;
    std::cout << "Груз 2 > Груз 3: " << (cargo2 > cargo3 ? "true" : "false") << std::endl;
    
    // Демонстрируем перемещение груза
    std::cout << "\nПеремещение груза 1 в аэропорт назначения:" << std::endl;
    std::cout << "До перемещения: " << cargo1.toString() << std::endl;
    cargo1.moveToAirport("LED");
    std::cout << "После перемещения: " << cargo1.toString() << std::endl;
    std::cout << "Достиг места назначения: " << (cargo1.hasReachedDestination() ? "да" : "нет") << std::endl;
    
    // Демонстрируем валидацию
    std::cout << "\nВалидация грузов:" << std::endl;
    std::cout << "Груз 1 валиден: " << (cargo1.isValid() ? "да" : "нет") << std::endl;
    std::cout << "Груз 2 валиден: " << (cargo2.isValid() ? "да" : "нет") << std::endl;
    std::cout << "Груз 200 валиден: " << (cargo3.isValid() ? "да" : "нет") << std::endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "Программа тестирования класса Cargo" << std::endl;
    std::cout << "===================================" << std::endl;
    
    // Запускаем тесты
    bool testsPassed = runCargoTests();
    
    if (testsPassed) {
        std::cout << "\n=== ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО! ===" << std::endl;
    } else {
        std::cout << "\n=== НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ! ===" << std::endl;
        return 1;
    }
    
    // Демонстрируем работу класса
    demonstrateCargo();
    
    return 0;
}
