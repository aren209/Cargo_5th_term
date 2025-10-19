#include "Passenger.h"
#include <sstream>
#include <iomanip>

// Инициализация статической константы
const double Passenger::FIXED_MASS = 80.0;

// Конструктор по умолчанию
Passenger::Passenger() 
    : passengerNumber(""), name(""), departureAirport(""), destinationAirport("") {
}

// Конструктор с параметрами
Passenger::Passenger(const std::string& number, const std::string& passengerName,
                    const std::string& departure, const std::string& destination)
    : passengerNumber(number), name(passengerName), 
      departureAirport(departure), destinationAirport(destination) {
}

// Конструктор копирования
Passenger::Passenger(const Passenger& other)
    : passengerNumber(other.passengerNumber), name(other.name),
      departureAirport(other.departureAirport), destinationAirport(other.destinationAirport) {
}

// Оператор присваивания
Passenger& Passenger::operator=(const Passenger& other) {
    if (this != &other) {
        passengerNumber = other.passengerNumber;
        name = other.name;
        departureAirport = other.departureAirport;
        destinationAirport = other.destinationAirport;
    }
    return *this;
}

// Деструктор
Passenger::~Passenger() {
    // Деструктор пустой, так как мы не используем динамическую память
}

// Геттеры
std::string Passenger::getPassengerNumber() const {
    return passengerNumber;
}

std::string Passenger::getName() const {
    return name;
}

std::string Passenger::getDepartureAirport() const {
    return departureAirport;
}

std::string Passenger::getDestinationAirport() const {
    return destinationAirport;
}

double Passenger::getMass() const {
    return FIXED_MASS;
}

// Сеттеры
void Passenger::setPassengerNumber(const std::string& number) {
    passengerNumber = number;
}

void Passenger::setName(const std::string& passengerName) {
    name = passengerName;
}

void Passenger::setDepartureAirport(const std::string& departure) {
    departureAirport = departure;
}

void Passenger::setDestinationAirport(const std::string& destination) {
    destinationAirport = destination;
}

// Проверить, достиг ли пассажир места назначения
bool Passenger::hasReachedDestination() const {
    // В данном контексте предполагаем, что пассажир достиг места назначения,
    // если аэропорт назначения совпадает с текущим местоположением
    // (это будет определяться в контексте рейса)
    return false;  // По умолчанию пассажир не достиг места назначения
}

// Получить строковое представление объекта
std::string Passenger::toString() const {
    std::ostringstream oss;
    oss << "Passenger #" << passengerNumber 
        << " (" << name << ")"
        << " [From: " << departureAirport 
        << " To: " << destinationAirport 
        << " Mass: " << std::fixed << std::setprecision(1) << FIXED_MASS << " kg]";
    
    return oss.str();
}

// Проверить корректность данных пассажира
bool Passenger::isValid() const {
    // Проверяем, что все обязательные поля заполнены
    if (passengerNumber.empty() || name.empty() || 
        departureAirport.empty() || destinationAirport.empty()) {
        return false;
    }
    
    // Проверяем, что аэропорты отправления и назначения разные
    if (departureAirport == destinationAirport) {
        return false;
    }
    
    return true;
}

// Операторы сравнения (по массе)
bool Passenger::operator==(const Passenger& other) const {
    return FIXED_MASS == other.FIXED_MASS;  // Все пассажиры имеют одинаковую массу
}

bool Passenger::operator!=(const Passenger& other) const {
    return !(*this == other);
}

bool Passenger::operator<(const Passenger& other) const {
    return FIXED_MASS < other.FIXED_MASS;  // Все пассажиры имеют одинаковую массу, поэтому всегда false
}

bool Passenger::operator<=(const Passenger& other) const {
    return FIXED_MASS <= other.FIXED_MASS;  // Все пассажиры имеют одинаковую массу, поэтому всегда true
}

bool Passenger::operator>(const Passenger& other) const {
    return FIXED_MASS > other.FIXED_MASS;  // Все пассажиры имеют одинаковую массу, поэтому всегда false
}

bool Passenger::operator>=(const Passenger& other) const {
    return FIXED_MASS >= other.FIXED_MASS;  // Все пассажиры имеют одинаковую массу, поэтому всегда true
}
