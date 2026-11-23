// extension.hpp -- B4.1: one NEW bill type and one NEW payer, added WITHOUT editing any existing class.
// billing.hpp is byte-for-byte identical to Task B1 (verified with cmp / sha256sum in run_all.sh).
#pragma once

#include "billing.hpp"

namespace hosp {

// New bill type: maternity package plus extra days beyond the package stay.
class MaternityBill final : public Bill {
public:
    MaternityBill(std::string patient, double deliveryPackage, int extraDays, double perExtraDay)
        : Bill(std::move(patient)),
          package_(requireNonNegative(deliveryPackage, "delivery package")),
          extraDays_(extraDays),
          perDay_(requireNonNegative(perExtraDay, "extra-day charge"))
    {
        if (extraDays < 0) throw NegativeChargeError("extra days must not be negative");
    }
    double total() const noexcept override { return package_ + extraDays_ * perDay_; }
    const char* type() const noexcept override { return "MT"; }
    std::string_view packageCode() const noexcept override { return "MT-DEL"; }

private:
    double package_;
    int extraDays_;
    double perDay_;
};

// New payer: employer tie-up pays pct % of the bill, never more than a per-claim cap.
class CorporateTieUp final : public Payer {
public:
    CorporateTieUp(double coveragePercent, double perClaimCap)
        : pct_(coveragePercent), cap_(requireNonNegative(perClaimCap, "per-claim cap"))
    {
        if (coveragePercent < 0 || coveragePercent > 100) throw BillingError("coverage percentage must be 0..100");
    }
    double amountCovered(const Bill& bill) const override { return std::min(bill.total() * pct_ / 100.0, cap_); }
    std::string name() const override { return "Corporate"; }

private:
    double pct_, cap_;
};

}  // namespace hosp
