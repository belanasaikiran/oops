#include <iostream>
#include<vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

struct DataSet{
  string name;
  int* data;
  int size;
  chrono::time_point<chrono::steady_clock> creationTime;
  DataSet(const string& n, int s) : name(n), size(s), creationTime(chrono::steady_clock::now()){
      data = nullptr;
   }
};

class MemoryTracker{
private:
  vector<DataSet*> activeSets;
  int totalAllocations;
  int totalDeallocations;
public:
  MemoryTracker() : totalAllocations(0), totalDeallocations(0) {
    cout << "Memory Tracker Initiliazed." << endl;
  }
  bool allocateDataSet(const string& name, int size){
    if(size <= 0) {
      cout << "Error: cannot allocate dataset '" << name << "' with size " << size <<  endl;
      return false;
    }
    // check if dataset already exists
    for(DataSet* set : activeSets){
      if(set->name == name){
        cout << "Error: Dataset '" << name << "' already exists!" << endl;
        return false;
      }
    }

    try{
      DataSet* newSet = new DataSet(name, size);
      newSet->data = new int[size];

      // initialize with sample data
      for(int i = 0; i < size; i++){
        newSet->data[i] = i * 5;
      }
      activeSets.push_back(newSet);
      totalAllocations++;
      cout << "Allocated dataset '" << name << "' with " << size  << "elements" << endl;
      return true;
    }
    catch (const bad_alloc& e){
      cout  << "X Allocation failed for dataset '" << name << "': " << e.what() << endl;
      return false;
    }
  }

  bool deallocateDataSet(const string& name){
    for(auto it = activeSets.begin(); it != activeSets.end(); it++){
      if((*it)->name == name){
        DataSet* set = *it;
        cout << "Deallocating dataset '" << name  << "'..." << endl;
        delete[] set->data;
        delete set;
        activeSets.erase(it);
        totalDeallocations++;
        cout << "Dataset '" << name << "' deallocated successfully" << endl;
        return true;
      }
    }
    cout << "X Error: Dataset '" << name << "' not found for dellocation!" << endl;
    return false;
  }
  void displayActiveDataSets(){
    cout << "\n=== Active DataSets ===" << endl;
    if(activeSets.empty()){
      cout << "No active datasets." << endl;
      return;
    }

    for(DataSet* set : activeSets){
      auto duration = chrono::steady_clock::now() - set->creationTime;
      auto seconds = chrono::duration_cast<chrono::seconds>(duration).count();
      cout << "DataSet: " << set->name << " | Size: " << set->size << " | Age: " << seconds
        << "s | Address: " << set->data << endl;
    }
  }

  void detectMemoryLeaks(){
    cout << "\n=== Memory Leak Detection ===" << endl;
    cout << "Total allocations: "<< totalAllocations << endl;
    cout << "Total deallocations: " << totalDeallocations << endl;
    cout << "Active datasets: " << activeSets.size() << endl;
    if(!activeSets.empty()){
      cout << "^ WARNING: Memory leaks detected!" << endl;
      cout << "The following datasets were not properly deallocated: " << endl;
      for (DataSet*  set: activeSets){
        cout << " - " << set->name << " (" << set->size << " elements)" << endl;
      }
    }else{
      cout << "No memory leaks detected - all datasets properly cleaned up!" << endl;
    }
  }

  ~MemoryTracker(){
    cout << "\nMemoryTracker destructor called - cleaning up remaining datasets..." << endl;
    for(DataSet* set : activeSets){
      cout << "Force cleaning dataset: "<< set->name <<endl;
      delete[] set->data;
      delete set;
    }
    activeSets.clear();
  }
};



int main() {
  cout << "=== Memory Error Detection System ===" << endl;
  MemoryTracker tracker;
  // Normal operations
  cout << "\n--- Creating DataSets ---" << endl;
  tracker.allocateDataSet("Temperature", 100);
  tracker.allocateDataSet("Humidity", 50);
  tracker.allocateDataSet("Pressure", 75);
  tracker.displayActiveDataSets();
  // Demonstrate error detection
  cout << "\n--- Testing Error Detection ---" << endl;
  // Try to create duplicate dataset
  tracker.allocateDataSet("Temperature", 200); // Should fail
  // Try to create invalid dataset
  tracker.allocateDataSet("InvalidSet", -10); // Should fail
  // Try to delete non-existent dataset
  tracker.deallocateDataSet("NonExistent"); // Should fail
  // Properly deallocate some datasets
  cout << "\n--- Proper Cleanup ---" << endl;
  tracker.deallocateDataSet("Humidity");
  tracker.deallocateDataSet("Pressure");
  tracker.displayActiveDataSets();
  // Check for leaks before program ends
  tracker.detectMemoryLeaks();
  cout << "\n--- Program Ending (destructor will clean up remaining datasets) ---" << endl;
  return 0;
}
