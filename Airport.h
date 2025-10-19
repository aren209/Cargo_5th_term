#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <vector>
#include <memory>
#include "Cargo.h"
#include "UrgentCargo.h"
#include "Passenger.h"

// Предварительное объявление класса Aircraft
class Aircraft;

class Airport {
private:
    std::string name;                                    ///< Название аэропорта
    std::vector<std::shared_ptr<Cargo>> cargoList;      ///< Список грузов в аэропорту
    std::vector<std::shared_ptr<UrgentCargo>> urgentCargoList; ///< Список срочных грузов
    std::vector<std::shared_ptr<Passenger>> passengerList;     ///< Список пассажиров
    std::vector<std::shared_ptr<Aircraft>> aircraftList; ///< Список самолётов в аэропорту

public:
    // Конструкторы
    Airport();
    Airport(const std::string& airportName);
    Airport(const Airport& other);
    
    // Оператор присваивания
    Airport& operator=(const Airport& other);
    
    // Деструктор
    ~Airport();
    
    // Геттеры
    std::string getName() const;
    const std::vector<std::shared_ptr<Cargo>>& getCargoList() const;
    const std::vector<std::shared_ptr<UrgentCargo>>& getUrgentCargoList() const;
    const std::vector<std::shared_ptr<Passenger>>& getPassengerList() const;
    const std::vector<std::shared_ptr<Aircraft>>& getAircraftList() const;
    
    // Сеттеры
    void setName(const std::string& airportName);
    
    // Методы для работы с грузами
    void addCargo(std::shared_ptr<Cargo> cargo);
    void addUrgentCargo(std::shared_ptr<UrgentCargo> urgentCargo);
    void removeCargo(const std::string& cargoNumber);
    void removeUrgentCargo(const std::string& cargoNumber);
    std::shared_ptr<Cargo> findCargo(const std::string& cargoNumber) const;
    std::shared_ptr<UrgentCargo> findUrgentCargo(const std::string& cargoNumber) const;
    
    // Методы для работы с пассажирами
    void addPassenger(std::shared_ptr<Passenger> passenger);
    void removePassenger(const std::string& passengerNumber);
    std::shared_ptr<Passenger> findPassenger(const std::string& passengerNumber) const;
    
    // Методы для работы с самолётами
    void addAircraft(std::shared_ptr<Aircraft> aircraft);
    void removeAircraft(const std::string& aircraftNumber);
    std::shared_ptr<Aircraft> findAircraft(const std::string& aircraftNumber) const;
    
    // Общие методы
    double getTotalCargoWeight() const;  ///< Получить общий вес всех грузов
    int getTotalPassengerCount() const;  ///< Получить общее количество пассажиров
    int getTotalAircraftCount() const;   ///< Получить общее количество самолётов
    std::string toString() const;        ///< Получить строковое представление
    bool isValid() const;                 ///< Проверить корректность данных
    
    // Методы для поиска
    std::vector<std::shared_ptr<UrgentCargo>> getOverdueCargo() const;  ///< Получить просроченные срочные грузы
    std::vector<std::shared_ptr<UrgentCargo>> getUrgentCargo() const;   ///< Получить срочные грузы
};

#endif // AIRPORT_H
