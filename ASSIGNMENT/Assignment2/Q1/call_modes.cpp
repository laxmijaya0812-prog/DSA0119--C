#include <iostream>

void changeByValue(int fee, int percent) {
    fee -= fee * percent / 100;
}

void changeByReference(int& fee, int percent) {
    fee -= fee * percent / 100;
}

void changeByPointer(int* fee, int percent) {
    if (fee != nullptr) {
        *fee -= *fee * percent / 100;
    }
}

int main() {
    int valueFee = 1000;
    int referenceFee = 1000;
    int pointerFee = 1000;

    changeByValue(valueFee, 10);
    changeByReference(referenceFee, 10);
    changeByPointer(&pointerFee, 10);

    std::cout << "By value   : " << valueFee << '\n';
    std::cout << "By reference: " << referenceFee << '\n';
    std::cout << "By pointer : " << pointerFee << '\n';
}
