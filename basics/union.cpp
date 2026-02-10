#include <iostream>

union Data{
    int integer;
    float decimal;
    char character;
};

int main(){
    Data data;
    data.integer = 42;
    std::cout << "Integer: "<< data.integer << std::endl;

    data.decimal = 3.14;
    std::cout << "Decimal: "<< data.decimal << std::endl;

    return 0;
}


// Abstraction - hiding away the unnecessary details
// Encapsulation - bundling data members and methods
// Polymorphism - one to many -> one definition name but multiple definitions based on paramaters
// Inheritance - create new classes from base class and add features
