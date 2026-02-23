//! \file Flight.h
//! \brief Класс рейса: маршрут, время, самолёт, статус.

#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <ctime>
#include <memory>
#include "Aircraft.h"

//! Рейс: номер, аэропорты отправления/назначения, время вылета/прилёта, ID самолёта, флаг завершения.
class Flight {
private:
    std::string flightNumber;           ///< Номер рейса
    std::string departureAirport;       ///< Аэропорт отправления
    std::string destinationAirport;     ///< Аэропорт назначения
    std::time_t departureTime;          ///< Время отбытия
    std::time_t arrivalTime;            ///< Время прибытия
    std::string aircraftId;             ///< Идентификатор самолёта
    bool completed;                     ///< Флаг завершения рейса

public:
    // Конструкторы
    Flight();
    Flight(const std::string& number, const std::string& departure, 
           const std::string& destination, std::time_t depTime, 
           std::time_t arrTime, const std::string& aircraft);
    Flight(const Flight& other);
    
    // Оператор присваивания
    Flight& operator=(const Flight& other);
    
    // Деструктор
    ~Flight();
    
    // Геттеры
    std::string getFlightNumber() const;
    std::string getDepartureAirport() const;
    std::string getDestinationAirport() const;
    std::time_t getDepartureTime() const;
    std::time_t getArrivalTime() const;
    std::string getAircraftId() const;
    bool isCompleted() const;
    
    // Сеттеры
    void setFlightNumber(const std::string& number);
    void setDepartureAirport(const std::string& departure);
    void setDestinationAirport(const std::string& destination);
    void setDepartureTime(std::time_t depTime);
    void setArrivalTime(std::time_t arrTime);
    void setAircraftId(const std::string& aircraft);
    void setCompleted(bool completed);
    
    // Основные методы
    std::time_t getFlightDuration() const;              ///< Получить продолжительность рейса в секундах
    double getFlightDurationHours() const;              ///< Получить продолжительность рейса в часах
    bool isInProgress() const;                          ///< Проверить, идёт ли рейс сейчас
    bool isScheduled() const;                           ///< Проверить, запланирован ли рейс
    void completeFlight();                               ///< Завершить рейс
    
    // Методы для работы с обратным рейсом
    Flight createReturnFlight() const;                  ///< Создать обратный рейс
    std::string getReturnFlightNumber() const;          ///< Получить номер обратного рейса
    
    // Методы для работы с грузами и пассажирами
    void unloadCargoAndPassengers(std::shared_ptr<Aircraft> aircraft) const; ///< Выгрузить грузы и пассажиров по прибытии
    
    // Общие методы
    std::string toString() const;                       ///< Получить строковое представление
    bool isValid() const;                               ///< Проверить корректность данных
    bool conflictsWith(const Flight& other) const;      ///< Проверить конфликт с другим рейсом
    
    // Операторы сравнения (по времени отправления)
    bool operator==(const Flight& other) const;
    bool operator!=(const Flight& other) const;
    bool operator<(const Flight& other) const;
    bool operator<=(const Flight& other) const;
    bool operator>(const Flight& other) const;
    bool operator>=(const Flight& other) const;
};

#endif // FLIGHT_H
