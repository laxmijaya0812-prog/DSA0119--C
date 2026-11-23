// a5_alt_manual_delete.cpp -- A5 alternative concept: free memory with delete in every catch block
// instead of RAII.
//   g++ -std=c++17 -g -fsanitize=address -DMANUAL_DELETE a5_alt_manual_delete.cpp   (manual delete)
//   g++ -std=c++17 -g -fsanitize=address                 a5_alt_manual_delete.cpp   (RAII: std::vector)
#include <iostream>
#include <stdexcept>
#include <vector>

#ifdef MANUAL_DELETE
int process(int mode)
{
    double* items = new double[100];                       // raw owning pointer
    try {
        if (mode == 1) throw std::runtime_error("bad item");
        if (mode == 2) throw std::out_of_range("index");   // error path added LATER: nobody added a delete[]
        std::cout << "  mode " << mode << ": ok\n";
    } catch (const std::runtime_error&) {
        delete[] items;                                    // has to be repeated in EVERY handler
        std::cout << "  mode " << mode << ": runtime_error handled\n";
        return -1;
    }
    delete[] items;                                        // and on the normal path
    return 0;
}
#else
int process(int mode)
{
    std::vector<double> items(100);                        // RAII: released on every path, nothing to remember
    try {
        if (mode == 1) throw std::runtime_error("bad item");
        if (mode == 2) throw std::out_of_range("index");
        std::cout << "  mode " << mode << ": ok\n";
    } catch (const std::runtime_error&) {
        std::cout << "  mode " << mode << ": runtime_error handled\n";
        return -1;
    }
    return 0;
}
#endif

int main()
{
    for (int mode = 0; mode <= 2; ++mode) {
        try {
            process(mode);
        } catch (const std::exception& e) {
            std::cout << "  mode " << mode << ": escaped to main: " << e.what() << '\n';
        }
    }
}
