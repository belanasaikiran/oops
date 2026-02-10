#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace  std;

struct SensorData{
  std::string sensorId;
  double reading;
  int batteryLevel;
};

class DataManager{
private:
  vector<SensorData*> sensors;
public:
  // Add sensor with safety checks
  bool addSensor(const string& id, double reading, int battery){
    if(id.empty() || battery < 0 || battery > 100){
      cout << "invalid sensor data provided" << endl;
      return false;
    }
    SensorData* newSensor = new SensorData{id, reading, battery};
    sensors.push_back(newSensor);
    cout << "Added sensor: " << id << endl;
    return true;
  }

  // Find sensor by ID using safe pointer handling
  SensorData* findSensor(const std::string& id){
    for(SensorData* sensor : sensors){
      if(sensor != nullptr  && sensor->sensorId == id) return sensor;
    }

    return nullptr; // not found
  }

  // Update sensor reading with reference for efficiency
  bool updateSensorReading(const string& id, double newReading){
    SensorData* sensor = findSensor(id);
    if(sensor != nullptr){
      sensor->reading = newReading;
      cout << "Updated sensor id "  << id << " reading to " << newReading << endl;
      return true;
    }
    cout << "Sensor " << id << " not found " << endl;
    return false;
  }

  // Display all sensors using const references for read-only access
  void displayAllSensors() const{
    cout << "\n=== All Sensors ===" << endl;
    for(const SensorData* sensor : sensors){
      if(sensor != nullptr){
        std::cout << "ID: " << sensor->sensorId
          << " | Reading: " << sensor->reading
          << " | Battery: " << sensor->batteryLevel << "%" << endl;
      }
    }
  }

  // Data cleanup with Destructor to ensure no memory leaks
  ~DataManager(){
    cout << "Cleaning up DataManager..." << endl;
    for(SensorData* sensor : sensors){
      if(sensor != nullptr){
        delete sensor;
      }
    }
    sensors.clear();
  }
};


int main(){
  DataManager manager;
  // add sensors to the system
  manager.addSensor("TEMP001", 23.5, 85);
  manager.addSensor("HUM001", 60.2, 92);
  manager.addSensor("", 15.0, 50); // invalid - empty ID
  manager.addSensor("PRESS001", 1013.25, 105); // invalid - battery > 100
  manager.displayAllSensors();

  // Update sensor readings
  manager.updateSensorReading("TEMP001", 24.8);
  manager.updateSensorReading("NONEXISENT", 0.0); // should handle gracefully
  manager.displayAllSensors();

  // Demonstrate safe pointer usage
  SensorData* tempSensor = manager.findSensor("TEMP001");
  if(tempSensor != nullptr){
    cout << "\nFound temperature sensor: " << tempSensor->reading << "°C" << endl;
    // Safe to use temp Sensor here
  }
  else{
    cout << "temperature sensor not found " << endl;
  }

  return 0;
}
