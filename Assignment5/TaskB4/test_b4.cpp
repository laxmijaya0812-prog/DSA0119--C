// test_b4.cpp -- B4.1 / B4.2: new bill + new payer working with the unchanged code, plus error paths.
//   g++ -std=c++17 -Wall -Wextra -g -fsanitize=address test_b4.cpp
#include "extension.hpp"

#include <cmath>
#include <limits>
#include <new>

using namespace hosp;

static int failures = 0;
static void check(const std::string& what, double got, double expected)
{
    const bool ok = std::fabs(got - expected) < 1e-9;
    failures += ok ? 0 : 1;
    std::cout << (ok ? "PASS  " : "FAIL  ") << std::left << std::setw(46) << what << std::right << std::setw(10) << money(got)
              << "  (hand " << money(expected) << ")\n";
}

int main()
{
    PatientRegistry registry;
    for (const char* n : {"Ravi", "Meena", "Kavita", "Arjun", "Lakshmi"}) registry.add(n);
    const BillingService service(registry);

    const SelfPay self;
    const Insurance insurance(60000, 20);
    const GovernmentScheme gov({{"OP-STD", 1000}, {"IP-GEN", 45000}, {"DC-CATARACT", 25000}, {"EM-STD", 6000}, {"MT-DEL", 30000}});
    const CorporateTieUp corporate(90, 30000);

    // Maternity: 35000 + 2 x 3000 = 41000
    const MaternityBill maternity("Lakshmi", 35000, 2, 3000);
    std::cout << "--- new bill type (MT) with every payer ---\n";
    check("MT total", maternity.total(), 41000);
    check("MT covered by SelfPay", service.settle(maternity, self).covered, 0);
    check("MT covered by Insurance (80 % of 41000)", service.settle(maternity, insurance).covered, 32800);
    check("MT covered by Government (package 30000)", service.settle(maternity, gov).covered, 30000);
    check("MT covered by Corporate (90 % = 36900, cap 30000)", service.settle(maternity, corporate).covered, 30000);

    // New payer with the four OLD bill types: 1530, 30000, 27000, 7200
    std::cout << "--- new payer (Corporate) with the old bill types ---\n";
    const OutPatientBill op("Ravi", 500, 1200);
    const InPatientBill ip("Meena", 3, 2500, 40000, 2500);
    const DayCareBill dc("Kavita", 30000, "DC-CATARACT");
    const EmergencyBill em("Arjun", 3000, 4, 1250);
    check("OP covered by Corporate (90 % of 1700)", service.settle(op, corporate).covered, 1530);
    check("IP covered by Corporate (cap)", service.settle(ip, corporate).covered, 30000);
    check("DC covered by Corporate (90 % of 30000)", service.settle(dc, corporate).covered, 27000);
    check("EM covered by Corporate (90 % of 8000)", service.settle(em, corporate).covered, 7200);

    // The day total uses the SAME DayBook::total() code as before.
    DayBook book;
    book.add(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    book.add(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000, 2500));
    book.add(std::make_unique<DayCareBill>("Kavita", 30000, "DC-CATARACT"));
    book.add(std::make_unique<EmergencyBill>("Arjun", 3000, 4, 1250));
    book.add(std::make_unique<MaternityBill>("Lakshmi", 35000, 2, 3000));
    std::cout << "--- one day book holding old and new bill types ---\n";
    check("day total (89700 old + 41000 new)", book.total(), 130700);

    // Error paths: thrown AND caught (objects are destroyed by unwinding; ASAN must stay silent)
    std::cout << "--- error paths with the new classes ---\n";
    auto expect = [&](const char* label, auto&& fn) {
        try { fn(); std::cout << "FAIL  " << label << ": no exception\n"; ++failures; }
        catch (const ClaimError& e)   { std::cout << "PASS  " << std::left << std::setw(40) << label << std::right << "ClaimError: " << e.what() << '\n'; }
        catch (const BillingError& e) { std::cout << "PASS  " << std::left << std::setw(40) << label << std::right << "BillingError: " << e.what() << '\n'; }
        catch (const std::bad_alloc& e) { std::cout << "PASS  " << std::left << std::setw(40) << label << std::right << "bad_alloc: " << e.what() << '\n'; }
        catch (...) { std::cout << "PASS  " << std::left << std::setw(40) << label << std::right << "unknown exception type\n"; }
    };
    expect("negative extra-day charge", [&] { MaternityBill b("Lakshmi", 35000, 2, -3000); });
    expect("negative days", [&] { MaternityBill b("Lakshmi", 35000, -1, 3000); });
    expect("MT unknown to old government table", [&] { service.settle(maternity, GovernmentScheme({{"OP-STD", 1000}})); });
    expect("MT above insurance cover", [&] { service.settle(maternity, Insurance(1000, 0)); });
    expect("missing patient record", [&] { const MaternityBill b("Stranger", 1, 0, 0); service.settle(b, self); });
    expect("invalid corporate percentage", [&] { CorporateTieUp c(150, 100); });
    expect("overflowing array request", [&] {
        volatile std::size_t n = std::numeric_limits<std::size_t>::max() / 4;
        double* p = new double[n];
        delete[] p;
    });
    expect("non-std exception", [&] { MaternityBill b("Lakshmi", 1, 1, 1); throw 7; });
    std::ostringstream log;
    const Settlement s = service.settleOrFallback(maternity, Insurance(1000, 0), log);
    check("fallback keeps the bill (patient pays all)", s.patientPays, 41000);
    std::cout << log.str();

    std::cout << (failures == 0 ? "ALL TESTS PASSED\n" : "SOME TESTS FAILED\n");
    return failures == 0 ? 0 : 1;
}
