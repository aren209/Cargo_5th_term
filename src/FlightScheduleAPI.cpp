#define FLIGHT_SCHEDULE_EXPORTS
#include "FlightScheduleAPI.h"
#include "Schedule.h"
#include "Flight.h"
#include "Aircraft.h"
#include "Airport.h"
#include "Cargo.h"
#include "UrgentCargo.h"
#include "Passenger.h"
#include "FlightScheduleException.h"
#include <string>
#include <sstream>
#include <cstring>

#define BUFFER_SIZE 8192

// ============================================
// Schedule API Implementation
// ============================================

extern "C" {

ScheduleHandle Schedule_Create() {
    return new Schedule();
}

void Schedule_Destroy(ScheduleHandle handle) {
    if (handle) {
        delete static_cast<Schedule*>(handle);
    }
}

int Schedule_AddFlight(ScheduleHandle handle, FlightHandle flight) {
    if (!handle || !flight) return 0;
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        auto* flightPtr = static_cast<Flight*>(flight);
        auto sharedFlight = std::make_shared<Flight>(*flightPtr);
        schedule->addFlight(sharedFlight);
        return 1;
    } catch (...) {
        return 0;
    }
}

int Schedule_RemoveFlight(ScheduleHandle handle, const char* flightNumber) {
    if (!handle || !flightNumber) return 0;
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        schedule->removeFlight(std::string(flightNumber));
        return 1;
    } catch (...) {
        return 0;
    }
}

FlightHandle Schedule_FindFlight(ScheduleHandle handle, const char* flightNumber) {
    if (!handle || !flightNumber) return nullptr;
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        auto flight = schedule->findFlight(std::string(flightNumber));
        if (flight) {
            // Создаём копию для возврата
            return new Flight(*flight);
        }
        return nullptr;
    } catch (...) {
        return nullptr;
    }
}

int Schedule_IsValid(ScheduleHandle handle) {
    if (!handle) return 0;
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        return schedule->isValid() ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

void Schedule_GetValidationErrors(ScheduleHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        auto errors = schedule->getValidationErrors();
        std::ostringstream oss;
        for (const auto& error : errors) {
            oss << error << "\n";
        }
        std::string result = oss.str();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Schedule_ValidateAndFix(ScheduleHandle handle) {
    if (!handle) return;
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        schedule->validateAndFix();
    } catch (...) {
        // Игнорируем ошибки
    }
}

void Schedule_ToString(ScheduleHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        std::string result = schedule->toString();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Schedule_GetScheduleForAircraft(ScheduleHandle handle, const char* aircraftId, char* buffer, int bufferSize) {
    if (!handle || !aircraftId || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        std::string result = schedule->getScheduleForAircraft(std::string(aircraftId));
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Schedule_GetScheduleForAircraftInRange(ScheduleHandle handle, const char* aircraftId,
                                            long long startTime, long long endTime, char* buffer, int bufferSize) {
    if (!handle || !aircraftId || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        std::string result = schedule->getScheduleForAircraftInRange(std::string(aircraftId),
                                                                      static_cast<std::time_t>(startTime),
                                                                      static_cast<std::time_t>(endTime));
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Schedule_GetOverdueUrgentCargoReport(ScheduleHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        std::string result = schedule->getOverdueUrgentCargoReport();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

double Schedule_GetTotalFlightTime(ScheduleHandle handle, const char* aircraftId) {
    if (!handle || !aircraftId) return 0.0;
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        return schedule->getTotalFlightTime(std::string(aircraftId));
    } catch (...) {
        return 0.0;
    }
}

int Schedule_GetTotalFlights(ScheduleHandle handle) {
    if (!handle) return 0;
    try {
        auto* schedule = static_cast<Schedule*>(handle);
        return schedule->getTotalFlights();
    } catch (...) {
        return 0;
    }
}

// ============================================
// Flight API Implementation
// ============================================

FlightHandle Flight_Create(const char* number, const char* departure, const char* destination,
                           long long depTime, long long arrTime, const char* aircraft) {
    if (!number || !departure || !destination || !aircraft) return nullptr;
    try {
        return new Flight(std::string(number), std::string(departure), std::string(destination),
                         static_cast<std::time_t>(depTime), static_cast<std::time_t>(arrTime),
                         std::string(aircraft));
    } catch (...) {
        return nullptr;
    }
}

void Flight_Destroy(FlightHandle handle) {
    if (handle) {
        delete static_cast<Flight*>(handle);
    }
}

void Flight_GetFlightNumber(FlightHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* flight = static_cast<Flight*>(handle);
        std::string result = flight->getFlightNumber();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Flight_GetDepartureAirport(FlightHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* flight = static_cast<Flight*>(handle);
        std::string result = flight->getDepartureAirport();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Flight_GetDestinationAirport(FlightHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* flight = static_cast<Flight*>(handle);
        std::string result = flight->getDestinationAirport();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

long long Flight_GetDepartureTime(FlightHandle handle) {
    if (!handle) return 0;
    try {
        auto* flight = static_cast<Flight*>(handle);
        return static_cast<long long>(flight->getDepartureTime());
    } catch (...) {
        return 0;
    }
}

long long Flight_GetArrivalTime(FlightHandle handle) {
    if (!handle) return 0;
    try {
        auto* flight = static_cast<Flight*>(handle);
        return static_cast<long long>(flight->getArrivalTime());
    } catch (...) {
        return 0;
    }
}

void Flight_GetAircraftId(FlightHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* flight = static_cast<Flight*>(handle);
        std::string result = flight->getAircraftId();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

int Flight_IsCompleted(FlightHandle handle) {
    if (!handle) return 0;
    try {
        auto* flight = static_cast<Flight*>(handle);
        return flight->isCompleted() ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

void Flight_ToString(FlightHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* flight = static_cast<Flight*>(handle);
        std::string result = flight->toString();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

int Flight_IsValid(FlightHandle handle) {
    if (!handle) return 0;
    try {
        auto* flight = static_cast<Flight*>(handle);
        return flight->isValid() ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

double Flight_GetFlightDurationHours(FlightHandle handle) {
    if (!handle) return 0.0;
    try {
        auto* flight = static_cast<Flight*>(handle);
        return flight->getFlightDurationHours();
    } catch (...) {
        return 0.0;
    }
}

// ============================================
// Aircraft API Implementation
// ============================================

AircraftHandle Aircraft_Create(const char* number, double maxPayload) {
    if (!number) return nullptr;
    try {
        return new Aircraft(std::string(number), maxPayload);
    } catch (const InvalidAircraftException& e) {
        // Ловим конкретное исключение для некорректной грузоподъёмности
        // В реальном приложении здесь можно было бы логировать ошибку
        return nullptr;
    } catch (const FlightScheduleException& e) {
        // Ловим другие исключения системы расписания
        return nullptr;
    } catch (...) {
        // Ловим все остальные исключения (на случай непредвиденных ошибок)
        return nullptr;
    }
}

void Aircraft_Destroy(AircraftHandle handle) {
    if (handle) {
        delete static_cast<Aircraft*>(handle);
    }
}

void Aircraft_GetAircraftNumber(AircraftHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* aircraft = static_cast<Aircraft*>(handle);
        std::string result = aircraft->getAircraftNumber();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

double Aircraft_GetMaxPayload(AircraftHandle handle) {
    if (!handle) return 0.0;
    try {
        auto* aircraft = static_cast<Aircraft*>(handle);
        return aircraft->getMaxPayload();
    } catch (...) {
        return 0.0;
    }
}

double Aircraft_GetCurrentPayload(AircraftHandle handle) {
    if (!handle) return 0.0;
    try {
        auto* aircraft = static_cast<Aircraft*>(handle);
        return aircraft->getCurrentPayload();
    } catch (...) {
        return 0.0;
    }
}

int Aircraft_AddCargo(AircraftHandle handle, CargoHandle cargo) {
    if (!handle || !cargo) return 0;
    try {
        auto* aircraft = static_cast<Aircraft*>(handle);
        auto* cargoPtr = static_cast<Cargo*>(cargo);
        auto sharedCargo = std::make_shared<Cargo>(*cargoPtr);
        return aircraft->addCargo(sharedCargo) ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

int Aircraft_AddUrgentCargo(AircraftHandle handle, UrgentCargoHandle urgentCargo) {
    if (!handle || !urgentCargo) return 0;
    try {
        auto* aircraft = static_cast<Aircraft*>(handle);
        auto* urgentCargoPtr = static_cast<UrgentCargo*>(urgentCargo);
        auto sharedUrgentCargo = std::make_shared<UrgentCargo>(*urgentCargoPtr);
        return aircraft->addUrgentCargo(sharedUrgentCargo) ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

int Aircraft_AddPassenger(AircraftHandle handle, PassengerHandle passenger) {
    if (!handle || !passenger) return 0;
    try {
        auto* aircraft = static_cast<Aircraft*>(handle);
        auto* passengerPtr = static_cast<Passenger*>(passenger);
        auto sharedPassenger = std::make_shared<Passenger>(*passengerPtr);
        return aircraft->addPassenger(sharedPassenger) ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

void Aircraft_ToString(AircraftHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* aircraft = static_cast<Aircraft*>(handle);
        std::string result = aircraft->toString();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

double Aircraft_GetAvailableCapacity(AircraftHandle handle) {
    if (!handle) return 0.0;
    try {
        auto* aircraft = static_cast<Aircraft*>(handle);
        return aircraft->getAvailableCapacity();
    } catch (...) {
        return 0.0;
    }
}

// ============================================
// Airport API Implementation
// ============================================

AirportHandle Airport_Create(const char* name) {
    if (!name) return nullptr;
    try {
        return new Airport(std::string(name));
    } catch (...) {
        return nullptr;
    }
}

void Airport_Destroy(AirportHandle handle) {
    if (handle) {
        delete static_cast<Airport*>(handle);
    }
}

void Airport_GetName(AirportHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* airport = static_cast<Airport*>(handle);
        std::string result = airport->getName();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Airport_AddAircraft(AirportHandle handle, AircraftHandle aircraft) {
    if (!handle || !aircraft) return;
    try {
        auto* airport = static_cast<Airport*>(handle);
        auto* aircraftPtr = static_cast<Aircraft*>(aircraft);
        auto sharedAircraft = std::make_shared<Aircraft>(*aircraftPtr);
        airport->addAircraft(sharedAircraft);
    } catch (...) {
        // Игнорируем ошибки
    }
}

void Airport_AddCargo(AirportHandle handle, CargoHandle cargo) {
    if (!handle || !cargo) return;
    try {
        auto* airport = static_cast<Airport*>(handle);
        auto* cargoPtr = static_cast<Cargo*>(cargo);
        auto sharedCargo = std::make_shared<Cargo>(*cargoPtr);
        airport->addCargo(sharedCargo);
    } catch (...) {
        // Игнорируем ошибки
    }
}

void Airport_ToString(AirportHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* airport = static_cast<Airport*>(handle);
        std::string result = airport->toString();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

// ============================================
// Cargo API Implementation
// ============================================

CargoHandle Cargo_Create(const char* number, double mass, const char* departure,
                         const char* destination, const char* current, long long arrival) {
    if (!number || !departure || !destination || !current) return nullptr;
    try {
        return new Cargo(std::string(number), mass, std::string(departure),
                        std::string(destination), std::string(current),
                        static_cast<std::time_t>(arrival));
    } catch (...) {
        return nullptr;
    }
}

void Cargo_Destroy(CargoHandle handle) {
    if (handle) {
        delete static_cast<Cargo*>(handle);
    }
}

void Cargo_GetCargoNumber(CargoHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* cargo = static_cast<Cargo*>(handle);
        std::string result = cargo->getCargoNumber();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

double Cargo_GetMass(CargoHandle handle) {
    if (!handle) return 0.0;
    try {
        auto* cargo = static_cast<Cargo*>(handle);
        return cargo->getMass();
    } catch (...) {
        return 0.0;
    }
}

void Cargo_ToString(CargoHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* cargo = static_cast<Cargo*>(handle);
        std::string result = cargo->toString();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

// ============================================
// UrgentCargo API Implementation
// ============================================

UrgentCargoHandle UrgentCargo_Create(const char* number, double mass, const char* departure,
                                      const char* destination, const char* current,
                                      long long arrival, long long deadline) {
    if (!number || !departure || !destination || !current) return nullptr;
    try {
        return new UrgentCargo(std::string(number), mass, std::string(departure),
                              std::string(destination), std::string(current),
                              static_cast<std::time_t>(arrival),
                              static_cast<std::time_t>(deadline));
    } catch (...) {
        return nullptr;
    }
}

void UrgentCargo_Destroy(UrgentCargoHandle handle) {
    if (handle) {
        delete static_cast<UrgentCargo*>(handle);
    }
}

int UrgentCargo_IsOverdue(UrgentCargoHandle handle) {
    if (!handle) return 0;
    try {
        auto* urgentCargo = static_cast<UrgentCargo*>(handle);
        return urgentCargo->isOverdue() ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

void UrgentCargo_ToString(UrgentCargoHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* urgentCargo = static_cast<UrgentCargo*>(handle);
        std::string result = urgentCargo->toString();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

// ============================================
// Passenger API Implementation
// ============================================

PassengerHandle Passenger_Create(const char* number, const char* name,
                                  const char* departure, const char* destination) {
    if (!number || !name || !departure || !destination) return nullptr;
    try {
        return new Passenger(std::string(number), std::string(name),
                            std::string(departure), std::string(destination));
    } catch (...) {
        return nullptr;
    }
}

void Passenger_Destroy(PassengerHandle handle) {
    if (handle) {
        delete static_cast<Passenger*>(handle);
    }
}

void Passenger_GetPassengerNumber(PassengerHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* passenger = static_cast<Passenger*>(handle);
        std::string result = passenger->getPassengerNumber();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Passenger_GetName(PassengerHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* passenger = static_cast<Passenger*>(handle);
        std::string result = passenger->getName();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

void Passenger_ToString(PassengerHandle handle, char* buffer, int bufferSize) {
    if (!handle || !buffer || bufferSize <= 0) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
        return;
    }
    try {
        auto* passenger = static_cast<Passenger*>(handle);
        std::string result = passenger->toString();
        strncpy_s(buffer, bufferSize, result.c_str(), _TRUNCATE);
    } catch (...) {
        if (buffer && bufferSize > 0) buffer[0] = '\0';
    }
}

} // extern "C"

