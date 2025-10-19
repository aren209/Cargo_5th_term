#include "Flight.h"
#include <sstream>
#include <iomanip>
#include <ctime>

// Конструктор по умолчанию
Flight::Flight() 
    : flightNumber(""), departureAirport(""), destinationAirport(""),
      departureTime(0), arrivalTime(0), aircraftId(""), completed(false) {
}

// Конструктор с параметрами
Flight::Flight(const std::string& number, const std::string& departure, 
               const std::string& destination, std::time_t depTime, 
               std::time_t arrTime, const std::string& aircraft)
    : flightNumber(number), departureAirport(departure), destinationAirport(destination),
      departureTime(depTime), arrivalTime(arrTime), aircraftId(aircraft), completed(false) {
}

// Конструктор копирования
Flight::Flight(const Flight& other)
    : flightNumber(other.flightNumber), departureAirport(other.departureAirport),
      destinationAirport(other.destinationAirport), departureTime(other.departureTime),
      arrivalTime(other.arrivalTime), aircraftId(other.aircraftId), completed(other.completed) {
}

// Оператор присваивания
Flight& Flight::operator=(const Flight& other) {
    if (this != &other) {
        flightNumber = other.flightNumber;
        departureAirport = other.departureAirport;
        destinationAirport = other.destinationAirport;
        departureTime = other.departureTime;
        arrivalTime = other.arrivalTime;
        aircraftId = other.aircraftId;
        completed = other.completed;
    }
    return *this;
}

// Деструктор
Flight::~Flight() {
    // Деструктор пустой, так как мы не используем динамическую память
}

// Геттеры
std::string Flight::getFlightNumber() const {
    return flightNumber;
}

std::string Flight::getDepartureAirport() const {
    return departureAirport;
}

std::string Flight::getDestinationAirport() const {
    return destinationAirport;
}

std::time_t Flight::getDepartureTime() const {
    return departureTime;
}

std::time_t Flight::getArrivalTime() const {
    return arrivalTime;
}

std::string Flight::getAircraftId() const {
    return aircraftId;
}

bool Flight::isCompleted() const {
    return completed;
}

// Сеттеры
void Flight::setFlightNumber(const std::string& number) {
    flightNumber = number;
}

void Flight::setDepartureAirport(const std::string& departure) {
    departureAirport = departure;
}

void Flight::setDestinationAirport(const std::string& destination) {
    destinationAirport = destination;
}

void Flight::setDepartureTime(std::time_t depTime) {
    departureTime = depTime;
}

void Flight::setArrivalTime(std::time_t arrTime) {
    arrivalTime = arrTime;
}

void Flight::setAircraftId(const std::string& aircraft) {
    aircraftId = aircraft;
}

void Flight::setCompleted(bool completed) {
    this->completed = completed;
}

// Получить продолжительность рейса в секундах
std::time_t Flight::getFlightDuration() const {
    if (arrivalTime <= departureTime) {
        return 0;
    }
    return arrivalTime - departureTime;
}

// Получить продолжительность рейса в часах
double Flight::getFlightDurationHours() const {
    return static_cast<double>(getFlightDuration()) / 3600.0;
}

// Проверить, идёт ли рейс сейчас
bool Flight::isInProgress() const {
    if (completed) return false;
    
    std::time_t now = std::time(nullptr);
    return now >= departureTime && now <= arrivalTime;
}

// Проверить, запланирован ли рейс
bool Flight::isScheduled() const {
    if (completed) return false;
    
    std::time_t now = std::time(nullptr);
    return now < departureTime;
}

// Завершить рейс
void Flight::completeFlight() {
    completed = true;
}

// Создать обратный рейс
Flight Flight::createReturnFlight() const {
    // Создаём обратный рейс с теми же параметрами, но поменяв местами аэропорты
    Flight returnFlight;
    returnFlight.setFlightNumber(getReturnFlightNumber());
    returnFlight.setDepartureAirport(destinationAirport);
    returnFlight.setDestinationAirport(departureAirport);
    returnFlight.setAircraftId(aircraftId);
    
    // Время обратного рейса рассчитываем как время прибытия + время на подготовку
    // (например, 2 часа на подготовку самолёта)
    std::time_t preparationTime = 2 * 60 * 60; // 2 часа в секундах
    returnFlight.setDepartureTime(arrivalTime + preparationTime);
    
    // Время прибытия обратного рейса рассчитываем как время отправления + продолжительность полёта
    std::time_t flightDuration = getFlightDuration();
    returnFlight.setArrivalTime(returnFlight.getDepartureTime() + flightDuration);
    
    return returnFlight;
}

// Получить номер обратного рейса
std::string Flight::getReturnFlightNumber() const {
    // Простая логика: добавляем "R" к номеру рейса для обратного
    return flightNumber + "R";
}

// Выгрузить грузы и пассажиров по прибытии
void Flight::unloadCargoAndPassengers(std::shared_ptr<Aircraft> aircraft) const {
    if (!aircraft) return;
    
    aircraft->clearAll();
}

// Получить строковое представление объекта
std::string Flight::toString() const {
    std::ostringstream oss;
    oss << "Flight #" << flightNumber << std::endl;
    oss << "  Route: " << departureAirport << " -> " << destinationAirport << std::endl;
    oss << "  Aircraft: " << aircraftId << std::endl;
    
    // Форматируем время отправления
    if (departureTime > 0) {
        char depTimeStr[100];
        std::tm tmStruct;
        localtime_s(&tmStruct, &departureTime);
        std::strftime(depTimeStr, sizeof(depTimeStr), "%Y-%m-%d %H:%M:%S", &tmStruct);
        oss << "  Departure: " << depTimeStr << std::endl;
    }
    
    // Форматируем время прибытия
    if (arrivalTime > 0) {
        char arrTimeStr[100];
        std::tm tmStruct;
        localtime_s(&tmStruct, &arrivalTime);
        std::strftime(arrTimeStr, sizeof(arrTimeStr), "%Y-%m-%d %H:%M:%S", &tmStruct);
        oss << "  Arrival: " << arrTimeStr << std::endl;
    }
    
    oss << "  Duration: " << std::fixed << std::setprecision(2) << getFlightDurationHours() << " hours" << std::endl;
    oss << "  Status: ";
    if (completed) {
        oss << "Completed";
    } else if (isInProgress()) {
        oss << "In Progress";
    } else if (isScheduled()) {
        oss << "Scheduled";
    } else {
        oss << "Unknown";
    }
    oss << std::endl;
    
    return oss.str();
}

// Проверить корректность данных рейса
bool Flight::isValid() const {
    // Проверяем, что все обязательные поля заполнены
    if (flightNumber.empty() || departureAirport.empty() || 
        destinationAirport.empty() || aircraftId.empty()) {
        return false;
    }
    
    // Проверяем, что аэропорты отправления и назначения разные
    if (departureAirport == destinationAirport) {
        return false;
    }
    
    // Проверяем, что время прибытия больше времени отправления
    if (arrivalTime <= departureTime) {
        return false;
    }
    
    return true;
}

// Проверить конфликт с другим рейсом
bool Flight::conflictsWith(const Flight& other) const {
    // Рейсы конфликтуют, если используют один и тот же самолёт
    // и их времена пересекаются
    if (aircraftId != other.aircraftId) {
        return false;  // Разные самолёты - конфликта нет
    }
    
    // Проверяем пересечение временных интервалов
    return !(arrivalTime <= other.departureTime || other.arrivalTime <= departureTime);
}

// Операторы сравнения (по времени отправления)
bool Flight::operator==(const Flight& other) const {
    return departureTime == other.departureTime;
}

bool Flight::operator!=(const Flight& other) const {
    return !(*this == other);
}

bool Flight::operator<(const Flight& other) const {
    return departureTime < other.departureTime;
}

bool Flight::operator<=(const Flight& other) const {
    return departureTime <= other.departureTime;
}

bool Flight::operator>(const Flight& other) const {
    return departureTime > other.departureTime;
}

bool Flight::operator>=(const Flight& other) const {
    return departureTime >= other.departureTime;
}
