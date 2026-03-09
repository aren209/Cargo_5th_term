//! \file Cargo.h
//! \brief Базовый класс груза в системе авиаперевозок

#ifndef CARGO_H
#define CARGO_H

#include <string>
#include <ctime>


//! Базовый класс для представления груза (идентификатор, масса, аэропорты, время прибытия).
class Cargo {
private:
    std::string cargoNumber;        ///< Строковый идентификатор груза (например, "C-001"; не обязательно число)
    double mass;                    ///< Масса груза в килограммах
    std::string departureAirport;   ///< Аэропорт отправления
    std::string destinationAirport; ///< Аэропорт назначения
    std::string currentAirport;     ///< Текущее местоположение груза
    std::time_t arrivalTime;        ///< Время прибытия в аэропорт отправления

public:
    /**
     * \brief Создать пустой объект груза.
     *
     * Используется в ситуациях, когда поля будут заполнены позднее сеттерами.
     */
    Cargo();

    /**
     * \brief Создать груз с заданными параметрами.
     *
     * \param number Строковый идентификатор груза (например, "C-001");
     *               значение не обязано быть числом, допускаются буквы и другие символы.
     * \param cargoMass Масса груза в килограммах.
     * \param departure Аэропорт отправления.
     * \param destination Аэропорт назначения.
     * \param current Текущее местоположение груза.
     * \param arrival Время прибытия груза в аэропорт назначения (std::time_t, Unix time).
     */
    Cargo(const std::string& number, double cargoMass, 
          const std::string& departure, const std::string& destination,
          const std::string& current, std::time_t arrival);
    Cargo(const Cargo& other);
    Cargo& operator=(const Cargo& other);
    ~Cargo();    ///деструктор

    // Геттеры
    std::string getCargoNumber() const;
    double getMass() const;
    std::string getDepartureAirport() const;
    std::string getDestinationAirport() const;
    std::string getCurrentAirport() const;
    std::time_t getArrivalTime() const;

    // Сеттеры
    void setCargoNumber(const std::string& number);
    void setMass(double cargoMass);
    void setDepartureAirport(const std::string& departure);
    void setDestinationAirport(const std::string& destination);
    void setCurrentAirport(const std::string& current);
    void setArrivalTime(std::time_t arrival);

    // Операторы сравнения по массе
    bool operator==(const Cargo& other) const;
    bool operator!=(const Cargo& other) const;
    bool operator<(const Cargo& other) const;
    bool operator<=(const Cargo& other) const;
    bool operator>(const Cargo& other) const;
    bool operator>=(const Cargo& other) const;

    void moveToAirport(const std::string& newAirport);

    bool hasReachedDestination() const;

    virtual std::string toString() const;

    virtual bool isValid() const;
};

#endif // CARGO_H
