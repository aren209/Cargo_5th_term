//! \file Aircraft.h
//! \brief Класс самолёта: грузоподъёмность, списки грузов и пассажиров.

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <vector>
#include <memory>
#include "Cargo.h"
#include "UrgentCargo.h"
#include "Passenger.h"


//! Самолёт: номер, максимальная грузоподъёмность, списки грузов (обычных и срочных) и пассажиров.
class Aircraft {
private:
    std::string aircraftNumber;                          ///< Номер самолёта
    double maxPayload;                                  ///< Максимальная грузоподъёмность в кг
    std::vector<std::shared_ptr<Cargo>> cargoList;      ///< Список грузов на борту
    std::vector<std::shared_ptr<UrgentCargo>> urgentCargoList; ///< Список срочных грузов на борту
    std::vector<std::shared_ptr<Passenger>> passengerList;     ///< Список пассажиров на борту

public:
    // Конструкторы
    Aircraft();
    Aircraft(const std::string& number, double maxPayload);
    Aircraft(const Aircraft& other);
    
    // Оператор присваивания
    Aircraft& operator=(const Aircraft& other);
    
    // Деструктор
    ~Aircraft();
    
    // Геттеры
    std::string getAircraftNumber() const;
    double getMaxPayload() const;
    double getCurrentPayload() const;  ///< Получить текущую загрузку
    const std::vector<std::shared_ptr<Cargo>>& getCargoList() const;
    const std::vector<std::shared_ptr<UrgentCargo>>& getUrgentCargoList() const;
    const std::vector<std::shared_ptr<Passenger>>& getPassengerList() const;
    
    // Сеттеры
    void setAircraftNumber(const std::string& number);
    void setMaxPayload(double maxPayload);
    
    // Методы для работы с грузами
    bool addCargo(std::shared_ptr<Cargo> cargo);        ///< Добавить груз (возвращает true если успешно)
    bool addUrgentCargo(std::shared_ptr<UrgentCargo> urgentCargo); ///< Добавить срочный груз
    void removeCargo(const std::string& cargoNumber);
    void removeUrgentCargo(const std::string& cargoNumber);
    std::shared_ptr<Cargo> findCargo(const std::string& cargoNumber) const;
    std::shared_ptr<UrgentCargo> findUrgentCargo(const std::string& cargoNumber) const;
    
    // Методы для работы с пассажирами
    bool addPassenger(std::shared_ptr<Passenger> passenger); ///< Добавить пассажира
    void removePassenger(const std::string& passengerNumber);
    std::shared_ptr<Passenger> findPassenger(const std::string& passengerNumber) const;
    
    // Общие методы
    bool canCarry(double additionalWeight) const;       ///< Проверить, может ли самолёт взять дополнительный груз
    double getAvailableCapacity() const;                ///< Получить доступную грузоподъёмность
    int getTotalPassengerCount() const;                 ///< Получить общее количество пассажиров
    int getTotalCargoCount() const;                     ///< Получить общее количество грузов
    void clearAll();                                    ///< Очистить все грузы и пассажиров
    std::string toString() const;                       ///< Получить строковое представление
    bool isValid() const;                               ///< Проверить корректность данных
    
    // Методы для поиска
    std::vector<std::shared_ptr<UrgentCargo>> getOverdueCargo() const;  ///< Получить просроченные срочные грузы
    std::vector<std::shared_ptr<UrgentCargo>> getUrgentCargo() const;   ///< Получить срочные грузы
};

#endif // AIRCRAFT_H
