#include "UrgentCargo.h"
#include <sstream>
#include <iomanip>
#include <ctime>

// Конструктор по умолчанию
UrgentCargo::UrgentCargo() : Cargo(), deadline(0) {
}

// Конструктор с параметрами
UrgentCargo::UrgentCargo(const std::string& number, double cargoMass, 
                        const std::string& departure, const std::string& destination,
                        const std::string& current, std::time_t arrival, std::time_t deadline)
    : Cargo(number, cargoMass, departure, destination, current, arrival), deadline(deadline) {
}

// Конструктор копирования
UrgentCargo::UrgentCargo(const UrgentCargo& other) 
    : Cargo(other), deadline(other.deadline) {
}

// Конструктор из обычного груза с добавлением крайнего срока
UrgentCargo::UrgentCargo(const Cargo& cargo, std::time_t deadline)
    : Cargo(cargo), deadline(deadline) {
}

// Оператор присваивания
UrgentCargo& UrgentCargo::operator=(const UrgentCargo& other) {
    if (this != &other) {
        Cargo::operator=(other);  // Вызываем оператор присваивания базового класса
        deadline = other.deadline;
    }
    return *this;
}

// Деструктор
UrgentCargo::~UrgentCargo() {
    // Деструктор пустой, так как мы не используем динамическую память
}

// Геттеры и сеттеры
std::time_t UrgentCargo::getDeadline() const {
    return deadline;
}

void UrgentCargo::setDeadline(std::time_t deadline) {
    this->deadline = deadline;
}

// Проверить, просрочен ли груз
bool UrgentCargo::isOverdue() const {
    if (deadline == 0) return false;  // Если крайний срок не установлен
    
    std::time_t now = std::time(nullptr);
    return now > deadline;
}

// Проверить, является ли груз срочным
bool UrgentCargo::isUrgent() const {
    if (deadline == 0) return false;
    
    std::time_t now = std::time(nullptr);
    // Считаем груз срочным, если до крайнего срока осталось менее 24 часов
    return (deadline - now) < 24 * 60 * 60;
}

// Получить количество дней до крайнего срока
int UrgentCargo::getDaysUntilDeadline() const {
    if (deadline == 0) return -1;  // Если крайний срок не установлен
    
    std::time_t now = std::time(nullptr);
    std::time_t diff = deadline - now;
    
    if (diff < 0) return 0;  // Уже просрочен
    
    return static_cast<int>(diff / (24 * 60 * 60));
}

// Переопределение метода toString
std::string UrgentCargo::toString() const {
    std::ostringstream oss;
    oss << "URGENT " << Cargo::toString();  // Добавляем префикс "URGENT"
    
    if (deadline > 0) {
        char timeStr[100];
        std::tm tmStruct;
        localtime_s(&tmStruct, &deadline);
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tmStruct);
        oss << " [Deadline: " << timeStr << "]";
        
        if (isOverdue()) {
            oss << " [OVERDUE!]";
        } else if (isUrgent()) {
            oss << " [URGENT!]";
        }
    }
    
    return oss.str();
}

// Переопределение метода isValid
bool UrgentCargo::isValid() const {
    // Сначала проверяем базовую валидность
    if (!Cargo::isValid()) {
        return false;
    }
    
    // Дополнительные проверки для срочного груза
    if (deadline == 0) {
        return false;  // Крайний срок должен быть установлен
    }
    
    // Крайний срок должен быть в будущем (при создании)
    std::time_t now = std::time(nullptr);
    if (deadline <= now) {
        return false;
    }
    
    return true;
}

// Операторы сравнения (по массе, как в базовом классе)
bool UrgentCargo::operator==(const UrgentCargo& other) const {
    return Cargo::operator==(other) && deadline == other.deadline;
}

bool UrgentCargo::operator!=(const UrgentCargo& other) const {
    return !(*this == other);
}

bool UrgentCargo::operator<(const UrgentCargo& other) const {
    // Сначала сравниваем по массе, затем по крайнему сроку
    if (Cargo::operator<(other)) return true;
    if (Cargo::operator>(other)) return false;
    return deadline < other.deadline;
}

bool UrgentCargo::operator<=(const UrgentCargo& other) const {
    return (*this < other) || (*this == other);
}

bool UrgentCargo::operator>(const UrgentCargo& other) const {
    return !(*this <= other);
}

bool UrgentCargo::operator>=(const UrgentCargo& other) const {
    return !(*this < other);
}
