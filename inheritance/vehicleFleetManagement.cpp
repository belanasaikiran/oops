#include <iostream>
#include <string>

using namespace std;
// TODO: Implement Vehicle base class
class Vehicle {
private:
    string brand;       // Immutable after creation
    int year;           // Manufacturing year
protected:
    int maxSpeed;       // Derived classes may need for calculations
    bool engineRunning; // Common to all vehicles
public:
    Vehicle(const string& b, int y, int speed)
        : brand(b), year(y), maxSpeed(speed), engineRunning(false) {
        cout << year << " " << brand << " vehicle created" << endl;
    }
    virtual ~Vehicle() {
        cout << year << " " << brand << " vehicle destroyed" << endl;
    }
    virtual void displayInfo() const {
        cout << year << " " << brand << " - Max Speed: " << maxSpeed
             << " mph, Engine: " << (engineRunning ? "Running" : "Off") << endl;
    }
    void startEngine() {
        if (!engineRunning) {
            engineRunning = true;
            cout << brand << " engine started" << endl;
        }
    }
    void stopEngine() {
        if (engineRunning) {
            engineRunning = false;
            cout << brand << " engine stopped" << endl;
        }
    }
    string getBrand() const { return brand; }
    int getYear() const { return year; }
};
// TODO: Implement Car class
class Car : public Vehicle {
private:
    int numDoors;
    bool trunkOpen;
public:
    Car(const string& brand, int year, int maxSpeed, int doors)
        : Vehicle(brand, year, maxSpeed), numDoors(doors), trunkOpen(false) {
        cout << "Car with " << doors << " doors created" << endl;
    }
    ~Car() {
        cout << "Car " << getBrand() << " destroyed" << endl;
    }
    void displayInfo() const override {
        Vehicle::displayInfo();
        cout << "Type: Car, Doors: " << numDoors
             << ", Trunk: " << (trunkOpen ? "Open" : "Closed") << endl;
    }
    void openTrunk() {
        trunkOpen = true;
        cout << getBrand() << " trunk opened" << endl;
    }
};
// TODO: Implement Truck class
class Truck : public Vehicle {
private:
    double payloadCapacity;
    double currentLoad;
public:
    Truck(const string& brand, int year, int maxSpeed, double capacity)
        : Vehicle(brand, year, maxSpeed), payloadCapacity(capacity), currentLoad(0.0) {
        cout << "Truck with " << capacity << " ton capacity created" << endl;
    }
    ~Truck() {
        cout << "Truck " << getBrand() << " destroyed" << endl;
    }
    void displayInfo() const override {
        Vehicle::displayInfo();
        cout << "Type: Truck, Capacity: " << payloadCapacity
             << " tons, Current Load: " << currentLoad << " tons" << endl;
    }
    bool loadCargo(double weight) {
        if (currentLoad + weight <= payloadCapacity) {
            currentLoad += weight;
            cout << "Loaded " << weight << " tons" << endl;
            return true;
        }
        cout << "Cannot load - would exceed capacity!" << endl;
        return false;
    }
};
