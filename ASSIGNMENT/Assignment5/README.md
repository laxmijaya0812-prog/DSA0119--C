# DSA01-XXXXXXXX-Assignment5 — Reliable Software with Runtime Polymorphism and Exceptions (CO5)

Case study: **Hospital Billing and Insurance Claims** (virtual functions, abstract classes, runtime
polymorphism, throwing / catching / rethrowing exceptions, RAII).

| | |
|---|---|
| **Name** | `[YOUR NAME]` |
| **Register number** | `[YOUR REGISTER NUMBER]` |
| **GitHub repository** | `https://github.com/[your-user]/DSA01-[RegisterNumber]-Assignment5` |
| **Compiler used for the results below** | g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0, Linux x86-64 — *replace with your own `g++ --version` if you re-run on another machine* |
| **Standard compile command** | `g++ -std=c++17 -Wall -Wextra <file>.cpp -o prog` |
| **Timing builds** | add `-O2` |
| **Sanitizer builds** | add `-g -fsanitize=address` |

## How to build and run

Everything at once (compiles every program, runs it, and saves the real output as `TaskXX/output_*.txt`):

```bash
chmod +x run_all.sh
./run_all.sh
```

One program by hand (example — Task B1):

```bash
cd TaskB1
g++ -std=c++17 -Wall -Wextra test_b1.cpp -o prog && ./prog
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address test_b1.cpp -o prog && ./prog      # address sanitizer
```

> Take your own terminal screenshots of the runs and save them in each `TaskXX/screenshots/` folder.
> Timings depend on the machine: re-run `./run_all.sh` on your computer and use your own numbers.

## Folder / task map

Part A (design) has its diagrams and pseudocode in the report; the folders below hold the
programs used to *test* the design claims (predict-the-output programs, alternative-concept experiments).

| Folder | Task | Files | Result in one line |
|---|---|---|---|
| `TaskA1` | A1 Polymorphic bill hierarchy | `pseudocode_A1.txt`, `a1_space_demo.cpp` | virtual and switch totals agree (87200); sizeof: 24/32/16/32 bytes per bill vs 32 for the tagged struct |
| `TaskA2` | A2 Runtime binding, vtables, virtual destructor | `predict_a2.cpp`, `predict_a2_nonvirtual_dtor.cpp`, `predict_a2_override_error.cpp`, `a2_alt_dynamic_cast.cpp`, `pseudocode_A2.txt` | `500 ~OPBill ~Bill`; without virtual destructor only `~Bill` runs; dynamic_cast chain ≈ 3× slower than a virtual call |
| `TaskA3` | A3 Abstract Payer design | `billing.hpp`, `a3_payers.cpp`, `a3_alt_stdfunction.cpp`, `pseudocode_A3.txt` | new `CorporateTieUp` payer added without editing any existing class; virtual / `std::function` / function pointer all ≈ 3 ns per call |
| `TaskA4` | A4 Exception strategy | `billing.hpp`, `a4_exceptions.cpp`, `a4_dtor_throw_demo.cpp`, `a4_alt_returncode.cpp` | every error maps to a class under `BillingError`; a throwing destructor during unwinding aborts (exit 134) |
| `TaskA5` | A5 Recovery and rethrowing | `predict_a5.cpp`, `a5_alt_manual_delete.cpp`, `pseudocode_A5.txt` | `~f ~main caught 42`; manual `delete` leaks 800 bytes on a new error path, RAII does not |
| `TaskB1` | B1 Implement and validate the billing | `billing.hpp`, `test_b1.cpp`, `complete_b1.cpp`, `complete_b1_nonvirtual_total.cpp`, `b1_runtime_binding.cpp`, `b1_virtual_dtor.cpp`, `b1_alt_templates.cpp` | 12 bill×payer cases and the day total (89700) match hand calculation; ASAN shows what a missing virtual destructor does |
| `TaskB2` | B2 Engineer error conditions | `predict_b2.cpp`, `complete_b2.cpp`, `complete_b2_throw_e.cpp`, `b2_rethrow_slicing.cpp`, `b2_error_tests.cpp`, `charges.txt`, `b2_alt_optional.cpp` | all injected errors are caught, every tracker object destroyed (26 of 26); exceptions ≈ 9× slower than `optional` at 1 % invalid, ≈ 100× at 50 % |
| `TaskB3` | B3 virtual vs switch vs variant; cost of exceptions | `b3_bills.hpp`, `b3_correctness.cpp`, `b3_benchmark.cpp`, `b3_exception_cost.cpp` | all three representations agree on 10 bills (258800); timings for 1,000 / 1,000,000 / 10,000,000 bills |
| `TaskB4` | B4 Extension, sanitizer report, decision | `billing.hpp` (unchanged), `extension.hpp`, `regression_b1.cpp`, `test_b4.cpp`, `b4_alt_plugin.cpp`, `rates.cfg`, `rates_bad.cfg` | new `MaternityBill` + `CorporateTieUp` added with zero edits to existing classes; ASAN clean |

`billing.hpp` is copied into the folders that use it; `TaskB4/output_b4_billing_unchanged.txt` shows that the
copy in `TaskB4` is byte-for-byte identical to the one in `TaskB1`.

Files named `*_error.cpp` / `*_nonvirtual_total.cpp` **fail to compile on purpose** (they show the compiler's
diagnostic); the matching `output_*.txt` file records the error message.

## Commit plan (history is checked)

Commit after each task, for example:

```bash
git add TaskA1 && git commit -m "Task A1: Bill hierarchy pseudocode and space demo"
git add TaskA2 && git commit -m "Task A2: runtime binding, vtables, predict-the-output"
...
git add TaskB4 README.md run_all.sh && git commit -m "Task B4: extension, ASAN report, decision"
```

## Design summary

* `Bill` (abstract) ← `OutPatientBill`, `InPatientBill`, `DayCareBill`, `EmergencyBill`, (`MaternityBill` in B4).
* `Payer` (abstract) ← `SelfPay`, `Insurance`, `GovernmentScheme`, (`CorporateTieUp` in A3/B4).
* `BillingError` ← `NegativeChargeError`, `ClaimError` (← `ClaimExceedsCoverError`, `UnknownPackageError`),
  `PatientRecordError`; standard exceptions (`std::invalid_argument`, `std::bad_alloc`) are handled at the top level.
* Error flow: detect and throw in constructors/payers → log and rethrow (`throw;`) in `settleLogged` →
  recover in `settleOrFallback` (bill the patient in full) → report in `main`/`ProcessDay`.
