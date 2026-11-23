// a4_alt_returncode.cpp -- A4 alternative concept: report errors by return codes (enum Status)
// instead of exceptions. Same three layers, same inputs, same results -- more code.
//   g++ -std=c++17 -Wall -Wextra a4_alt_returncode.cpp
#include <iostream>
#include <stdexcept>

// ---------------- return-code version ----------------
enum class Status { Ok, NegativeCharge, ExceedsCover };
const char* text(Status s)
{
    switch (s) {
        case Status::Ok: return "ok";
        case Status::NegativeCharge: return "negative charge";
        case Status::ExceedsCover: return "claim exceeds cover";
    }
    return "?";
}
Status validateRC(double billed, double cover)             // layer 1
{
    if (billed < 0 || cover < 0) return Status::NegativeCharge;
    if (billed > cover) return Status::ExceedsCover;
    return Status::Ok;
}
Status approveRC(double billed, double cover, double& out) // layer 2: must forward every error by hand
{
    Status s = validateRC(billed, cover);
    if (s != Status::Ok) return s;
    out = billed;
    return Status::Ok;
}
Status settleRC(double billed, double cover, double& out)  // layer 3: and again
{
    Status s = approveRC(billed, cover, out);
    if (s != Status::Ok) return s;
    return Status::Ok;
}

// ---------------- exception version ----------------
struct NegativeCharge : std::runtime_error { using std::runtime_error::runtime_error; };
struct ExceedsCover : std::runtime_error { using std::runtime_error::runtime_error; };
void validateEx(double billed, double cover)
{
    if (billed < 0 || cover < 0) throw NegativeCharge("negative charge");
    if (billed > cover) throw ExceedsCover("claim exceeds cover");
}
double approveEx(double billed, double cover) { validateEx(billed, cover); return billed; }   // nothing to forward
double settleEx(double billed, double cover) { return approveEx(billed, cover); }             // nothing to forward

int main()
{
    const double cases[][2] = {{40000, 50000}, {60000, 50000}, {-1, 50000}};
    for (const auto& c : cases) {
        double paid = 0;
        const Status s = settleRC(c[0], c[1], paid);
        std::cout << "return code: " << (s == Status::Ok ? "approved " + std::to_string(int(paid)) : text(s)) << "   |   ";
        try {
            const int paidEx = int(settleEx(c[0], c[1]));
            std::cout << "exception: approved " << paidEx << '\n';
        } catch (const std::exception& e) {
            std::cout << "exception: " << e.what() << '\n';
        }
    }
    // Lines that exist only to forward errors in the return-code version: 1 (approveRC) + 1 (settleRC)
    // + the caller's check = 3 per call chain; the exception version needs 0 forwarding lines.
    // A caller that ignores the returned Status compiles and silently continues with a wrong 'paid'.
}
