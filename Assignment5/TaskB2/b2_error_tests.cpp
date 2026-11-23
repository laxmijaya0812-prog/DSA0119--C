// b2_error_tests.cpp -- B2: engineer error conditions and show what each test reveals.
//   g++ -std=c++17 -Wall -Wextra b2_error_tests.cpp
//   ./a.out               (full run, includes a genuine std::bad_alloc)
//   ./a.out --asan-safe   (for -fsanitize=address: skips the 32 TiB request that ASAN itself aborts on)
#include "billing.hpp"

#include <cstring>
#include <fstream>
#include <limits>
#include <new>

using namespace hosp;

// ---- RAII tracker: prints in its destructor so unwinding is visible --------------------
struct Tracker {
    static inline int created = 0, destroyed = 0;
    std::string name;
    explicit Tracker(std::string n) : name(std::move(n)) { ++created; std::cout << "    [+] " << name << " created\n"; }
    ~Tracker() { ++destroyed; std::cout << "    [-] " << name << " destroyed\n"; }
    Tracker(const Tracker&) = delete;
    Tracker& operator=(const Tracker&) = delete;
};

// ---- top-level wrapper: every task is isolated; the program keeps running ---------------
template <class F>
void runTask(const std::string& name, F&& task)
{
    std::cout << "TEST " << name << '\n';
    try {
        Tracker outer(name + "/outer");
        task();
        std::cout << "  result: completed normally\n";
    } catch (const ClaimError& e) {
        std::cout << "  result: ClaimError (" << e.what() << ")\n";
    } catch (const BillingError& e) {
        std::cout << "  result: BillingError (" << e.what() << ")\n";
    } catch (const std::bad_alloc& e) {
        std::cout << "  result: std::bad_alloc (" << e.what() << ")\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "  result: std::invalid_argument (" << e.what() << ")\n";
    } catch (const std::out_of_range& e) {
        std::cout << "  result: std::out_of_range (" << e.what() << ")\n";
    } catch (const std::exception& e) {
        std::cout << "  result: std::exception (" << e.what() << ")\n";
    } catch (...) {
        std::cout << "  result: catch(...) - exception of unknown type\n";
    }
}

double parseAmount(const std::string& text)
{
    std::size_t pos = 0;
    const double v = std::stod(text, &pos);          // may throw invalid_argument / out_of_range
    if (pos != text.size()) throw BillingError("trailing characters in number '" + text + "'");
    return v;
}

int main(int argc, char** argv)
{
    const bool asanSafe = argc > 1 && std::strcmp(argv[1], "--asan-safe") == 0;
    PatientRegistry registry;
    registry.add("Ravi");
    registry.add("Meena");
    BillingService service(registry);

    // 1. text and negative numbers from an input file ------------------------------------
    std::ifstream in("charges.txt");
    if (!in) { std::cout << "cannot open charges.txt\n"; return 1; }
    std::string line;
    while (std::getline(in, line)) {
        runTask("input '" + line + "'", [&] {
            Tracker inner("bill for '" + line + "'");
            const double amount = parseAmount(line);
            OutPatientBill bill("Ravi", amount, 0);   // constructor rejects negative charges
            std::cout << "    accepted charge " << money(bill.total()) << '\n';
        });
    }
    std::cout << "NOTE std::stod(\"12x\") on its own returns " << std::stod("12x") << " and does not throw\n\n";

    // 2. claim above cover, unknown package, missing patient -----------------------------
    runTask("claim above cover (settle)", [&] {
        const InPatientBill bill("Meena", 3, 2500, 40000, 2500);
        service.settle(bill, Insurance(1000, 0));
    });
    runTask("unknown package (settle)", [&] {
        const DayCareBill bill("Meena", 30000, "DC-UNKNOWN");
        service.settle(bill, GovernmentScheme({{"DC-CATARACT", 25000}}));
    });
    runTask("missing patient record", [&] {
        const OutPatientBill bill("Nobody", 500, 100);
        service.settle(bill, SelfPay());
    });
    runTask("recovery: claim above cover (settleOrFallback)", [&] {
        const InPatientBill bill("Meena", 3, 2500, 40000, 2500);
        const Settlement s = service.settleOrFallback(bill, Insurance(1000, 0), std::cout);
        std::cout << "    bill kept: payer=" << s.payer << " patient pays " << money(s.patientPays)
                  << " fellBack=" << std::boolalpha << s.fellBack << '\n';
    });

    // 3. memory allocation failure -------------------------------------------------------
    runTask("allocation: overflowing array size", [&] {
        Tracker inner("array request");
        volatile std::size_t n = std::numeric_limits<std::size_t>::max() / 4;
        double* p = new double[n];
        delete[] p;
    });
    if (!asanSafe) {
        runTask("allocation: 32 TiB request", [&] {
            Tracker inner("array request");
            volatile std::size_t n = std::size_t(1) << 45;
            char* p = new char[n];
            delete[] p;
        });
    } else {
        std::cout << "TEST allocation: 32 TiB request\n  skipped (--asan-safe)\n";
    }

    // 4. catch(...) at the top level keeps the program alive -----------------------------
    runTask("non-std exception: throw 7", [&] {
        Tracker a("frame 1 object"), b("frame 2 object");
        throw 7;
    });
    runTask("non-std exception: throw \"text\"", [&] {
        Tracker a("frame 1 object");
        throw "plain C string";
    });
    runTask("normal task after the failures", [&] { std::cout << "    still running\n"; });

    std::cout << "\nTrackers created = " << Tracker::created << ", destroyed = " << Tracker::destroyed
              << (Tracker::created == Tracker::destroyed ? "  -> every object was destroyed\n" : "  -> LEAK\n");
    return Tracker::created == Tracker::destroyed ? 0 : 1;
}
