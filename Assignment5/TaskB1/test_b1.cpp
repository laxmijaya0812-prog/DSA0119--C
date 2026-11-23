// test_b1.cpp -- B1.1: one bill of each kind x each payer, verified against hand calculation.
#include "billing.hpp"

#include <cmath>

using namespace hosp;

struct Hand {            // values worked out by hand (see report, Table B1.1)
    double total, self, insurance, government;
};

int main()
{
    PatientRegistry registry;
    for (const char* n : {"Ravi", "Meena", "Kavita", "Arjun"}) registry.add(n);
    BillingService service(registry);

    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000, 2500));
    bills.push_back(std::make_unique<DayCareBill>("Kavita", 30000, "DC-CATARACT"));
    bills.push_back(std::make_unique<EmergencyBill>("Arjun", 3000, 4, 1250));

    const SelfPay self;
    const Insurance insurance(60000, 20);   // cover limit 60000, co-payment 20 %
    const GovernmentScheme gov({{"OP-STD", 1000}, {"IP-GEN", 45000}, {"DC-CATARACT", 25000}, {"EM-STD", 6000}});
    const Payer* payers[] = {&self, &insurance, &gov};

    const Hand hand[4] = {{1700, 0, 1360, 1000},
                          {50000, 0, 40000, 45000},
                          {30000, 0, 24000, 25000},
                          {8000, 0, 6400, 6000}};

    std::cout << "Bill  Patient  Payer       Total  Covered  PatientPays  HandCovered  Result\n";
    int failures = 0;
    for (std::size_t i = 0; i < bills.size(); ++i) {
        for (std::size_t p = 0; p < 3; ++p) {
            const Settlement s = service.settle(*bills[i], *payers[p]);
            const double expectCovered = p == 0 ? hand[i].self : p == 1 ? hand[i].insurance : hand[i].government;
            const bool ok = std::fabs(s.total - hand[i].total) < 1e-9 && std::fabs(s.covered - expectCovered) < 1e-9 &&
                            std::fabs(s.patientPays - (hand[i].total - expectCovered)) < 1e-9;
            failures += ok ? 0 : 1;
            std::cout << std::left << std::setw(6) << bills[i]->type() << std::setw(9) << bills[i]->patient()
                      << std::setw(12) << s.payer << std::right << std::setw(8) << money(s.total) << std::setw(9)
                      << money(s.covered) << std::setw(13) << money(s.patientPays) << std::setw(13)
                      << money(expectCovered) << "  " << (ok ? "PASS" : "FAIL") << '\n';
        }
    }

    DayBook book;
    book.add(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    book.add(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000, 2500));
    book.add(std::make_unique<DayCareBill>("Kavita", 30000, "DC-CATARACT"));
    book.add(std::make_unique<EmergencyBill>("Arjun", 3000, 4, 1250));
    std::cout << "\nDay book (" << book.size() << " bills):\n";
    book.print(std::cout);
    const bool dayOk = std::fabs(book.total() - 89700) < 1e-9;   // 1700 + 50000 + 30000 + 8000
    failures += dayOk ? 0 : 1;
    std::cout << "Day total = " << money(book.total()) << " (hand: 89700.00) " << (dayOk ? "PASS" : "FAIL") << '\n';
    std::cout << (failures == 0 ? "ALL TESTS PASSED\n" : "SOME TESTS FAILED\n");
    return failures == 0 ? 0 : 1;
}
