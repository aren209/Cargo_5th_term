#include <iostream>
#include <windows.h>
#include <clocale>

// Объявляем функции тестирования
int testCargoMain();
int testHierarchyMain();

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "=====================================================" << std::endl;
    std::cout << "    СИСТЕМА УПРАВЛЕНИЯ АВИАПЕРЕВОЗКАМИ" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << std::endl;

    // Запускаем тесты базового класса Cargo
    std::cout << "ЗАПУСК ТЕСТОВ БАЗОВОГО КЛАССА CARGO" << std::endl;
    std::cout << "====================================" << std::endl;
    bool cargoTestsPassed = testCargoMain() == 0;
    
    std::cout << std::endl;
    std::cout << "ЗАПУСК ТЕСТОВ ИЕРАРХИИ КЛАССОВ" << std::endl;
    std::cout << "===============================" << std::endl;
    bool hierarchyTestsPassed = testHierarchyMain() == 0;
    
    std::cout << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << "                    ИТОГОВЫЙ РЕЗУЛЬТАТ" << std::endl;
    std::cout << "=====================================================" << std::endl;
    
    if (cargoTestsPassed && hierarchyTestsPassed) {
        std::cout << "✅ ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!" << std::endl;
        std::cout << "✅ Иерархия классов готова к использованию!" << std::endl;
        return 0;
    } else {
        std::cout << "❌ НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ!" << std::endl;
        if (!cargoTestsPassed) {
            std::cout << "❌ Тесты базового класса Cargo провалены" << std::endl;
        }
        if (!hierarchyTestsPassed) {
            std::cout << "❌ Тесты иерархии классов провалены" << std::endl;
        }
        return 1;
    }
}

