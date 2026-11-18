# DSA01 – Assignment 4: Designing Reusable Class Hierarchies

**Student name:** [Enter your name]  
**Register number:** [Enter your register number]  
**Course:** Object Oriented Programming with C++ (DSA01)  
**Compiler:** g++ [enter version]  
**Language standard:** C++17

## Repository structure

```text
DSA01-<RegisterNumber>-Assignment4/
├── README.md
├── Report/
│   └── Assignment4_Report.md
├── Task01/
│   └── task01.cpp
├── Task02/
│   └── task02.cpp
├── Task03/
│   └── task03.cpp
├── Task04/
│   └── task04.cpp
├── Task05/
│   └── task05.cpp
└── Task06/
    └── task06.cpp
```

## Compile and run

Open a terminal in the relevant task folder. Compile each program using:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic task01.cpp -o task01
./task01
```

Replace `task01` with the corresponding source and executable name for each task. On Windows, run `task01.exe`.

For the memory-safety experiment in Task 4, use:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -fsanitize=address -g task04.cpp -o task04
./task04
```

AddressSanitizer availability depends on the compiler/platform. The repaired program should not report invalid deletion or leaks attributable to the program.

## Task results

- **Task 1:** Replaces duplicated payroll classes with a common abstract `Staff` class and overridden pay calculations.
- **Task 2:** Models `Person`, `Student`, `Staff`, `TeachingAssistant`, and `ResearchScholar`; virtual inheritance prevents duplicate `Person` subobjects.
- **Task 3:** Demonstrates base/member/derived construction and destruction order, initializer lists, and the `Address` member object.
- **Task 4:** Demonstrates static versus dynamic binding, virtual destruction, and safe polymorphic ownership with `std::unique_ptr`.
- **Task 5:** Demonstrates direct objects, pointers, references, dynamic allocation, `this`, method chaining, and searching a collection of cart pointers.
- **Task 6:** Implements an extensible vehicle-rental hierarchy with an abstract base class and virtual rental-charge calculation.

## GitHub submission checklist

1. Rename this folder to `DSA01-<RegisterNumber>-Assignment4`.
2. Replace the student placeholders in this file and the report.
3. Compile and execute every program with the stated command.
4. Add screenshots of your own terminal outputs under the corresponding task folders (not supplied as fabricated evidence).
5. Commit each task separately, for example:
   - `Add Task 1 payroll hierarchy`
   - `Add Task 2 university hierarchy`
   - `Add Task 3 constructor demonstrations`
   - `Add Task 4 polymorphism and ownership`
   - `Add Task 5 object pointers and this`
   - `Add Task 6 vehicle rental hierarchy`
6. Create the GitHub repository with the required name and paste its URL into the report cover details.
7. Share the repository with the course faculty.
