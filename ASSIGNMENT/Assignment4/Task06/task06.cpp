#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Vehicle {
protected:
    std::string registration;
    double dailyRate;
    bool rented = false;

public:
    Vehicle(const std::string& reg, double rate)
        : registration(reg), dailyRate(rate) {}
    virtual double charge(int days) const = 0;
    virtual std::string type() const = 0;

    bool rent() {
        if (rented) return false;
        rented = true;
        return true;
    }

    double returnVehicle(int daysLate = 0) {
        rented = false;
        const double lateFee = daysLate > 0 ? daysLate * 100.0 : 0.0;
        return lateFee;
    }

    bool isRented() const { return rented; }
    std::string getRegistration() const { return registration; }
    virtual void display() const {
        std::cout << type() << " | " << registration
                  << " | daily rate: " << dailyRate
                  << " | status: " << (rented ? "Rented" : "Available") << '\n';
    }
    virtual ~Vehicle() = default;
};

class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& reg, double rate) : Vehicle(reg, rate) {}
    double charge(int days) const override { return dailyRate * days; }
    std::string type() const override { return "Bicycle"; }
};

class MotorVehicle : public Vehicle {
protected:
    double fuelOrChargeLevel;
    std::string serviceDueDate;

public:
    MotorVehicle(const std::string& reg, double rate, double level,
                 const std::string& due)
        : Vehicle(reg, rate), fuelOrChargeLevel(level), serviceDueDate(due) {}
};

class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(const std::string& reg, double rate, double fuel,
               const std::string& due)
        : MotorVehicle(reg, rate, fuel, due) {}
    double charge(int days) const override { return dailyRate * days; }
    std::string type() const override { return "TwoWheeler"; }
};

class Car : public MotorVehicle {
protected:
    int seats;

public:
    Car(const std::string& reg, double rate, double fuel,
        const std::string& due, int capacity)
        : MotorVehicle(reg, rate, fuel, due), seats(capacity) {}
    double charge(int days) const override { return dailyRate * days; }
    std::string type() const override { return "Car"; }
};

class ElectricCar : public Car {
private:
    double batteryKWh;
    double pricePerKWh;

public:
    ElectricCar(const std::string& reg, double rate, double battery,
                double kWhPrice, const std::string& due, int capacity)
        : Car(reg, rate, battery, due, capacity),
          batteryKWh(battery), pricePerKWh(kWhPrice) {}

    double charge(int days) const override {
        const double rental = dailyRate * days;
        const double energyCost = batteryKWh * pricePerKWh;
        return rental + energyCost;
    }
    std::string type() const override { return "ElectricCar"; }
};

// Extension example: add a new type without editing existing vehicle classes.
class ElectricBus : public Vehicle {
private:
    int seats;
    double batteryKWh;
    double pricePerKWh;

public:
    ElectricBus(const std::string& reg, double rate, int capacity,
                double battery, double energyPrice)
        : Vehicle(reg, rate), seats(capacity),
          batteryKWh(battery), pricePerKWh(energyPrice) {}

    double charge(int days) const override {
        return dailyRate * days + batteryKWh * pricePerKWh;
    }
    std::string type() const override { return "ElectricBus"; }
};

int main() {
    std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<Bicycle>("BIKE-101", 100));
    fleet.push_back(std::make_unique<TwoWheeler>("TN-01-AB-1234", 250, 80, "2027-01-15"));
    fleet.push_back(std::make_unique<Car>("TN-02-CD-5678", 1500, 60, "2027-02-01", 5));
    fleet.push_back(std::make_unique<ElectricCar>("TN-03-EV-9999", 1800, 50, 8, "2027-03-10", 5));
    fleet.push_back(std::make_unique<ElectricBus>("TN-04-BUS-1111", 4000, 30, 180, 8));

    std::cout << "Fleet listing:\n";
    for (const auto& vehicle : fleet) vehicle->display();

    std::cout << "\nRent and return demonstration:\n";
    for (const auto& vehicle : fleet) {
        if (vehicle->rent()) {
            std::cout << vehicle->getRegistration()
                      << " rental charge for 2 days = "
                      << vehicle->charge(2) << '\n';
            const double lateFee = vehicle->returnVehicle(1);
            std::cout << "Returned; late fee = " << lateFee << '\n';
        }
    }

    std::cout << "\nFleet after return:\n";
    for (const auto& vehicle : fleet) vehicle->display();
    return 0;
}
