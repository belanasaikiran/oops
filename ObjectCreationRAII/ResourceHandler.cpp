#include <fstream>
#include <iostream>
#include <string>

class ResourceHandler {

private:
  std::fstream *fileStream;
  std::string filePath;
  bool isResourceOpen;

public:
  // Default Ctr - RAII: Initialize without binding to resource
  ResourceHandler() : fileStream(nullptr), filePath(""), isResourceOpen(false) {
    std::cout << "Resource Handler Created (no resource bound)" << std::endl;
  }

  // Parameterized Ctr - RAII:: Acquire resource during Initialization
  ResourceHandler(const std::string &path)
      : filePath(path), isResourceOpen(false) {
    fileStream = new std::fstream();
    fileStream->open(filePath,
                     std::ios::in | std::ios::out |
                         std::ios::app); // in - read, out - write, app - append
    if (fileStream->is_open()) {
      isResourceOpen = true;
      std::cout << "Resource acquired: " << filePath << std::endl;
    } else {
      std::cout << "Failed to acquire resource " << filePath << std::endl;
      delete fileStream;
      fileStream = nullptr;
    }
  }

  // Copy Ctr - Safe Resource Handling
  ResourceHandler(const ResourceHandler &other)
      : filePath(other.filePath), isResourceOpen(false) {
    fileStream = new std::fstream();
    if (other.isResourceOpen) {
      fileStream->open(filePath, std::ios::in | std::ios::out | std::ios::app);
    if (fileStream->is_open()) {
      isResourceOpen = true;
      std::cout << "Resource copied: " << filePath << std::endl;
    } else {
      std::cout << "Failed to copy resource: " << filePath << std::endl;
      delete fileStream;
      fileStream = nullptr;
    }
  }
  else {
    fileStream = nullptr;
    std::cout << "Copied ResourceHandler (no active resource)" << std::endl;
  }
}

// Dtr
~ResourceHandler() {
  if (isResourceOpen && fileStream) {
    fileStream->close();
    std::cout << "Resource release: " << filePath << std::endl;
  }
  delete fileStream;
  std::cout << "Resource Handler destroyed" << std::endl;
}

bool isOpen() const { return isResourceOpen; }

// write data to resource
void writeData(const std::string &data) {
  if (isResourceOpen && fileStream) {
    *fileStream << data << std::endl;
    std::cout << "Data written to " << filePath << std::endl;
  } else {
    std::cout << "Cannot write - resource not available" << std::endl;
  }
}
}
;

int main() {
  std::cout << "=== Testing ResourceHandler ===\n";

  // default Ctr
  ResourceHandler handler1;
  std::cout << "Handler1 is open: " << handler1.isOpen() << std::endl;

  // Testing Parameterized Ctr
  {
    ResourceHandler handler2("test_file.txt");
    std::cout << "Handler 2 is open: " << handler2.isOpen() << std::endl;
    if (handler2.isOpen()) {
      handler2.writeData("Testing RAII resource Management");
    }

    // testing copy Ctr
    ResourceHandler handler3 = handler2;
    std::cout << "Handler3 is open: " << handler3.isOpen() << std::endl;
    std::cout << "\n=== Objects going out of scope ===" << std::endl;
  }

  // Invalid Resource
  ResourceHandler handler4("invalid/path/file.txt");
  std::cout << "Handler4 is open: " << handler4.isOpen() << std::endl;
  handler4.writeData("This should fail");
  std::cout << "\n=== Program ending ===\n";
  return 0;
}
