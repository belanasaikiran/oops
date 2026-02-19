#include <iostream>
#include <string>

using namespace std;

// TODO: Design your own inheritance system
// Example: Electronic Device hierarchy
class ElectronicDevice {
private:
  string brand;
  string model;

protected:
  int powerConsumption;
  bool isOn;

public:
  ElectronicDevice(const string &b, const string &m, int powerCons)
      : brand(b), model(m) {
    powerConsumption = powerCons;
    isOn = false;
  }
  ~ElectronicDevice() {
    cout << "Destroying electronic device " << brand << " - " << model << endl;
  }

  void displaySpecs() {
    cout << "Brand: " << brand << " | model: " << model
         << " | Power Consumption: " << powerConsumption << " | On: " << isOn
         << endl;
  }

  void turnOn() {
    if (!isOn) {
      cout << "Turning on " << brand << " - " << model << endl;
      isOn = true;
    } else {
      cout << "Already On!" << endl;
    }
  }

  void turnOff() {
    if (isOn) {
      cout << "Turning off " << brand << " - " << model << endl;
      isOn = false;
    } else {
      cout << "Already Off!" << endl;
    }
  }

  string getBrand() const { return brand; }
  // TODO: Add appropriate members and access control
  // Consider: brand, model, powerConsumption, isOn
  // Methods: turnOn(), turnOff(), displaySpecs()
};
class Smartphone : public ElectronicDevice {
private:
  int screenSize;
  int storageCapacity;
  int batteryLife;

public:
  Smartphone(const string &b, const string &m, int powerCons,
             int sSize, int storage, int battery)
      : ElectronicDevice(b, m, powerCons), storageCapacity(storage),
        batteryLife(battery) {}

  ~Smartphone() {
    cout << "Destroying smart phone " << ElectronicDevice::getBrand() << endl;
  }

  // TODO: Add phone-specific features
  // Consider: screenSize, storageCapacity, batteryLife
  // Methods: makeCall(), sendText(), takePicture()
  //
  void makeCall(const string &number) {
    cout << "making a call on " << ElectronicDevice::getBrand()
         << " to " << number << endl;
  }

  void sendText(const string &number) {
    cout << "sending a text on " << ElectronicDevice::getBrand()
         << " to " << number << endl;
  }

  void takePicture() {
    cout << "taking a picture on device: " << ElectronicDevice::getBrand()
         << endl;
  }
};
class Laptop : public ElectronicDevice {
private:
  int ramSize;
  string processorType;
  int screenSize;

public:
  Laptop(const string &b, const string &m, int powerCons, int ram,
         const string &processor, int ssize)
      : ElectronicDevice(b, m, powerCons), ramSize(ram),
        processorType(processor), screenSize(ssize) {
    cout << "Created a new Laptop device: " << ElectronicDevice::getBrand()
         << endl;
  }
  ~Laptop() {
    cout << "Destroying laptop device: " << ElectronicDevice::getBrand()
         << endl;
  }

  // TODO: Add laptop-specific features
  // Consider: ramSize, processorType, screenSize
  // Methods: runProgram(), connectToWifi(), saveFile()

  void runProgram(const string &name) {
    cout << "running program " << name << " on laptop "
         << ElectronicDevice::getBrand() << endl;
  }

  void connectToWifi(const string &name) {
    cout << "connecting to wifi network " << name << " on laptop "
         << ElectronicDevice::getBrand() << endl;
  }

  void saveFile(const string &path) {
    cout << "saving file " << path << " to laptop "
            << ElectronicDevice::getBrand() << endl;
  }
};

int main() {
  ElectronicDevice electricJeep("Electric Jeep", "jeep", 400);
  Smartphone samsung("Samsung", "phone", 100, 6, 128, 100);
  Laptop lenovo("Lenovo", "laptop", 100, 128, "intel", 13);

  electricJeep.displaySpecs();
  samsung.displaySpecs();
  lenovo.displaySpecs();

  electricJeep.turnOn();
  samsung.turnOn();
  lenovo.turnOn();

  lenovo.connectToWifi("math");
  samsung.makeCall("eva");

  electricJeep.displaySpecs();
  samsung.displaySpecs();
  lenovo.displaySpecs();

  cout << "--- program ending ---\n";

  return 0;
}
