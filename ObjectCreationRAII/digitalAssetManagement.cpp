#include <iostream>
#include <string>
#include <chrono>
#include <ctime>


class DigitalAsset {
private:
  std::string fileName;
  std::string fileType;
  double fileSizeMB;
  std::string creationDate;
  bool isActive;
  static int totalAssets; // track total number of assets created

public:
 DigitalAsset() : fileName("untitled"), fileType("unknown"), fileSizeMB(0.0), isActive(true){
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    creationDate = std::ctime(&time_t);
    creationDate.pop_back(); // remove new line
    totalAssets++;
    std::cout << "Default asset created: " << fileName << " | Total Assets: " << totalAssets << std::endl;
  }


  // Parameterized Ctr
  DigitalAsset(const std::string& name, const std::string& type, double size) : fileName(name), fileType(type), fileSizeMB(size), isActive(true) {
     auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    creationDate = std::ctime(&time_t);
    creationDate.pop_back(); // remove new line
    totalAssets++;
    std::cout << "Asset created: " << fileName << " (" << fileType << ") | Total Assets: " << totalAssets << std::endl;
}


  // Copt Ctr
  DigitalAsset(const DigitalAsset& other) : fileName(other.fileName), fileType(other.fileType), creationDate(other.creationDate), isActive(other.isActive) {
  totalAssets++;
std::cout << " Asset Copied: "<< fileName << " from " << other.fileName << " | Total Assets: " << totalAssets << std::endl;
  }

  // Destructor
  ~DigitalAsset(){
    totalAssets--;
    std::cout << "Asset: " << fileName << " [" << fileName << "] - " << fileSizeMB << "MB - Created: " << creationDate << " - Status: " << (isActive ? "Active" : "Archived") << std::endl;
  }

  void displayInfo() const {
    std::cout << "Asset: " << fileName << " [" << fileType << "] - "
        << fileSizeMB << "MB - Created: " << creationDate
        << " - Status: " << (isActive ? "Active" : "Archived") << std::endl;
  }

  void archive(){
    isActive = false;
    std::cout << "Asset " << fileName << " has been archived." << std::endl;
  }

  static  int getTotalAssets(){
    return totalAssets;
  }
};

// Initialize Static Member
int DigitalAsset::totalAssets = 0;

int main(){
  std::cout << "=== Digital Asset Management System ===" << std::endl;
  std::cout << "Initial Total Assets: " << DigitalAsset::getTotalAssets << std::endl << std::endl;

  // Test default Ctr
  std::cout << "1. Creating default Asset: " << std::endl;
  DigitalAsset defaultAsset;
  defaultAsset.displayInfo();
  std::cout << std::endl;


  // Test Parameterized Ctr
  std::cout << "2. Creating Specific assets: " << std::endl;
  DigitalAsset logo("company_logo.png", "image", 2.5);
  logo.displayInfo();
  DigitalAsset video("promo_vide.mp4", "video", 150.0);
  video.displayInfo();
  std::cout << std::endl;


  // Test Cpt Ctr and demonstrate lifecycle
  std::cout << "3. Testing Cpy Ctr: "<< std::endl;

  {
    DigitalAsset logoCopy = logo; // copy ctr called
    logoCopy.displayInfo();
    logoCopy.archive();
    std::cout << "--- logoCopy going out of scope ---" << std::endl;
  } // logoCopy Destructor called here

  std::cout << "\n4. Final Status: " << std::endl;
  std::cout << "Total Assets remaining: " << DigitalAsset::getTotalAssets() << std::endl;
  std::cout << "\n=== Program ending - remaining objects will be destroyed ===" << std::endl;
  return 0;

}
