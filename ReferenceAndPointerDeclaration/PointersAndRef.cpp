#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct DataPoint{
  double value;
  string label;
  int timestamp;
};

void processDataPoint(DataPoint& data){
  data.value *= 1.1; // Apply 10% scaling factor
  data.timestamp += 1000; // update timestamp
  cout << "Processed: " << data.label << " = " << data.value << endl;
}

// function using const reference for read only access
void displayDataPoint(const DataPoint& data){
  cout << "Data: " << data.label  << " | Value: " << data.value <<  " | Time: " << data.timestamp << endl;
}

int main(){
  DataPoint sensor1  = {25.5, "Temperature", 1000};
  cout << "=== Before Processing ===" << endl;
  displayDataPoint(sensor1);

  // Create reference to the data Data point
  DataPoint& sensorRef = sensor1;
  cout << "reference value: " << sensorRef.value << endl;
  cout << "Origina;l and reference have same address: " <<  (&sensor1 == &sensorRef ? 
                                                             "true" : "false") << endl;
  processDataPoint(sensorRef);
  cout << "\n=== After Processing ===" << endl;
  displayDataPoint(sensor1);
  return 0;
}
