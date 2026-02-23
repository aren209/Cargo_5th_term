//! \file Passenger.h
//! \brief Класс пассажира: номер, имя, аэропорты; фиксированная масса.

#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <ctime>

//! Пассажир: номер, имя, аэропорты отправления/назначения; масса 80 кг.
class Passenger {
private:
    std::string passengerNumber;     ///< Номер пассажира
    std::string name;               ///< Имя пассажира
    std::string departureAirport;   ///< Аэропорт отправления
    std::string destinationAirport; ///< Аэропорт назначения
    static const double FIXED_MASS;  ///< Фиксированная масса пассажира и багажа (80 кг)

public:
    // Конструкторы
    Passenger();
    Passenger(const std::string& number, const std::string& passengerName,
              const std::string& departure, const std::string& destination);
    Passenger(const Passenger& other);
    
    // Оператор присваивания
    Passenger& operator=(const Passenger& other);
    
    // Деструктор
    ~Passenger();
    
    // Геттеры
    std::string getPassengerNumber() const;
    std::string getName() const;
    std::string getDepartureAirport() const;
    std::string getDestinationAirport() const;
    double getMass() const;  ///< Возвращает фиксированную массу 80 кг
    
    // Сеттеры
    void setPassengerNumber(const std::string& number);
    void setName(const std::string& passengerName);
    void setDepartureAirport(const std::string& departure);
    void setDestinationAirport(const std::string& destination);
    
    // Методы
    bool hasReachedDestination() const;  ///< Проверить, достиг ли пассажир места назначения
    std::string toString() const;        ///< Получить строковое представление
    bool isValid() const;                 ///< Проверить корректность данных
    
    // Операторы сравнения (по массе)
    bool operator==(const Passenger& other) const;
    bool operator!=(const Passenger& other) const;
    bool operator<(const Passenger& other) const;
    bool operator<=(const Passenger& other) const;
    bool operator>(const Passenger& other) const;
    bool operator>=(const Passenger& other) const;
};

#endif // PASSENGER_H
