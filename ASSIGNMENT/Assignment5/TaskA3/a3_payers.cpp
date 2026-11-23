// a3_payers.cpp -- A3.1 payer interface and A3.2 extension without touching any existing class.
//   g++ -std=c++17 -Wall -Wextra a3_payers.cpp
#include "billing.hpp"

#include <type_traits>

using namespace hosp;

// ---- A3.2: NEW payer, written in this file only; billing.hpp is not modified ------------
class CorporateTieUp final : public Payer {
public:
    CorporateTieUp(double coveragePercent, double perClaimCap) : pct_(coveragePercent), cap_(perClaimCap) {}
    double amountCovered(const Bill& bill) const override
    {
        return std::min(bill.total() * pct_ / 100.0, cap_);   // employer pays pct %, never more than the cap
    }
    std::string name() const override { return "Corporate"; }

private:
    double pct_, cap_;
};

// Generic code written BEFORE CorporateTieUp existed: it works with every Payer, old or new.
double totalCovered(const Payer& payer, const std::vector<std::unique_ptr<Bill>>& bills)
{
    double sum = 0;
    for (const auto& b : bills) sum += payer.amountCovered(*b);
    return sum;
}

int main()
{
    static_assert(std::is_abstract_v<Payer>, "Payer must be abstract");
    static_assert(!std::is_abstract_v<Insurance> && !std::is_abstract_v<GovernmentScheme> && !std::is_abstract_v<SelfPay>,
                  "concrete payers must implement every pure virtual function");

    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000, 2500));
    bills.push_back(std::make_unique<DayCareBill>("Kavita", 30000, "DC-CATARACT"));
    bills.push_back(std::make_unique<EmergencyBill>("Arjun", 3000, 4, 1250));

    const SelfPay self;
    const Insurance insurance(60000, 20);
    const GovernmentScheme gov({{"OP-STD", 1000}, {"IP-GEN", 45000}, {"DC-CATARACT", 25000}, {"EM-STD", 6000}});
    const CorporateTieUp corporate(90, 30000);   // NEW payer: 90 % up to 30000 per claim

    const Payer* payers[] = {&self, &insurance, &gov, &corporate};
    std::cout << "Covered amount per bill (OP, IP, DC, EM) and total for each payer:\n";
    for (const Payer* p : payers) {
        std::cout << std::left << std::setw(11) << p->name() << std::right;
        for (const auto& b : bills) std::cout << std::setw(10) << money(p->amountCovered(*b));
        std::cout << "  | total " << std::setw(10) << money(totalCovered(*p, bills)) << '\n';
    }
    // Corporate by hand: OP 1700*0.9=1530 ; IP min(45000,30000)=30000 ; DC min(27000,30000)=27000 ; EM 8000*0.9=7200
}
