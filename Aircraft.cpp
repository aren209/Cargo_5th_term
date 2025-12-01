#include "Aircraft.h"
#include <sstream>
#include <algorithm>
#include <iomanip>

// Конструктор по умолчанию
Aircraft::Aircraft() : aircraftNumber(""), maxPayload(0.0) {
}

// Конструктор с параметрами
Aircraft::Aircraft(const std::string& number, double maxPayload) 
    : aircraftNumber(number), maxPayload(maxPayload) {
}

// Конструктор копирования
Aircraft::Aircraft(const Aircraft& other)
    : aircraftNumber(other.aircraftNumber), maxPayload(other.maxPayload),
      cargoList(other.cargoList), urgentCargoList(other.urgentCargoList),
      passengerList(other.passengerList) {
}

// Оператор присваивания
Aircraft& Aircraft::operator=(const Aircraft& other) {
    if (this != &other) {
        aircraftNumber = other.aircraftNumber;
        maxPayload = other.maxPayload;
        cargoList = other.cargoList;
        urgentCargoList = other.urgentCargoList;
        passengerList = other.passengerList;
    }
    return *this;
}

// Деструктор
Aircraft::~Aircraft() {
    // Деструктор пустой, так как мы используем shared_ptr
}

// Геттеры
std::string Aircraft::getAircraftNumber() const {
    return aircraftNumber;
}

double Aircraft::getMaxPayload() const {
    return maxPayload;
}

double Aircraft::getCurrentPayload() const {
    double currentWeight = 0.0;
    
    // Добавляем вес обычных грузов
    for (const auto& cargo : cargoList) {
        if (cargo) {
            currentWeight += cargo->getMass();
        }
    }
    
    // Добавляем вес срочных грузов
    for (const auto& urgentCargo : urgentCargoList) {
        if (urgentCargo) {
            currentWeight += urgentCargo->getMass();
        }
    }
    
    // Добавляем вес пассажиров
    for (const auto& passenger : passengerList) {
        if (passenger) {
            currentWeight += passenger->getMass();
        }
    }
    
    return currentWeight;
}

const std::vector<std::shared_ptr<Cargo>>& Aircraft::getCargoList() const {
    return cargoList;
}

const std::vector<std::shared_ptr<UrgentCargo>>& Aircraft::getUrgentCargoList() const {
    return urgentCargoList;
}

const std::vector<std::shared_ptr<Passenger>>& Aircraft::getPassengerList() const {
    return passengerList;
}

// Сеттеры
void Aircraft::setAircraftNumber(const std::string& number) {
    aircraftNumber = number;
}

void Aircraft::setMaxPayload(double maxPayload) {
    this->maxPayload = maxPayload;
}

// Методы для работы с грузами
bool Aircraft::addCargo(std::shared_ptr<Cargo> cargo) {
    if (!cargo || !cargo->isValid()) {
        return false;
    }
    
    // Проверяем, поместится ли груз
    if (!canCarry(cargo->getMass())) {
        return false;
    }
    
    cargoList.push_back(cargo);
    return true;
}

bool Aircraft::addUrgentCargo(std::shared_ptr<UrgentCargo> urgentCargo) {
    if (!urgentCargo || !urgentCargo->isValid()) {
        return false;
    }
    
    // Проверяем, поместится ли груз
    if (!canCarry(urgentCargo->getMass())) {
        return false;
    }
    
    urgentCargoList.push_back(urgentCargo);
    return true;
}

void Aircraft::removeCargo(const std::string& cargoNumber) {
    cargoList.erase(
        std::remove_if(cargoList.begin(), cargoList.end(),
            [&cargoNumber](const std::shared_ptr<Cargo>& cargo) {
                return cargo && cargo->getCargoNumber() == cargoNumber;
            }),
        cargoList.end()
    );
}

void Aircraft::removeUrgentCargo(const std::string& cargoNumber) {
    urgentCargoList.erase(
        std::remove_if(urgentCargoList.begin(), urgentCargoList.end(),
            [&cargoNumber](const std::shared_ptr<UrgentCargo>& cargo) {
                return cargo && cargo->getCargoNumber() == cargoNumber;
            }),
        urgentCargoList.end()
    );
}

std::shared_ptr<Cargo> Aircraft::findCargo(const std::string& cargoNumber) const {
    auto it = std::find_if(cargoList.begin(), cargoList.end(),
        [&cargoNumber](const std::shared_ptr<Cargo>& cargo) {
            return cargo && cargo->getCargoNumber() == cargoNumber;
        });
    
    return (it != cargoList.end()) ? *it : nullptr;
}

std::shared_ptr<UrgentCargo> Aircraft::findUrgentCargo(const std::string& cargoNumber) const {
    auto it = std::find_if(urgentCargoList.begin(), urgentCargoList.end(),
        [&cargoNumber](const std::shared_ptr<UrgentCargo>& cargo) {
            return cargo && cargo->getCargoNumber() == cargoNumber;
        });
    
    return (it != urgentCargoList.end()) ? *it : nullptr;
}

// Методы для работы с пассажирами
bool Aircraft::addPassenger(std::shared_ptr<Passenger> passenger) {
    if (!passenger || !passenger->isValid()) {
        return false;
    }
    
    // Проверяем, поместится ли пассажир
    if (!canCarry(passenger->getMass())) {
        return false;
    }
    
    passengerList.push_back(passenger);
    return true;
}

void Aircraft::removePassenger(const std::string& passengerNumber) {
    passengerList.erase(
        std::remove_if(passengerList.begin(), passengerList.end(),
            [&passengerNumber](const std::shared_ptr<Passenger>& passenger) {
                return passenger && passenger->getPassengerNumber() == passengerNumber;
            }),
        passengerList.end()
    );
}

std::shared_ptr<Passenger> Aircraft::findPassenger(const std::string& passengerNumber) const {
    auto it = std::find_if(passengerList.begin(), passengerList.end(),
        [&passengerNumber](const std::shared_ptr<Passenger>& passenger) {
            return passenger && passenger->getPassengerNumber() == passengerNumber;
        });
    
    return (it != passengerList.end()) ? *it : nullptr;
}

// Проверить, может ли самолёт взять дополнительный груз
bool Aircraft::canCarry(double additionalWeight) const {
    return (getCurrentPayload() + additionalWeight) <= maxPayload;
}

// Получить доступную грузоподъёмность
double Aircraft::getAvailableCapacity() const {
    return maxPayload - getCurrentPayload();
}

// Получить общее количество пассажиров
int Aircraft::getTotalPassengerCount() const {
    return static_cast<int>(passengerList.size());
}

// Получить общее количество грузов
int Aircraft::getTotalCargoCount() const {
    return static_cast<int>(cargoList.size() + urgentCargoList.size());
}

// Очистить все грузы и пассажиров
void Aircraft::clearAll() {
    cargoList.clear();
    urgentCargoList.clear();
    passengerList.clear();
}

// Получить строковое представление объекта
std::string Aircraft::toString() const {
    std::ostringstream oss;
    oss << "Aircraft #" << aircraftNumber << std::endl;
    oss << "  Max Payload: " << std::fixed << std::setprecision(2) << maxPayload << " kg" << std::endl;
    oss << "  Current Payload: " << std::fixed << std::setprecision(2) << getCurrentPayload() << " kg" << std::endl;
    oss << "  Available Capacity: " << std::fixed << std::setprecision(2) << getAvailableCapacity() << " kg" << std::endl;
    oss << "  Cargo: " << cargoList.size() << " items" << std::endl;
    oss << "  Urgent Cargo: " << urgentCargoList.size() << " items" << std::endl;
    oss << "  Passengers: " << passengerList.size() << " people" << std::endl;
    
    return oss.str();
}

// Проверить корректность данных самолёта
bool Aircraft::isValid() const {
    return !aircraftNumber.empty() && maxPayload > 0;
}

// Получить просроченные срочные грузы
std::vector<std::shared_ptr<UrgentCargo>> Aircraft::getOverdueCargo() const {
    std::vector<std::shared_ptr<UrgentCargo>> overdueCargo;
    
    for (const auto& urgentCargo : urgentCargoList) {
        if (urgentCargo && urgentCargo->isOverdue()) {
            overdueCargo.push_back(urgentCargo);
        }
    }
    
    return overdueCargo;
}

// Получить срочные грузы
std::vector<std::shared_ptr<UrgentCargo>> Aircraft::getUrgentCargo() const {
    std::vector<std::shared_ptr<UrgentCargo>> urgentCargo;
    
    for (const auto& cargo : urgentCargoList) {
        if (cargo && cargo->isUrgent()) {
            urgentCargo.push_back(cargo);
        }
    }
    
    return urgentCargo;
}
