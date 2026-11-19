# DSA01-RegisterNumber-Assignment3

## Assignment 3 – CO3
**Topic:** Constructors, Destructors and Operator Overloading in C++

> Replace `RegisterNumber` with your actual register number before creating the final GitHub repository.

### Student
- Name: Lakshmi Devi J
- Register Number: __________________
- Course: C++ / Data Structures and Algorithms
- Compiler: g++ 13 or later
- Standard: C++17

### Compile command
```bash
g++ -std=c++17 -Wall -Wextra -pedantic file.cpp -o app
```

### Run
Linux/macOS:
```bash
./app
```
Windows PowerShell:
```powershell
.pp.exe
```

## Repository structure

- `Q1/` Constructors and constructor overloading
- `Q2/` Copy constructor, deep copy and Rule of Three
- `Q3/` Dynamic constructor and destructor
- `Q4/` Arithmetic and stream operator overloading
- `Q5/` Increment, comparison and subscript operators
- `Q6/` Rules of operator overloading and dynamic Text class
- `Q7/` Complex-number class for AC impedance
- `evidence/` command/output records to help capture required screenshots

## Task summary

### Q1
Demonstrates default, parameterised and overloaded constructors, constructor delegation and validation of copy count. Object construction is O(1) and uses O(1) object space.

### Q2
Demonstrates shallow-copy problems and a Rule-of-Three deep-copy implementation. The final implementation has an independent dynamically allocated medicine array.

### Q3
Demonstrates a dynamically allocated two-dimensional seat map and destructor-based memory release. The implementation also shows stack, block-scope and heap objects.

### Q4
Implements a `Money` class with `+`, `-`, `+=`, `==`, `<`, `<<` and `>>`. Money is stored as paise in `long long`.

### Q5
Implements prefix/postfix increment, comparisons and checked `operator[]` in a playlist.

### Q6
Implements a dynamically allocated `Text` class with constructor, copy constructor, assignment, destructor, concatenation and comparisons.

### Q7
Designs a `Complex` class from first principles and applies it to series and parallel RLC impedance calculations.

## Required evidence

For the final submission, run every program yourself and capture screenshots showing:
1. source code in VS Code,
2. compile command,
3. program execution and output,
4. sanitizer execution where required.

The assignment requires progressive commits, with at least one commit for each task. Use commits such as:
```text
Add Q1 constructors
Add Q2 deep copy and Rule of Three
Add Q3 dynamic constructor and destructor
Add Q4 Money operators
Add Q5 increment and subscript operators
Add Q6 Text operator overloading
Add Q7 Complex impedance calculator
```

Do not claim screenshots or measurements that you have not actually produced.
