# Q7 Design

## Data
A complex impedance has:
- real part
- imaginary part

## Required operations
- addition
- subtraction
- multiplication
- division
- equality
- formatted output
- magnitude

## AC formulas
- Resistor: `ZR = R`
- Inductor: `ZL = jωL`
- Capacitor: `ZC = -j/(ωC)`
- Series: `Zseries = Z1 + Z2 + Z3`
- Parallel: `Zparallel = (Z1 × Z2) / (Z1 + Z2)`

## Constructors
- default constructor
- real-part constructor
- real-and-imaginary-parts constructor

## Complexity
Every arithmetic operation is O(1) time and O(1) additional space because a complex number contains two doubles.

`std::complex<double>` provides similar asymptotic complexity and is preferable for production code when its existing behavior is sufficient. The custom class is useful for demonstrating operator overloading and explicit division-by-zero handling.
