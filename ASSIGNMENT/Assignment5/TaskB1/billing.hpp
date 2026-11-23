// billing.hpp -- Hospital Billing and Insurance Claims: core design
// Assignment 5 (CO5): runtime polymorphism + exception handling.
//
//   Bill  (abstract)  <- OutPatientBill, InPatientBill, DayCareBill, EmergencyBill
//   Payer (abstract)  <- SelfPay, Insurance, GovernmentScheme
//   BillingError      <- NegativeChargeError, ClaimError (<- ClaimExceedsCoverError,
//                        UnknownPackageError), PatientRecordError
#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace hosp {

inline std::string money(double v)
{
    std::ostringstream os;
    os << std::fixed << std::setprecision(2) << v;
    return os.str();
}

// ---------------------------------------------------------------- exceptions (A4.1)
class BillingError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
class NegativeChargeError : public BillingError {
public:
    using BillingError::BillingError;
};
class ClaimError : public BillingError {
public:
    using BillingError::BillingError;
};
class ClaimExceedsCoverError : public ClaimError {
public:
    using ClaimError::ClaimError;
};
class UnknownPackageError : public ClaimError {
public:
    using ClaimError::ClaimError;
};
class PatientRecordError : public BillingError {
public:
    using BillingError::BillingError;
};

inline double requireNonNegative(double value, const char* what)
{
    if (value < 0)
        throw NegativeChargeError(std::string(what) + " must not be negative (got " + money(value) + ")");
    return value;
}

// ---------------------------------------------------------------- bills (A1)
class Bill {
public:
    explicit Bill(std::string patient) : patient_(std::move(patient)) {}
    virtual ~Bill() = default;                                  // virtual destructor (A2.3)

    virtual double total() const noexcept = 0;                  // pure virtual
    virtual const char* type() const noexcept = 0;              // pure virtual
    virtual std::string_view packageCode() const noexcept = 0;  // pure virtual

    void print(std::ostream& os) const                          // non-virtual, uses virtual calls
    {
        os << type() << ' ' << patient_ << ": " << money(total()) << '\n';
    }
    const std::string& patient() const noexcept { return patient_; }

protected:
    Bill(const Bill&) = default;                                // protected: no slicing by copy
    Bill& operator=(const Bill&) = default;

private:
    std::string patient_;
};

class OutPatientBill final : public Bill {
public:
    OutPatientBill(std::string patient, double consultation, double tests)
        : Bill(std::move(patient)),
          consultation_(requireNonNegative(consultation, "consultation fee")),
          tests_(requireNonNegative(tests, "test charges")) {}
    double total() const noexcept override { return consultation_ + tests_; }
    const char* type() const noexcept override { return "OP"; }
    std::string_view packageCode() const noexcept override { return "OP-STD"; }

private:
    double consultation_, tests_;
};

class InPatientBill final : public Bill {
public:
    InPatientBill(std::string patient, int days, double perDay, double procedures, double medicines)
        : Bill(std::move(patient)),
          days_(days),
          perDay_(requireNonNegative(perDay, "room charge per day")),
          procedures_(requireNonNegative(procedures, "procedure charges")),
          medicines_(requireNonNegative(medicines, "medicine charges"))
    {
        if (days < 0) throw NegativeChargeError("number of days must not be negative");
    }
    double total() const noexcept override { return days_ * perDay_ + procedures_ + medicines_; }
    const char* type() const noexcept override { return "IP"; }
    std::string_view packageCode() const noexcept override { return "IP-GEN"; }

private:
    int days_;
    double perDay_, procedures_, medicines_;
};

class DayCareBill final : public Bill {
public:
    DayCareBill(std::string patient, double packagePrice, std::string code)
        : Bill(std::move(patient)),
          price_(requireNonNegative(packagePrice, "package price")),
          code_(std::move(code)) {}
    double total() const noexcept override { return price_; }
    const char* type() const noexcept override { return "DC"; }
    std::string_view packageCode() const noexcept override { return code_; }

private:
    double price_;
    std::string code_;
};

class EmergencyBill final : public Bill {
public:
    EmergencyBill(std::string patient, double baseCharge, double hours, double ratePerHour)
        : Bill(std::move(patient)),
          base_(requireNonNegative(baseCharge, "emergency base charge")),
          hours_(requireNonNegative(hours, "hours")),
          rate_(requireNonNegative(ratePerHour, "hourly rate")) {}
    double total() const noexcept override { return base_ + hours_ * rate_; }
    const char* type() const noexcept override { return "EM"; }
    std::string_view packageCode() const noexcept override { return "EM-STD"; }

private:
    double base_, hours_, rate_;
};

// ---------------------------------------------------------------- payers (A3)
class Payer {
public:
    virtual ~Payer() = default;
    // Amount paid by the third party; the patient owes total() - amountCovered().
    // May throw ClaimError (and derived classes).
    virtual double amountCovered(const Bill& bill) const = 0;
    virtual std::string name() const = 0;
};

class SelfPay final : public Payer {
public:
    double amountCovered(const Bill&) const override { return 0.0; }
    std::string name() const override { return "SelfPay"; }
};

class Insurance final : public Payer {
public:
    Insurance(double coverLimit, double copayPercent)
        : coverLimit_(requireNonNegative(coverLimit, "cover limit")), copay_(copayPercent)
    {
        if (copayPercent < 0 || copayPercent > 100) throw BillingError("co-payment percentage must be 0..100");
    }
    double amountCovered(const Bill& bill) const override
    {
        const double payable = bill.total() * (100.0 - copay_) / 100.0;
        if (payable > coverLimit_)
            throw ClaimExceedsCoverError("claim of " + money(payable) + " exceeds policy cover of " + money(coverLimit_));
        return payable;
    }
    std::string name() const override { return "Insurance"; }

private:
    double coverLimit_, copay_;
};

class GovernmentScheme final : public Payer {
public:
    using RateTable = std::map<std::string, double, std::less<>>;
    explicit GovernmentScheme(RateTable rates) : rates_(std::move(rates))
    {
        for (const auto& entry : rates_) requireNonNegative(entry.second, "package rate");
    }
    double amountCovered(const Bill& bill) const override
    {
        const auto it = rates_.find(bill.packageCode());
        if (it == rates_.end())
            throw UnknownPackageError("unknown package code '" + std::string(bill.packageCode()) + "'");
        return std::min(it->second, bill.total());
    }
    std::string name() const override { return "Government"; }

private:
    RateTable rates_;
};

// ---------------------------------------------------------------- records and service (A4, A5)
class PatientRegistry {
public:
    void add(std::string name) { names_.insert(std::move(name)); }
    void require(std::string_view name) const
    {
        if (names_.find(name) == names_.end())
            throw PatientRecordError("no patient record for '" + std::string(name) + "'");
    }

private:
    std::set<std::string, std::less<>> names_;
};

struct Settlement {
    std::string payer;
    double total;
    double covered;
    double patientPays;
    bool fellBack;
};

class BillingService {
public:
    explicit BillingService(const PatientRegistry& registry) noexcept : registry_(registry) {}

    // Layer 1: pure computation; may throw PatientRecordError / ClaimError family.
    Settlement settle(const Bill& bill, const Payer& payer) const
    {
        registry_.require(bill.patient());
        const double covered = payer.amountCovered(bill);
        const double total = bill.total();
        return {payer.name(), total, covered, total - covered, false};
    }

    // Layer 2: log and rethrow the SAME exception object with "throw;".
    Settlement settleLogged(const Bill& bill, const Payer& payer, std::ostream& log) const
    {
        try {
            return settle(bill, payer);
        } catch (const BillingError& e) {
            log << "[audit] " << bill.type() << ' ' << bill.patient() << " via " << payer.name() << ": "
                << e.what() << '\n';
            throw;
        }
    }

    // Layer 3: recover from claim problems (bill is never lost); everything else propagates.
    Settlement settleOrFallback(const Bill& bill, const Payer& payer, std::ostream& log) const
    {
        try {
            return settleLogged(bill, payer, log);
        } catch (const ClaimError& e) {
            log << "[recovery] claim rejected (" << e.what() << "); patient billed in full\n";
            const SelfPay self;
            Settlement s = settle(bill, self);
            s.fellBack = true;
            return s;
        }
    }

private:
    const PatientRegistry& registry_;
};

// ---------------------------------------------------------------- processing all bills (A1.3)
class DayBook {
public:
    void add(std::unique_ptr<Bill> bill) { bills_.push_back(std::move(bill)); }
    double total() const noexcept
    {
        double sum = 0;
        for (const auto& b : bills_) sum += b->total();   // late binding picks the right total()
        return sum;
    }
    std::size_t size() const noexcept { return bills_.size(); }
    void print(std::ostream& os) const
    {
        for (const auto& b : bills_) b->print(os);
    }

private:
    std::vector<std::unique_ptr<Bill>> bills_;
};

}  // namespace hosp
