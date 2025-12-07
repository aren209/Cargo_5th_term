#include "Airport.h"
#include "Aircraft.h"  // Включаем после предварительного объявления
#include <sstream>
#include <algorithm>
#include <iomanip>

// Конструктор по умолчанию
Airport::Airport() : name("") {
}

// Конструктор с параметрами
Airport::Airport(const std::string& airportName) : name(airportName) {
}

// Конструктор копирования
Airport::Airport(const Airport& other) 
    : name(other.name), cargoList(other.cargoList), 
      urgentCargoList(other.urgentCargoList), passengerList(other.passengerList),
      aircraftList(other.aircraftList) {
}

// Оператор присваивания
Airport& Airport::operator=(const Airport& other) {
    if (this != &other) {
        name = other.name;
        cargoList = other.cargoList;
        urgentCargoList = other.urgentCargoList;
        passengerList = other.passengerList;
        aircraftList = other.aircraftList;
    }
    return *this;
}

// Деструктор
Airport::~Airport() {
    // Деструктор пустой, так как мы используем shared_ptr
}

// Геттеры
std::string Airport::getName() const {
    return name;
}

const std::vector<std::shared_ptr<Cargo>>& Airport::getCargoList() const {
    return cargoList;
}

const std::vector<std::shared_ptr<UrgentCargo>>& Airport::getUrgentCargoList() const {
    return urgentCargoList;
}

const std::vector<std::shared_ptr<Passenger>>& Airport::getPassengerList() const {
    return passengerList;
}

const std::vector<std::shared_ptr<Aircraft>>& Airport::getAircraftList() const {
    return aircraftList;
}

// Сеттеры
void Airport::setName(const std::string& airportName) {
    name = airportName;
}

// Методы для работы с грузами
void Airport::addCargo(std::shared_ptr<Cargo> cargo) {
    if (cargo && cargo->isValid()) {
        cargoList.push_back(cargo);
    }
}

void Airport::addUrgentCargo(std::shared_ptr<UrgentCargo> urgentCargo) {
    if (urgentCargo && urgentCargo->isValid()) {
        urgentCargoList.push_back(urgentCargo);
    }
}

void Airport::removeCargo(const std::string& cargoNumber) {
    cargoList.erase(
        std::remove_if(cargoList.begin(), cargoList.end(),
            [&cargoNumber](const std::shared_ptr<Cargo>& cargo) {
                return cargo && cargo->getCargoNumber() == cargoNumber;
            }),
        cargoList.end()
    );
}

void Airport::removeUrgentCargo(const std::string& cargoNumber) {
    urgentCargoList.erase(
        std::remove_if(urgentCargoList.begin(), urgentCargoList.end(),
            [&cargoNumber](const std::shared_ptr<UrgentCargo>& cargo) {
                return cargo && cargo->getCargoNumber() == cargoNumber;
            }),
        urgentCargoList.end()
    );
}

std::shared_ptr<Cargo> Airport::findCargo(const std::string& cargoNumber) const {
    auto it = std::find_if(cargoList.begin(), cargoList.end(),
        [&cargoNumber](const std::shared_ptr<Cargo>& cargo) {
            return cargo && cargo->getCargoNumber() == cargoNumber;
        });
    
    return (it != cargoList.end()) ? *it : nullptr;
}

std::shared_ptr<UrgentCargo> Airport::findUrgentCargo(const std::string& cargoNumber) const {
    auto it = std::find_if(urgentCargoList.begin(), urgentCargoList.end(),
        [&cargoNumber](const std::shared_ptr<UrgentCargo>& cargo) {
            return cargo && cargo->getCargoNumber() == cargoNumber;
        });
    
    return (it != urgentCargoList.end()) ? *it : nullptr;
}

// Методы для работы с пассажирами
void Airport::addPassenger(std::shared_ptr<Passenger> passenger) {
    if (passenger && passenger->isValid()) {
        passengerList.push_back(passenger);
    }
}

void Airport::removePassenger(const std::string& passengerNumber) {
    passengerList.erase(
        std::remove_if(passengerList.begin(), passengerList.end(),
            [&passengerNumber](const std::shared_ptr<Passenger>& passenger) {
                return passenger && passenger->getPassengerNumber() == passengerNumber;
            }),
        passengerList.end()
    );
}

std::shared_ptr<Passenger> Airport::findPassenger(const std::string& passengerNumber) const {
    auto it = std::find_if(passengerList.begin(), passengerList.end(),
        [&passengerNumber](const std::shared_ptr<Passenger>& passenger) {
            return passenger && passenger->getPassengerNumber() == passengerNumber;
        });
    
    return (it != passengerList.end()) ? *it : nullptr;
}

// Методы для работы с самолётами
void Airport::addAircraft(std::shared_ptr<Aircraft> aircraft) {
    if (aircraft) {
        aircraftList.push_back(aircraft);
    }
}

void Airport::removeAircraft(const std::string& aircraftNumber) {
    aircraftList.erase(
        std::remove_if(aircraftList.begin(), aircraftList.end(),
            [&aircraftNumber](const std::shared_ptr<Aircraft>& aircraft) {
                return aircraft && aircraft->getAircraftNumber() == aircraftNumber;
            }),
        aircraftList.end()
    );
}

std::shared_ptr<Aircraft> Airport::findAircraft(const std::string& aircraftNumber) const {
    auto it = std::find_if(aircraftList.begin(), aircraftList.end(),
        [&aircraftNumber](const std::shared_ptr<Aircraft>& aircraft) {
            return aircraft && aircraft->getAircraftNumber() == aircraftNumber;
        });
    
    return (it != aircraftList.end()) ? *it : nullptr;
}

// Получить общий вес всех грузов
double Airport::getTotalCargoWeight() const {
    double totalWeight = 0.0;
    
    // Добавляем вес обычных грузов
    for (const auto& cargo : cargoList) {
        if (cargo) {
            totalWeight += cargo->getMass();
        }
    }
    
    // Добавляем вес срочных грузов
    for (const auto& urgentCargo : urgentCargoList) {
        if (urgentCargo) {
            totalWeight += urgentCargo->getMass();
        }
    }
    
    // Добавляем вес пассажиров
    for (const auto& passenger : passengerList) {
        if (passenger) {
            totalWeight += passenger->getMass();
        }
    }
    
    return totalWeight;
}

// Получить общее количество пассажиров
int Airport::getTotalPassengerCount() const {
    return static_cast<int>(passengerList.size());
}

// Получить общее количество самолётов
int Airport::getTotalAircraftCount() const {
    return static_cast<int>(aircraftList.size());
}

// Получить строковое представление объекта
std::string Airport::toString() const {
    std::ostringstream oss;
    oss << "Airport: " << name << std::endl;
    oss << "  Cargo: " << cargoList.size() << " items" << std::endl;
    oss << "  Urgent Cargo: " << urgentCargoList.size() << " items" << std::endl;
    oss << "  Passengers: " << passengerList.size() << " people" << std::endl;
    oss << "  Aircraft: " << aircraftList.size() << " planes" << std::endl;
    oss << "  Total Weight: " << std::fixed << std::setprecision(2) << getTotalCargoWeight() << " kg" << std::endl;
    
    return oss.str();
}

// Проверить корректность данных аэропорта
bool Airport::isValid() const {
    return !name.empty();
}

// Получить просроченные срочные грузы
std::vector<std::shared_ptr<UrgentCargo>> Airport::getOverdueCargo() const {
    std::vector<std::shared_ptr<UrgentCargo>> overdueCargo;
    
    for (const auto& urgentCargo : urgentCargoList) {
        if (urgentCargo && urgentCargo->isOverdue()) {
            overdueCargo.push_back(urgentCargo);
        }
    }
    
    return overdueCargo;
}

// Получить срочные грузы
std::vector<std::shared_ptr<UrgentCargo>> Airport::getUrgentCargo() const {
    std::vector<std::shared_ptr<UrgentCargo>> urgentCargo;
    
    for (const auto& cargo : urgentCargoList) {
        if (cargo && cargo->isUrgent()) {
            urgentCargo.push_back(cargo);
        }
    }
    
    return urgentCargo;
}
