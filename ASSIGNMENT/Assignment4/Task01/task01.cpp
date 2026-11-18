#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Staff {
protected:
    std::string name;
    int id;
    double basic;

public:
    Staff(const std::string& n, int staffId, double amount)
        : name(n), id(staffId), basic(amount) {}

    virtual double pay() const = 0;
    virtual void display() const {
        std::cout << name << " (ID " << id << ") pay = "
                  << std::fixed << std::setprecision(2) << pay() << '\n';
    }
    virtual ~Staff() = default;
};

class Professor : public Staff {
private:
    double researchAllowance;

public:
    Professor(const std::string& n, int staffId, double amount, double research)
        : Staff(n, staffId, amount), researchAllowance(research) {}

    double pay() const override {
        return basic + basic * 0.42 + researchAllowance;
    }
};

class LabAssistant : public Staff {
public:
    LabAssistant(const std::string& n, int staffId, double amount)
        : Staff(n, staffId, amount) {}

    double pay() const override {
        return basic + basic * 0.42;
    }
};

class GuestFaculty : public Staff {
private:
    double hours;
    double hourlyRate;

public:
    GuestFaculty(const std::string& n, int staffId, double workedHours, double rate)
        : Staff(n, staffId, 0.0), hours(workedHours), hourlyRate(rate) {}

    double pay() const override {
        return hours * hourlyRate;
    }
};

int main() {
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Anita", 101, 50000, 8000));
    payroll.push_back(std::make_unique<LabAssistant>("Bala", 102, 30000));
    payroll.push_back(std::make_unique<GuestFaculty>("Charan", 103, 40, 750));

    for (const auto& member : payroll) {
        member->display();
    }
    return 0;
}
