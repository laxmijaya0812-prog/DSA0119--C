# Task 1 – Object-Oriented Analysis

## Objects

### SmartCard
Data:
- card ID
- balance in paise
- optional student/senior status

Behaviour:
- recharge
- canTravel
- deduct fare
- report balance

### Passenger
Data:
- passenger name/ID
- concession eligibility

Behaviour:
- determine applicable concession

### Journey
Data:
- origin/destination or farthest zone
- hour
- peak status

Behaviour:
- validate journey details
- request fare

### Zone
Represents Inner, Middle and Outer zones.

### FareTable
Data:
- base fare per zone
- peak surcharge
- minimum required balance

Behaviour:
- calculate exact fare

## OOP principles

- Encapsulation: SmartCard keeps balance private.
- Abstraction: FareTable exposes fare calculation without exposing implementation details.
- Inheritance: a future passenger hierarchy could derive StudentPassenger and SeniorPassenger from Passenger.
- Polymorphism: a future fare-policy interface could allow StandardFarePolicy and WeekendFarePolicy to implement different rules.

## Mandatory rule

After deducting a journey fare, the card must retain at least Rs 50.

## Procedural vs OOP

A procedural design would keep balances and fare values in variables and use free functions. OOP groups data with behaviour, so a rule such as minimum balance is easier to protect inside SmartCard.

Complexity per journey is O(1) and memory per card is O(1) in either design.
