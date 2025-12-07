#include "Cargo.h"
#include <sstream>
#include <iomanip>
#include <ctime>

// Конструктор по умолчанию
Cargo::Cargo() 
    : cargoNumber(""), mass(0.0), departureAirport(""), 
      destinationAirport(""), currentAirport(""), arrivalTime(0) {
}

// Конструктор с параметрами
Cargo::Cargo(const std::string& number, double cargoMass, 
             const std::string& departure, const std::string& destination,
             const std::string& current, std::time_t arrival)
    : cargoNumber(number), mass(cargoMass), departureAirport(departure),
      destinationAirport(destination), currentAirport(current), arrivalTime(arrival) {
}

// Конструктор копирования
Cargo::Cargo(const Cargo& other)
    : cargoNumber(other.cargoNumber), mass(other.mass), 
      departureAirport(other.departureAirport), destinationAirport(other.destinationAirport),
      currentAirport(other.currentAirport), arrivalTime(other.arrivalTime) {
}

// Оператор присваивания
Cargo& Cargo::operator=(const Cargo& other) {
    if (this != &other) {
        cargoNumber = other.cargoNumber;
        mass = other.mass;
        departureAirport = other.departureAirport;
        destinationAirport = other.destinationAirport;
        currentAirport = other.currentAirport;
        arrivalTime = other.arrivalTime;
    }
    return *this;
}

// Деструктор
Cargo::~Cargo() {
    // В данном случае деструктор пустой, так как мы не используем динамическую память
}

// Геттеры
std::string Cargo::getCargoNumber() const {
    return cargoNumber;
}

double Cargo::getMass() const {
    return mass;
}

std::string Cargo::getDepartureAirport() const {
    return departureAirport;
}

std::string Cargo::getDestinationAirport() const {
    return destinationAirport;
}

std::string Cargo::getCurrentAirport() const {
    return currentAirport;
}

std::time_t Cargo::getArrivalTime() const {
    return arrivalTime;
}

// Сеттеры
void Cargo::setCargoNumber(const std::string& number) {
    cargoNumber = number;
}

void Cargo::setMass(double cargoMass) {
    mass = cargoMass;
}

void Cargo::setDepartureAirport(const std::string& departure) {
    departureAirport = departure;
}

void Cargo::setDestinationAirport(const std::string& destination) {
    destinationAirport = destination;
}

void Cargo::setCurrentAirport(const std::string& current) {
    currentAirport = current;
}

void Cargo::setArrivalTime(std::time_t arrival) {
    arrivalTime = arrival;
}

// Операторы сравнения по массе
bool Cargo::operator==(const Cargo& other) const {
    return mass == other.mass;
}

bool Cargo::operator!=(const Cargo& other) const {
    return !(*this == other);
}

bool Cargo::operator<(const Cargo& other) const {
    return mass < other.mass;
}

bool Cargo::operator<=(const Cargo& other) const {
    return mass <= other.mass;
}

bool Cargo::operator>(const Cargo& other) const {
    return mass > other.mass;
}

bool Cargo::operator>=(const Cargo& other) const {
    return mass >= other.mass;
}

// Переместить груз в указанный аэропорт
void Cargo::moveToAirport(const std::string& newAirport) {
    currentAirport = newAirport;
}

// Проверить, достиг ли груз места назначения
bool Cargo::hasReachedDestination() const {
    return currentAirport == destinationAirport;
}

// Получить строковое представление объекта
std::string Cargo::toString() const {
    std::ostringstream oss;  // поток для создания строки явным образом
    oss << "Cargo #" << cargoNumber 
        << " (Mass: " << std::fixed << std::setprecision(2) << mass << " kg)"
        << " [From: " << departureAirport 
        << " To: " << destinationAirport 
        << " Current: " << currentAirport << "]";
    
    // Добавляем время прибытия в читаемом формате
    /*if (arrivalTime > 0) {
        char timeStr[100];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&arrivalTime));
        oss << " Arrived: " << timeStr;
    }*/
    if (arrivalTime > 0) {
        char timeStr[100];
        std::tm tmStruct; 

        localtime_s(&tmStruct, &arrivalTime);

        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tmStruct);
        oss << " Arrived: " << timeStr;
    }

    
    return oss.str();
}

// Проверить корректность данных груза
bool Cargo::isValid() const {
    // Проверяем, что все обязательные поля заполнены
    if (cargoNumber.empty() || departureAirport.empty() || 
        destinationAirport.empty() || currentAirport.empty()) {
        return false;
    }
    
    // Проверяем, что масса положительная
    if (mass <= 0) {
        return false;
    }
    
    // Проверяем, что аэропорты отправления и назначения разные
    if (departureAirport == destinationAirport) {
        return false;
    }
    
    return true;
}
