// b2_rethrow_slicing.cpp -- B2: "throw;" versus "throw e;" (the type of the rethrown object).
//   g++ -std=c++17 -Wall -Wextra b2_rethrow_slicing.cpp
#include <iostream>
#include <stdexcept>

class ClaimError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    ClaimError(const ClaimError& other) : std::runtime_error(other) { ++copies; }   // count copies
    static inline int copies = 0;
};

[[noreturn]] void origin() { throw ClaimError("exceeds cover"); }

void caseA() { try { origin(); } catch (const ClaimError&)      { throw; } }      // rethrow same object
void caseB() { try { origin(); } catch (const ClaimError& e)    { throw e; } }    // copy of a ClaimError
void caseC() { try { origin(); } catch (const std::exception& e) { throw e; } }   // copy of a std::exception: SLICED
void caseD() { try { origin(); } catch (const std::exception&)  { throw; } }      // same object, type preserved

template <class F>
void run(const char* label, F f)
{
    ClaimError::copies = 0;
    try {
        f();
    } catch (const ClaimError& e) {
        std::cout << label << ": main caught ClaimError      what()=\"" << e.what() << "\"  copies made=" << ClaimError::copies << '\n';
    } catch (const std::exception& e) {
        std::cout << label << ": main caught std::exception  what()=\"" << e.what() << "\"  (type and message lost)\n";
    }
}

int main()
{
    run("A  catch(const ClaimError&)       + throw;  ", caseA);
    run("B  catch(const ClaimError& e)     + throw e;", caseB);
    run("C  catch(const std::exception& e) + throw e;", caseC);
    run("D  catch(const std::exception&)   + throw;  ", caseD);
}
