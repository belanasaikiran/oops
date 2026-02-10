#include <iostream>
#include <string>
using namespace std;
struct Config {
  string mode;
  double threshold;
  bool enabled;
};
// Function using reference - always valid, cannot be reassigned
void updateConfigByReference(Config &config, const string &newMode) {
  config.mode = newMode;
  config.enabled = true;
  cout << "Updated config via reference: " << config.mode << endl;
}
// Function using pointer - can be null, can be reassigned
void updateConfigByPointer(Config *config, const string &newMode) {
  if (config != nullptr) {
     // Safety check required
        config->mode = newMode;
    config->enabled = true;
    cout << "Updated config via pointer: " << config->mode << endl;
  } else {
    cout << "Cannot update: null pointer received" << endl;
  }
}

int main() {
  Config systemConfig = {"default", 50.0, false};
  Config backupConfig = {"backup", 75.0, false};

  cout << "=== Reference behavior === " << endl;
  Config &configRef = systemConfig;
  cout << "initial reference points to: " << configRef.mode << endl;
  updateConfigByReference(configRef, "production");
  // referneces cannot be reassinged to differen objects
  configRef = backupConfig;
  cout << "After 'reassginment': systemConfig.mode = " << systemConfig.mode
       << endl;
  cout << "backupConfig.mode still = " << backupConfig.mode << endl;
  cout << "\n=== Pointer behavior ===" << endl;
  Config *configPtr = &systemConfig;
  cout << "initial Pointer points to: " << configPtr->mode << endl;
  updateConfigByPointer(configPtr, "testing");
  cout << "systemConfig.mode = " << systemConfig.mode << endl;
  cout << "backupConfig.mode = " << backupConfig.mode << endl;

  // Demonstrate null pointer handling
  cout << "\n=== Null pointer Safety ===" << endl;
  Config *nullPtr = nullptr;
  updateConfigByPointer(nullptr, "invalid"); // safe handling of nullptr
  return 0;
}
