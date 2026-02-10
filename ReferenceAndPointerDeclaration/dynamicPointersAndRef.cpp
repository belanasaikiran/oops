#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct DataPoint{
  double value;
  string label;
  int timestamp;
};



// Function to create a new data point dynamically
DataPoint* createDataPoint(double val, const string& lbl, int time){
  DataPoint* newData = new DataPoint;
  newData->value = val;
  newData->label = lbl;
  newData->timestamp = time;
  cout << "Created data point to address:  " << newData << endl;
  return newData;
}

// Function safely delete a data pointer
void deleteDataPointer(DataPoint*& dataPtr){
  if(dataPtr != nullptr){
    cout << "Deletinig data point: " << dataPtr->label << endl;
    delete dataPtr;
    dataPtr = nullptr; // preventing dangling pointer
  }
}
//
// void processDataPoint(DataPoint& data){
//   data.value *= 1.1; // Apply 10% scaling factor
//   data.timestamp += 1000; // update timestamp
//   cout << "Processed: " << data.label << " = " << data.value << endl;
// }
//
// // function using const reference for read only access
// void displayDataPoint(const DataPoint& data){
//   cout << "Data: " << data.label  << " | Value: " << data.value <<  " | Time: " << data.timestamp << endl;
// }

int main(){
  // Create data points using pointers
  DataPoint* humidity = createDataPoint(65.5, "Humidity", 2000);
  DataPoint* pressure = createDataPoint(1013.5, "Pressure", 2000);


  // Access data through pointers
  if(humidity != nullptr){
    cout << "Humidity: " << humidity->value << endl;
    cout << "Address stored in humidity pointer: " << humidity << endl;
    cout << "Address of humidity pointer itself: " << &humidity << endl;
    humidity->value += 5.0;
    cout << "Updated Humidity: " << humidity->value << "%" << endl;
  }

  // Pointer Arithmetic Demo
  cout << "\n=== Pointer Arithmetic ===" << endl;
  DataPoint data[3] = {
    {10.5, "Sensor1", 1000},
    {21.2, "Sensor2", 1001},
    {30.5, "Sensor3", 1002},
  };

  DataPoint* arrayPtr = data;
  for(int i = 0; i  < 3; i++){
    cout << "Elemen  " << i << ": " << arrayPtr->label
      << " = " << arrayPtr->value << endl;
    arrayPtr++;
  }


  // Clean up dynamicay allocated memory
  deleteDataPointer(humidity);
  deleteDataPointer(pressure);

  cout << "Humidity pointer after deletion: " << humidity << endl;
  cout << "Pressure pointer after deletion: " << pressure << endl;

  return 0;
}
