// b4_alt_plugin.cpp -- B4 alternative concept: new bill types described by rules in a configuration
// file ("plug-ins") and evaluated by one generic interpreter, versus compiled C++ classes (virtual).
//   g++ -std=c++17 -O2 -Wall -Wextra b4_alt_plugin.cpp ; ./a.out
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

struct ConfigError : std::runtime_error { using std::runtime_error::runtime_error; };

// ---- interpreted rules ---------------------------------------------------------------------
struct Rule {
    std::string name;
    double base, perUnit;
    std::string unit;
    double cap;
    double eval(double units) const noexcept
    {
        const double t = base + perUnit * units;
        return (cap > 0 && t > cap) ? cap : t;
    }
};

std::vector<Rule> loadRules(const std::string& path)
{
    std::ifstream in(path);
    if (!in) throw ConfigError("cannot open " + path);
    std::vector<Rule> rules;
    std::string line;
    for (int lineNo = 1; std::getline(in, line); ++lineNo) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ls(line);
        Rule r;
        if (!(ls >> r.name >> r.base >> r.perUnit >> r.unit >> r.cap))
            throw ConfigError(path + ":" + std::to_string(lineNo) + ": malformed rule '" + line + "'");
        if (r.base < 0 || r.perUnit < 0 || r.cap < 0)
            throw ConfigError(path + ":" + std::to_string(lineNo) + ": negative value in rule " + r.name);
        for (const Rule& other : rules)
            if (other.name == r.name) throw ConfigError(path + ":" + std::to_string(lineNo) + ": duplicate rule " + r.name);
        rules.push_back(std::move(r));
    }
    return rules;
}

// ---- compiled classes (the same three bill types written in C++) ---------------------------------
struct CBill {
    virtual ~CBill() = default;
    virtual double total() const noexcept = 0;
};
struct CMaternity final : CBill { double units; explicit CMaternity(double u) : units(u) {} double total() const noexcept override { return 35000 + 3000 * units; } };
struct CPhysio final : CBill    { double units; explicit CPhysio(double u) : units(u) {}    double total() const noexcept override { const double t = 800 * units; return t > 20000 ? 20000 : t; } };
struct CDialysis final : CBill  { double units; explicit CDialysis(double u) : units(u) {}  double total() const noexcept override { return 1500 + 1200 * units; } };

struct Record { std::string rule; std::size_t index; double units; };

template <class F>
double medianMs(F&& f, int trials = 5)
{
    std::vector<double> t;
    for (int i = 0; i < trials; ++i) {
        const auto a = std::chrono::steady_clock::now();
        f();
        const auto b = std::chrono::steady_clock::now();
        t.push_back(std::chrono::duration<double, std::milli>(b - a).count());
    }
    std::sort(t.begin(), t.end());
    return t[t.size() / 2];
}

int main()
{
    // 1. risk: a broken plug-in file is only detected when it is loaded
    for (const char* file : {"rates_bad.cfg"}) {
        try { loadRules(file); std::cout << file << ": loaded\n"; }
        catch (const ConfigError& e) { std::cout << "load error -> " << e.what() << '\n'; }
    }

    // 2. the good file
    const std::vector<Rule> rules = loadRules("rates.cfg");
    std::unordered_map<std::string, Rule> table;
    for (const Rule& r : rules) table.emplace(r.name, r);
    std::cout << "loaded " << rules.size() << " rules from rates.cfg; sizeof(Rule) = " << sizeof(Rule)
              << " bytes; sizeof(compiled bill object) = " << sizeof(CMaternity) << " bytes\n";
    try { table.at("MATERNTY"); }                                    // a typo in a bill's rule name
    catch (const std::out_of_range&) { std::cout << "typo 'MATERNTY' -> detected only at run time (unknown rule)\n"; }

    // 3. speed: 1,000,000 bills, random rule per bill
    const std::size_t n = 1'000'000;
    std::mt19937 rng(5);
    std::vector<Record> recs(n);
    std::vector<std::unique_ptr<CBill>> compiled;
    compiled.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        const std::size_t k = rng() % 3;                              // order in rates.cfg: MATERNITY, PHYSIO, DIALYSIS
        const double units = double(1 + rng() % 10);
        recs[i] = {rules[k].name, k, units};
        if (k == 0) compiled.push_back(std::make_unique<CMaternity>(units));
        else if (k == 1) compiled.push_back(std::make_unique<CPhysio>(units));
        else compiled.push_back(std::make_unique<CDialysis>(units));
    }
    double sA = 0, sB = 0, sC = 0;
    volatile double sink = 0;
    const double tA = medianMs([&] { sA = 0; for (const auto& b : compiled) sA += b->total(); sink = sA; });
    const double tB = medianMs([&] { sB = 0; for (const Record& r : recs) sB += rules[r.index].eval(r.units); sink = sB; });
    const double tC = medianMs([&] { sC = 0; for (const Record& r : recs) sC += table.find(r.rule)->second.eval(r.units); sink = sC; });
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "1,000,000 bills (median of 5)\n";
    std::cout << "  compiled class, virtual call              : " << std::setw(8) << tA << " ms  " << tA * 1e6 / n << " ns/bill\n";
    std::cout << "  interpreter, rule index pre-resolved      : " << std::setw(8) << tB << " ms  " << tB * 1e6 / n << " ns/bill\n";
    std::cout << "  interpreter, rule found by name per bill  : " << std::setw(8) << tC << " ms  " << tC * 1e6 / n << " ns/bill\n";
    std::cout << "  totals equal: " << (sA == sB && sB == sC ? "yes" : "NO") << "  (sum = " << sA << ")\n";
}
