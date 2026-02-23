//! \file FlightScheduleAPI.h
//! \brief C API для экспорта классов C++ в DLL (используется Python ctypes).

#ifndef FLIGHT_SCHEDULE_API_H
#define FLIGHT_SCHEDULE_API_H

#ifdef _WIN32
    #ifdef FLIGHT_SCHEDULE_EXPORTS
        #define FLIGHT_SCHEDULE_API __declspec(dllexport)
    #else
        #define FLIGHT_SCHEDULE_API __declspec(dllimport)
    #endif
#else
    #define FLIGHT_SCHEDULE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations для C интерфейса
typedef void* ScheduleHandle;
typedef void* FlightHandle;
typedef void* AircraftHandle;
typedef void* AirportHandle;
typedef void* CargoHandle;
typedef void* UrgentCargoHandle;
typedef void* PassengerHandle;

// ============================================
// Schedule API
// ============================================

FLIGHT_SCHEDULE_API ScheduleHandle Schedule_Create();
FLIGHT_SCHEDULE_API void Schedule_Destroy(ScheduleHandle handle);
FLIGHT_SCHEDULE_API int Schedule_AddFlight(ScheduleHandle handle, FlightHandle flight);
FLIGHT_SCHEDULE_API int Schedule_RemoveFlight(ScheduleHandle handle, const char* flightNumber);
FLIGHT_SCHEDULE_API FlightHandle Schedule_FindFlight(ScheduleHandle handle, const char* flightNumber);
FLIGHT_SCHEDULE_API int Schedule_IsValid(ScheduleHandle handle);
FLIGHT_SCHEDULE_API void Schedule_GetValidationErrors(ScheduleHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Schedule_ValidateAndFix(ScheduleHandle handle);
FLIGHT_SCHEDULE_API void Schedule_ToString(ScheduleHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Schedule_GetScheduleForAircraft(ScheduleHandle handle, const char* aircraftId, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Schedule_GetScheduleForAircraftInRange(ScheduleHandle handle, const char* aircraftId, 
                                                                 long long startTime, long long endTime, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Schedule_GetOverdueUrgentCargoReport(ScheduleHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API double Schedule_GetTotalFlightTime(ScheduleHandle handle, const char* aircraftId);
FLIGHT_SCHEDULE_API int Schedule_GetTotalFlights(ScheduleHandle handle);

// ============================================
// Flight API
// ============================================

FLIGHT_SCHEDULE_API FlightHandle Flight_Create(const char* number, const char* departure, const char* destination,
                                                long long depTime, long long arrTime, const char* aircraft);
FLIGHT_SCHEDULE_API void Flight_Destroy(FlightHandle handle);
FLIGHT_SCHEDULE_API void Flight_GetFlightNumber(FlightHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Flight_GetDepartureAirport(FlightHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Flight_GetDestinationAirport(FlightHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API long long Flight_GetDepartureTime(FlightHandle handle);
FLIGHT_SCHEDULE_API long long Flight_GetArrivalTime(FlightHandle handle);
FLIGHT_SCHEDULE_API void Flight_GetAircraftId(FlightHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API int Flight_IsCompleted(FlightHandle handle);
FLIGHT_SCHEDULE_API void Flight_ToString(FlightHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API int Flight_IsValid(FlightHandle handle);
FLIGHT_SCHEDULE_API double Flight_GetFlightDurationHours(FlightHandle handle);

// ============================================
// Aircraft API
// ============================================

FLIGHT_SCHEDULE_API AircraftHandle Aircraft_Create(const char* number, double maxPayload);
FLIGHT_SCHEDULE_API void Aircraft_Destroy(AircraftHandle handle);
FLIGHT_SCHEDULE_API void Aircraft_GetAircraftNumber(AircraftHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API double Aircraft_GetMaxPayload(AircraftHandle handle);
FLIGHT_SCHEDULE_API double Aircraft_GetCurrentPayload(AircraftHandle handle);
FLIGHT_SCHEDULE_API int Aircraft_AddCargo(AircraftHandle handle, CargoHandle cargo);
FLIGHT_SCHEDULE_API int Aircraft_AddUrgentCargo(AircraftHandle handle, UrgentCargoHandle urgentCargo);
FLIGHT_SCHEDULE_API int Aircraft_AddPassenger(AircraftHandle handle, PassengerHandle passenger);
FLIGHT_SCHEDULE_API void Aircraft_ToString(AircraftHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API double Aircraft_GetAvailableCapacity(AircraftHandle handle);

// ============================================
// Airport API
// ============================================

FLIGHT_SCHEDULE_API AirportHandle Airport_Create(const char* name);
FLIGHT_SCHEDULE_API void Airport_Destroy(AirportHandle handle);
FLIGHT_SCHEDULE_API void Airport_GetName(AirportHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Airport_AddAircraft(AirportHandle handle, AircraftHandle aircraft);
FLIGHT_SCHEDULE_API void Airport_AddCargo(AirportHandle handle, CargoHandle cargo);
FLIGHT_SCHEDULE_API void Airport_ToString(AirportHandle handle, char* buffer, int bufferSize);

// ============================================
// Cargo API
// ============================================

FLIGHT_SCHEDULE_API CargoHandle Cargo_Create(const char* number, double mass, const char* departure,
                                              const char* destination, const char* current, long long arrival);
FLIGHT_SCHEDULE_API void Cargo_Destroy(CargoHandle handle);
FLIGHT_SCHEDULE_API void Cargo_GetCargoNumber(CargoHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API double Cargo_GetMass(CargoHandle handle);
FLIGHT_SCHEDULE_API void Cargo_ToString(CargoHandle handle, char* buffer, int bufferSize);

// ============================================
// UrgentCargo API
// ============================================

FLIGHT_SCHEDULE_API UrgentCargoHandle UrgentCargo_Create(const char* number, double mass, const char* departure,
                                                          const char* destination, const char* current,
                                                          long long arrival, long long deadline);
FLIGHT_SCHEDULE_API void UrgentCargo_Destroy(UrgentCargoHandle handle);
FLIGHT_SCHEDULE_API int UrgentCargo_IsOverdue(UrgentCargoHandle handle);
FLIGHT_SCHEDULE_API void UrgentCargo_ToString(UrgentCargoHandle handle, char* buffer, int bufferSize);

// ============================================
// Passenger API
// ============================================

FLIGHT_SCHEDULE_API PassengerHandle Passenger_Create(const char* number, const char* name,
                                                      const char* departure, const char* destination);
FLIGHT_SCHEDULE_API void Passenger_Destroy(PassengerHandle handle);
FLIGHT_SCHEDULE_API void Passenger_GetPassengerNumber(PassengerHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Passenger_GetName(PassengerHandle handle, char* buffer, int bufferSize);
FLIGHT_SCHEDULE_API void Passenger_ToString(PassengerHandle handle, char* buffer, int bufferSize);

#ifdef __cplusplus
}
#endif

#endif // FLIGHT_SCHEDULE_API_H

