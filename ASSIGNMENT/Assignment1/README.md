# DSA01-RegisterNumber-Assignment1

## Assignment 1 – CO1
**Title:** Design, Implement and Validate the Fare Logic of a City Metro Smart-Card System

> Replace `RegisterNumber` with your actual register number before creating the final GitHub repository.

### Student
- Name: Lakshmi Devi J
- Register Number: __________________
- Compiler: g++ 13 or later
- Standard: C++17

### Compile command
```bash
g++ -std=c++17 -Wall -Wextra -pedantic file.cpp -o app
```

### Run on Windows PowerShell
```powershell
.pp.exe
```

### Repository structure

```text
Q1/   Object-oriented analysis
Q2/   Tokens, keywords, identifiers and constants
Q3/   Data types and type compatibility
Q4/   Implicit conversions + predicted output
Q5/   Operators and precedence + predicted output
Q6/   Control structures + complete code
Q7/   Procedural vs OOP
Q8/   Safe input/output
Q9/   Complete fare implementation and tests
Q10/  Expected vs observed execution
Q11/  Final design recommendation
Q12/  Day-4 design challenge
evidence/   Commands and screenshot checklist
```

### Required evidence

The assignment requires each task to be executed individually and requires input files and output screenshots in the repository. It also requires progressive commits, at least one per task.

Do not add fabricated screenshots, timings, sanitizer output or measured values. Run the programs on your computer and place the actual evidence in `evidence/`.

Suggested commits:
```text
Add Task 1 OOP analysis
Add Task 2 constants and enums
Add Task 3 data type analysis
Add Task 4 conversion experiments
Add Task 5 precedence experiments
Add Task 6 fare control logic
Add Task 7 procedural and OOP versions
Add Task 8 safe input output
Add Task 9 fare tests
Add Task 10 execution comparison
Add Task 11 final recommendation
Add Task 12 fourth-zone redesign
```

## Main design

Money is represented in integer paise to avoid ordinary floating-point rounding issues.

Base fares:
- Inner: Rs 10
- Middle: Rs 20
- Outer: Rs 30

Peak surcharge:
- 20%
- Peak: 08:00–09:59 and 17:00–19:59

Minimum balance after a journey:
- Rs 50

Task 12 extends the design with:
- Airport zone: Rs 60
- 10% weekend discount
- weekend discount does not combine with peak surcharge
- monthly pass for two selected zones
