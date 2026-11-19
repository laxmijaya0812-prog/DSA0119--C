#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

class Complex {
    double real;
    double imag;

public:
    Complex() : real(0.0), imag(0.0) {}
    explicit Complex(double r) : real(r), imag(0.0) {}
    Complex(double r, double i) : real(r), imag(i) {}

    double realPart() const { return real; }
    double imagPart() const { return imag; }

    Complex operator+(const Complex& z) const {
        return Complex(real + z.real, imag + z.imag);
    }

    Complex operator-(const Complex& z) const {
        return Complex(real - z.real, imag - z.imag);
    }

    Complex operator*(const Complex& z) const {
        return Complex(real * z.real - imag * z.imag,
                       real * z.imag + imag * z.real);
    }

    Complex operator/(const Complex& z) const {
        double denominator = z.real * z.real + z.imag * z.imag;
        if (denominator == 0.0) {
            throw std::domain_error("division by zero impedance");
        }

        return Complex(
            (real * z.real + imag * z.imag) / denominator,
            (imag * z.real - real * z.imag) / denominator
        );
    }

    bool operator==(const Complex& z) const {
        constexpr double eps = 1e-9;
        return std::abs(real - z.real) < eps &&
               std::abs(imag - z.imag) < eps;
    }

    double magnitude() const {
        return std::hypot(real, imag);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& z) {
        os << std::fixed << std::setprecision(4)
           << z.real;
        if (z.imag >= 0) {
            os << " + j" << z.imag;
        } else {
            os << " - j" << std::abs(z.imag);
        }
        return os;
    }
};

Complex resistor(double R) {
    return Complex(R, 0.0);
}

Complex inductor(double omega, double L) {
    return Complex(0.0, omega * L);
}

Complex capacitor(double omega, double C) {
    return Complex(0.0, -1.0 / (omega * C));
}

int main() {
    const double omega = 100.0;
    const double R = 10.0;
    const double L = 0.05;
    const double C = 0.002;

    Complex Zr = resistor(R);
    Complex Zl = inductor(omega, L);
    Complex Zc = capacitor(omega, C);

    Complex series = Zr + Zl + Zc;
    Complex parallel = (Zl * Zc) / (Zl + Zc);

    std::cout << "Resistor:  " << Zr << '\n';
    std::cout << "Inductor:  " << Zl << '\n';
    std::cout << "Capacitor: " << Zc << '\n';
    std::cout << "Series RLC: " << series
              << ", |Z| = " << series.magnitude() << '\n';
    std::cout << "Parallel LC: " << parallel
              << ", |Z| = " << parallel.magnitude() << '\n';

    try {
        Complex zero;
        std::cout << (Zr / zero) << '\n';
    } catch (const std::domain_error& e) {
        std::cout << "Zero-division test: " << e.what() << '\n';
    }
}
