#include <iostream>
#include <string>
#include <stdexcept>

class AuditoriumEvent {
    std::string name;
    int rows;
    int seatsPerRow;
    bool** seats;
    int booked;

public:
    AuditoriumEvent(const std::string& eventName, int r, int s)
        : name(eventName), rows(r), seatsPerRow(s), seats(nullptr), booked(0) {
        if (r < 0 || s < 0) {
            throw std::invalid_argument("rows and seats must be non-negative");
        }

        if (rows > 0) {
            seats = new bool*[rows];
            for (int i = 0; i < rows; ++i) {
                seats[i] = new bool[seatsPerRow] {};
            }
        }
        std::cout << "+ Constructed " << name << '\n';
    }

    ~AuditoriumEvent() {
        for (int i = 0; i < rows; ++i) {
            delete[] seats[i];
        }
        delete[] seats;

        std::cout << "- Destroyed " << name
                  << " | booked=" << booked << '\n';
    }

    bool book(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seatsPerRow || seats[r][s]) {
            return false;
        }
        seats[r][s] = true;
        ++booked;
        return true;
    }

    bool cancel(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seatsPerRow || !seats[r][s]) {
            return false;
        }
        seats[r][s] = false;
        --booked;
        return true;
    }
};

int main() {
    std::cout << "Stack object:\n";
    AuditoriumEvent mainEvent("Main Event", 3, 4);
    mainEvent.book(0, 0);
    mainEvent.book(1, 2);
    mainEvent.cancel(1, 2);

    std::cout << "\nBlock-scope object:\n";
    {
        AuditoriumEvent blockEvent("Block Event", 2, 3);
        blockEvent.book(0, 1);
    }

    std::cout << "\nHeap object:\n";
    AuditoriumEvent* heapEvent = new AuditoriumEvent("Heap Event", 2, 2);
    heapEvent->book(1, 1);
    delete heapEvent;

    std::cout << "\nZero-row test:\n";
    AuditoriumEvent emptyEvent("Empty Event", 0, 10);
}
