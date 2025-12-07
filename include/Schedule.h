#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include <memory>
#include <string>
#include <ctime>
#include "Flight.h"

class Schedule {
private:
    std::vector<std::shared_ptr<Flight>> flights;  ///< Список рейсов (упорядоченный по времени отправления)
    
    // Вспомогательные методы
    void sortFlights();                              ///< Отсортировать рейсы по времени отправления
    bool hasConflicts() const;                       ///< Проверить наличие конфликтов в расписании

public:
    // Конструкторы
    Schedule();
    Schedule(const Schedule& other);
    
    // Оператор присваивания
    Schedule& operator=(const Schedule& other);
    
    // Деструктор
    ~Schedule();
    
    // Основные методы
    void addFlight(std::shared_ptr<Flight> flight);  ///< Добавить рейс в расписание
    void removeFlight(const std::string& flightNumber); ///< Удалить рейс из расписания
    std::shared_ptr<Flight> findFlight(const std::string& flightNumber) const; ///< Найти рейс по номеру
    
    // Методы для работы с расписанием
    bool isValid() const;                            ///< Проверить корректность расписания
    std::vector<std::string> getValidationErrors() const; ///< Получить список ошибок валидации
    void validateAndFix();                          ///< Проверить и исправить ошибки в расписании
    
    // Методы для получения информации
    const std::vector<std::shared_ptr<Flight>>& getFlights() const; ///< Получить список всех рейсов
    std::vector<std::shared_ptr<Flight>> getFlightsByAircraft(const std::string& aircraftId) const; ///< Получить рейсы самолёта
    std::vector<std::shared_ptr<Flight>> getFlightsByAirport(const std::string& airportCode) const; ///< Получить рейсы аэропорта
    std::vector<std::shared_ptr<Flight>> getFlightsInTimeRange(std::time_t startTime, std::time_t endTime) const; ///< Получить рейсы в временном диапазоне
    
    // Методы для анализа времени полётов
    double getTotalFlightTime(const std::string& aircraftId) const; ///< Получить общее время полётов самолёта
    double getTotalFlightTimeInRange(const std::string& aircraftId, std::time_t startTime, std::time_t endTime) const; ///< Получить время полётов в диапазоне
    std::vector<std::string> getOverworkedAircraft() const; ///< Получить список перегруженных самолётов
    
    // Методы для вывода расписания
    std::string toString() const;                    ///< Получить строковое представление всего расписания
    std::string getScheduleForAircraft(const std::string& aircraftId) const; ///< Получить расписание для самолёта
    std::string getScheduleForAircraftInRange(const std::string& aircraftId, std::time_t startTime, std::time_t endTime) const; ///< Получить расписание самолёта в диапазоне
    std::string getOverdueUrgentCargoReport() const; ///< Получить отчёт о просроченных срочных грузах
    
    // Методы для работы с рейсами
    void completeFlight(const std::string& flightNumber); ///< Завершить рейс
    void addReturnFlights();                        ///< Добавить обратные рейсы для всех прямых рейсов
    
    // Статистические методы
    int getTotalFlights() const;                    ///< Получить общее количество рейсов
    int getCompletedFlights() const;                ///< Получить количество завершённых рейсов
    int getScheduledFlights() const;                ///< Получить количество запланированных рейсов
    int getInProgressFlights() const;               ///< Получить количество рейсов в процессе
};

#endif // SCHEDULE_H
