#include <iostream>
#include <new>
#include <string>


using namespace  std;

// Function to safely allocate memory with error handling

int* createDataArray(int size, const string& arrayName){
  if(size <= 0){
    cout << "Error: Invalid array size " << size << " for " << arrayName << endl;
    return nullptr;
  }

  try{
    cout << "Allocating " << size << " integers for " << arrayName << "..." << endl;
    int* array = new int[size];
    cout << "Successfully allocated memory at address: " << array << endl;
    return array;
  }
  catch (const bad_alloc& e){
    cout << "Memory allocation failed for " << arrayName << ": " << e.what() << endl;
    return nullptr;
  }

}

// Function to safely deallocate memory
void destroyDataArray(int* &array, const string& arrayName){
  if(array != nullptr){
    cout << "Deallocating "  << arrayName << " at address: " << array << endl;
    delete[] array;
    array = nullptr; // prevent dangling pointer
    cout << arrayName << " Successfully deallocated and pointer set to null. " << endl;
  }else{
    cout << "Warning: " << arrayName <<  " is already null - no deallocation needed" << endl;
  }
}

int main(){
  cout << "=== Laboratory Data Processing System ===" << endl;
  int experimentSize;
  cout << "Enter number of data points for experiment: ";
  cin >> experimentSize;

  // safe allocation with error handling
  int* experimentData = createDataArray(experimentSize, "ExperimentData");
  if(experimentData == nullptr){
    cout <<  "Failed to allocate experiment data. Exiting. " << endl;
    return 1;
  }
  // initialize array with sample data
  cout <<  "Initializing experiment data..." << endl;
  for(int i = 0; i < experimentSize; i++){
    experimentData[i] =  (i+1)*10; //  sample  data: 10, 20, 30, ...
  }

  // Display first few data points"
  cout << "First " << min(5, experimentSize) << " data points: ";
  for(int i = 0; i < min(5, experimentSize);i++){
    cout << experimentData[i] << " ";
  }
  cout << endl;

  // Process data (simulate some computation)
  cout << "Processing data..." << endl;
  long long sum = 0;
  for (int i = 0; i < experimentSize; i++) {
    sum += experimentData[i];;
  }

  cout << "Data processsing complete. Sum: " << sum << endl;

  // Safe cleanup
  destroyDataArray(experimentData, "ExperimentData");
  return 0;
}
