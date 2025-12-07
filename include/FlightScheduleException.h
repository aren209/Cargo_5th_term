#ifndef FLIGHT_SCHEDULE_EXCEPTION_H
#define FLIGHT_SCHEDULE_EXCEPTION_H

#include <string>
#include <exception>

/**
 * @class FlightScheduleException
 * @brief Базовый класс исключений для системы расписания полётов
 * 
 * Используется для обработки ошибок, возникающих при работе с расписанием,
 * самолётами, рейсами и другими компонентами системы.
 */
class FlightScheduleException : public std::exception {
private:
    std::string message;  ///< Сообщение об ошибке

public:
    /**
     * @brief Конструктор исключения
     * @param msg Сообщение об ошибке
     */
    explicit FlightScheduleException(const std::string& msg) : message(msg) {}
    
    /**
     * @brief Конструктор исключения
     * @param msg Сообщение об ошибке (C-строка)
     */
    explicit FlightScheduleException(const char* msg) : message(msg) {}
    
    /**
     * @brief Деструктор
     */
    virtual ~FlightScheduleException() noexcept = default;
    
    /**
     * @brief Получить сообщение об ошибке
     * @return C-строка с сообщением об ошибке
     */
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
    
    /**
     * @brief Получить сообщение об ошибке как строку
     * @return Строка с сообщением об ошибке
     */
    std::string getMessage() const {
        return message;
    }
};

/**
 * @class InvalidAircraftException
 * @brief Исключение для ошибок, связанных с некорректными данными самолёта
 * 
 * Выбрасывается при попытке создать самолёт с некорректными параметрами,
 * например, с отрицательной или нулевой грузоподъёмностью.
 */
class InvalidAircraftException : public FlightScheduleException {
public:
    /**
     * @brief Конструктор исключения
     * @param msg Сообщение об ошибке
     */
    explicit InvalidAircraftException(const std::string& msg) 
        : FlightScheduleException("InvalidAircraftException: " + msg) {}
    
    /**
     * @brief Конструктор исключения
     * @param msg Сообщение об ошибке (C-строка)
     */
    explicit InvalidAircraftException(const char* msg) 
        : FlightScheduleException(std::string("InvalidAircraftException: ") + msg) {}
};

#endif // FLIGHT_SCHEDULE_EXCEPTION_H

