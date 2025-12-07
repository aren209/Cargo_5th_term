#ifndef URGENT_CARGO_H
#define URGENT_CARGO_H

#include "Cargo.h"
#include <ctime>

class UrgentCargo : public Cargo {
private:
    std::time_t deadline;  ///< Крайняя дата доставки до аэропорта назначения

public:
    // Конструкторы
    UrgentCargo();
    UrgentCargo(const std::string& number, double cargoMass, 
                const std::string& departure, const std::string& destination,
                const std::string& current, std::time_t arrival, std::time_t deadline);
    UrgentCargo(const UrgentCargo& other);
    UrgentCargo(const Cargo& cargo, std::time_t deadline);
    
    // Оператор присваивания
    UrgentCargo& operator=(const UrgentCargo& other);
    
    // Деструктор
    ~UrgentCargo();
    
    // Геттеры и сеттеры
    std::time_t getDeadline() const;
    void setDeadline(std::time_t deadline);
    
    // Специфичные методы для срочного груза
    bool isOverdue() const;  ///< Проверить, просрочен ли груз
    bool isUrgent() const;   ///< Проверить, является ли груз срочным
    int getDaysUntilDeadline() const;  ///< Получить количество дней до крайнего срока
    
    // Переопределение методов базового класса
    std::string toString() const override;
    bool isValid() const override;
    
    // Операторы сравнения (по массе, как в базовом классе)
    bool operator==(const UrgentCargo& other) const;
    bool operator!=(const UrgentCargo& other) const;
    bool operator<(const UrgentCargo& other) const;
    bool operator<=(const UrgentCargo& other) const;
    bool operator>(const UrgentCargo& other) const;
    bool operator>=(const UrgentCargo& other) const;
};

#endif // URGENT_CARGO_H
