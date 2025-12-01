#include "Schedule.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <map>

// Конструктор по умолчанию
Schedule::Schedule() {
}

// Конструктор копирования
Schedule::Schedule(const Schedule& other) : flights(other.flights) {
}

// Оператор присваивания
Schedule& Schedule::operator=(const Schedule& other) {
    if (this != &other) {
        flights = other.flights;
    }
    return *this;
}

// Деструктор
Schedule::~Schedule() {
    // Деструктор пустой, так как мы используем shared_ptr
}

// Добавить рейс в расписание
void Schedule::addFlight(std::shared_ptr<Flight> flight) {
    if (flight && flight->isValid()) {
        flights.push_back(flight);
        sortFlights();  // Сортируем после добавления
    }
}

// Удалить рейс из расписания
void Schedule::removeFlight(const std::string& flightNumber) {
    flights.erase(
        std::remove_if(flights.begin(), flights.end(),
            [&flightNumber](const std::shared_ptr<Flight>& flight) {
                return flight && flight->getFlightNumber() == flightNumber;
            }),
        flights.end()
    );
}

// Найти рейс по номеру
std::shared_ptr<Flight> Schedule::findFlight(const std::string& flightNumber) const {
    auto it = std::find_if(flights.begin(), flights.end(),
        [&flightNumber](const std::shared_ptr<Flight>& flight) {
            return flight && flight->getFlightNumber() == flightNumber;
        });
    
    return (it != flights.end()) ? *it : nullptr;
}

// Отсортировать рейсы по времени отправления
void Schedule::sortFlights() {
    std::sort(flights.begin(), flights.end(),
        [](const std::shared_ptr<Flight>& a, const std::shared_ptr<Flight>& b) {
            if (!a || !b) return false;
            return *a < *b;  // Используем оператор сравнения Flight
        });
}

// Проверить наличие конфликтов в расписании
bool Schedule::hasConflicts() const {
    for (size_t i = 0; i < flights.size(); ++i) {
        for (size_t j = i + 1; j < flights.size(); ++j) {
            if (flights[i] && flights[j] && flights[i]->conflictsWith(*flights[j])) {
                return true;
            }
        }
    }
    return false;
}

// Проверить корректность расписания
bool Schedule::isValid() const {
    // Проверяем, что все рейсы валидны
    for (const auto& flight : flights) {
        if (!flight || !flight->isValid()) {
            return false;
        }
    }
    
    // Проверяем отсутствие конфликтов
    return !hasConflicts();
}

// Получить список ошибок валидации
std::vector<std::string> Schedule::getValidationErrors() const {
    std::vector<std::string> errors;
    
    // Проверяем валидность каждого рейса
    for (const auto& flight : flights) {
        if (!flight) {
            errors.push_back("Null flight found in schedule");
            continue;
        }
        
        if (!flight->isValid()) {
            errors.push_back("Invalid flight: " + flight->getFlightNumber());
        }
    }
    
    // Проверяем конфликты
    if (hasConflicts()) {
        errors.push_back("Schedule has conflicts between flights");
    }
    
    return errors;
}

// Проверить и исправить ошибки в расписании
void Schedule::validateAndFix() {
    // Удаляем невалидные рейсы
    flights.erase(
        std::remove_if(flights.begin(), flights.end(),
            [](const std::shared_ptr<Flight>& flight) {
                return !flight || !flight->isValid();
            }),
        flights.end()
    );
    
    // Сортируем после очистки
    sortFlights();
}

// Получить список всех рейсов
const std::vector<std::shared_ptr<Flight>>& Schedule::getFlights() const {
    return flights;
}

// Получить рейсы самолёта
std::vector<std::shared_ptr<Flight>> Schedule::getFlightsByAircraft(const std::string& aircraftId) const {
    std::vector<std::shared_ptr<Flight>> aircraftFlights;
    
    for (const auto& flight : flights) {
        if (flight && flight->getAircraftId() == aircraftId) {
            aircraftFlights.push_back(flight);
        }
    }
    
    return aircraftFlights;
}

// Получить рейсы аэропорта
std::vector<std::shared_ptr<Flight>> Schedule::getFlightsByAirport(const std::string& airportCode) const {
    std::vector<std::shared_ptr<Flight>> airportFlights;
    
    for (const auto& flight : flights) {
        if (flight && (flight->getDepartureAirport() == airportCode || 
                       flight->getDestinationAirport() == airportCode)) {
            airportFlights.push_back(flight);
        }
    }
    
    return airportFlights;
}

// Получить рейсы в временном диапазоне
std::vector<std::shared_ptr<Flight>> Schedule::getFlightsInTimeRange(std::time_t startTime, std::time_t endTime) const {
    std::vector<std::shared_ptr<Flight>> rangeFlights;
    
    for (const auto& flight : flights) {
        if (flight && flight->getDepartureTime() >= startTime && 
            flight->getDepartureTime() <= endTime) {
            rangeFlights.push_back(flight);
        }
    }
    
    return rangeFlights;
}

// Получить общее время полётов самолёта
double Schedule::getTotalFlightTime(const std::string& aircraftId) const {
    double totalTime = 0.0;
    
    for (const auto& flight : flights) {
        if (flight && flight->getAircraftId() == aircraftId) {
            totalTime += flight->getFlightDurationHours();
        }
    }
    
    return totalTime;
}

// Получить время полётов в диапазоне
double Schedule::getTotalFlightTimeInRange(const std::string& aircraftId, std::time_t startTime, std::time_t endTime) const {
    double totalTime = 0.0;
    
    for (const auto& flight : flights) {
        if (flight && flight->getAircraftId() == aircraftId &&
            flight->getDepartureTime() >= startTime && 
            flight->getDepartureTime() <= endTime) {
            totalTime += flight->getFlightDurationHours();
        }
    }
    
    return totalTime;
}

// Получить список перегруженных самолётов
std::vector<std::string> Schedule::getOverworkedAircraft() const {
    std::vector<std::string> overworkedAircraft;
    const double MAX_DAILY_FLIGHT_TIME = 12.0; // Максимум 12 часов в день
    
    // Группируем рейсы по самолётам и дням
    std::map<std::string, std::map<std::string, double> > aircraftDailyTime;
    
    for (const auto& flight : flights) {
        if (!flight) continue;
        
        std::string aircraftId = flight->getAircraftId();
        std::time_t depTime = flight->getDepartureTime();
        
        // Получаем дату (без времени)
        std::tm tm;
        localtime_s(&tm, &depTime);
        std::string date = std::to_string(tm.tm_year + 1900) + "-" + 
                          std::to_string(tm.tm_mon + 1) + "-" + 
                          std::to_string(tm.tm_mday);
        
        aircraftDailyTime[aircraftId][date] += flight->getFlightDurationHours();
    }
    
    // Проверяем, какие самолёты перегружены
    for (const auto& aircraft : aircraftDailyTime) {
        for (const auto& day : aircraft.second) {
            if (day.second > MAX_DAILY_FLIGHT_TIME) {
                overworkedAircraft.push_back(aircraft.first);
                break;
            }
        }
    }
    
    return overworkedAircraft;
}

// Получить строковое представление всего расписания
std::string Schedule::toString() const {
    std::ostringstream oss;
    oss << "Flight Schedule (" << flights.size() << " flights):" << std::endl;
    oss << "==========================================" << std::endl;
    
    for (const auto& flight : flights) {
        if (flight) {
            oss << flight->toString() << std::endl;
        }
    }
    
    return oss.str();
}

// Получить расписание для самолёта
std::string Schedule::getScheduleForAircraft(const std::string& aircraftId) const {
    std::ostringstream oss;
    oss << "Schedule for Aircraft " << aircraftId << ":" << std::endl;
    oss << "==========================================" << std::endl;
    
    auto aircraftFlights = getFlightsByAircraft(aircraftId);
    for (const auto& flight : aircraftFlights) {
        if (flight) {
            oss << flight->toString() << std::endl;
        }
    }
    
    if (aircraftFlights.empty()) {
        oss << "No flights scheduled for this aircraft." << std::endl;
    }
    
    return oss.str();
}

// Получить расписание самолёта в диапазоне
std::string Schedule::getScheduleForAircraftInRange(const std::string& aircraftId, std::time_t startTime, std::time_t endTime) const {
    std::ostringstream oss;
    char startTimeStr[100];
    char endTimeStr[100];
    ctime_s(startTimeStr, sizeof(startTimeStr), &startTime);
    ctime_s(endTimeStr, sizeof(endTimeStr), &endTime);
    oss << "Schedule for Aircraft " << aircraftId << " (" << startTimeStr << " - " << endTimeStr << "):" << std::endl;
    oss << "==========================================" << std::endl;
    
    auto aircraftFlights = getFlightsByAircraft(aircraftId);
    for (const auto& flight : aircraftFlights) {
        if (flight && flight->getDepartureTime() >= startTime && flight->getDepartureTime() <= endTime) {
            oss << flight->toString() << std::endl;
        }
    }
    
    return oss.str();
}

// Получить отчёт о просроченных срочных грузах
std::string Schedule::getOverdueUrgentCargoReport() const {
    std::ostringstream oss;
    oss << "Overdue Urgent Cargo Report:" << std::endl;
    oss << "=============================" << std::endl;
    
    oss << "No overdue urgent cargo found." << std::endl;
    
    return oss.str();
}

// Завершить рейс
void Schedule::completeFlight(const std::string& flightNumber) {
    auto flight = findFlight(flightNumber);
    if (flight) {
        flight->completeFlight();
    }
}

// Добавить обратные рейсы для всех прямых рейсов
void Schedule::addReturnFlights() {
    std::vector<std::shared_ptr<Flight>> returnFlights;
    
    for (const auto& flight : flights) {
        if (flight && !flight->isCompleted()) {
            Flight returnFlight = flight->createReturnFlight();
            returnFlights.push_back(std::make_shared<Flight>(returnFlight));
        }
    }
    
    // Добавляем обратные рейсы
    for (const auto& returnFlight : returnFlights) {
        addFlight(returnFlight);
    }
}

// Получить общее количество рейсов
int Schedule::getTotalFlights() const {
    return static_cast<int>(flights.size());
}

// Получить количество завершённых рейсов
int Schedule::getCompletedFlights() const {
    int count = 0;
    for (const auto& flight : flights) {
        if (flight && flight->isCompleted()) {
            count++;
        }
    }
    return count;
}

// Получить количество запланированных рейсов
int Schedule::getScheduledFlights() const {
    int count = 0;
    for (const auto& flight : flights) {
        if (flight && flight->isScheduled()) {
            count++;
        }
    }
    return count;
}

// Получить количество рейсов в процессе
int Schedule::getInProgressFlights() const {
    int count = 0;
    for (const auto& flight : flights) {
        if (flight && flight->isInProgress()) {
            count++;
        }
    }
    return count;
}
