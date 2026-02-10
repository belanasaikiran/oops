#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <new>
#include <string>
using namespace std;

// ========================================
// STEP 1: Memory Manager Class Definition
// ========================================

class MemoryManager {
private:
  // TODO: Define data structures to track memory allocations
  // Hint: Consider using a map to store address -> allocation info
  //
  // STEP 1 - Memory Manager Class:
  // □ Define AllocationInfo struct with size, timestamp, and other metadata
  // □ Use std::map<void*, AllocationInfo> to track allocations
  // □ Add counters for total allocations, deallocations, bytes used
  // □ Implement allocateMemory() and deallocateMemory() methods

  struct AllocationInfo {
    size_t size;
    time_t timestamp;
    // TODO: Add other useful tracking information
    bool isArray;
    string location; // location
    AllocationInfo(size_t s = 0, bool arr = false, const string &loc = "")
        : size(s), timestamp(time(nullptr)), isArray(arr), location(loc) {}
  };

  // TODO: Add member variables for tracking
  // - Total allocations
  // - Total deallocations
  // - Current allocated bytes
  // - Peak allocated bytes
  size_t totalAllocations;
  size_t totalDeallocations;
  size_t currentAllocatedBytes;
  size_t peakAllocatedBytes;
  size_t totalBytesAllocated;
  std::map<void *, AllocationInfo> allocations;
  ofstream logFile;

public:
  // TODO: Constructor
  MemoryManager(size_t initialSAize = 1024 * 1024)
      : totalAllocations(0), totalDeallocations(0),
        currentAllocatedBytes(0), peakAllocatedBytes(0), totalBytesAllocated(0) {
    // TODO: Initialize tracking variables
    logFile.open("memory_log.txt", ios::app);
    logToFile("Memory manager initialized");
    cout << "Memory Manager started - logging to memory_log.txt" << endl;
  }

  // TODO: Destructor - should report any remaining leaks
  ~MemoryManager() {
    // TODO: Call reportLeaks() automatically
    logToFile("Memory Manger Shutdown");
    cout << "\n=== Final Memory Report === " << endl;
    reportLeaks();
    printStatistics();

    if (logFile.is_open()) {
      logFile.close();
    }
  }

  // TODO: Method to allocate memory and track it
  void *allocateMemory(size_t size, bool isArray = false) {
    // TODO: Allocate memory using malloc
    // TODO: Track the allocation in your data structure
    // TODO: Update statistics
    // TODO: Return the allocated pointer
    if (size == 0) {
      logToFile("WARNING: Attempted to allocate 0 bytes");
      return nullptr;
    }

    void *ptr = malloc(size);
    if (!ptr) {
      logToFile("CRITICAL: Memory allocation failed for " + to_string(size) +
                " bytes");
      throw bad_alloc();
    }

    // Track the allocation
    allocations[ptr] = AllocationInfo(size, isArray, "Unknown");
    totalAllocations++;
    currentAllocatedBytes += size;
    totalBytesAllocated += size;

    if (currentAllocatedBytes > peakAllocatedBytes) {
      peakAllocatedBytes = currentAllocatedBytes;
    }

    string logMsg = "ALLOC: " + to_string(size) + " bytes at " +
                    to_string(reinterpret_cast<uintptr_t>(ptr)) +
                    (isArray ? " [ARRAY]" : " [SINGLE]");
    logToFile(logMsg);

    return ptr;
  }

  // TODO: Method to deallocate memory and update tracking
  void deallocateMemory(void *ptr, bool isArray) {
    // TODO: Check if ptr is valid and was allocated by this manager
    // TODO: Remove from tracking data structure
    // TODO: Update statistics
    // TODO: Free the memory using free()

    if (!ptr) {
      logToFile("WARNING: Attempted to delete null pointer");
      return;
    }

    auto it = allocations.find(ptr);
    if (it == allocations.end()) {
      logToFile("ERROR: Attempted to delete untracked memeory at " +
                to_string(reinterpret_cast<uintptr_t>(ptr)));
      cout << "WARNING: Deleting untracked memory!" << endl;
      return;
    }

    // check if array or single mismatch
    if (it->second.isArray != isArray) {
      string error = "ERROR: Memory type mismatch - alloated as "
                     + string(it->second.isArray ? "array" : "single") +
                            " but deleted as " +
                            string(isArray ? "array" : "single");
      logToFile(error);
      cout << "WARNING: " << error << endl;
    }

    size_t size = it->second.size;
    currentAllocatedBytes -= size;
    totalDeallocations++;

    string logMsg = "DEALLOC: " + to_string(size) + " bytes at " +
                    to_string(reinterpret_cast<uintptr_t>(ptr)) +
                    (isArray ? " [ARRAY]" : " [SINGLE]");
    logToFile(logMsg);
    allocations.erase(it);
    free(ptr);
  }

  // TODO: Method to allocate array and track it
  void *allocateArray(size_t size) {
    // TODO: Similar to allocateMemory but for arrays
    // TODO: Consider adding array-specific tracking
    return allocateMemory(size, true);
  }

  // TODO: Method to deallocate array
  void deallocateArray(void *ptr) {
    // TODO: Similar to deallocateMemory but for arrays
    deallocateMemory(ptr, true);
  }

  // ========================================
  // STEP 3: Leak Detection and Reporting
  // ========================================

  // TODO: Method to report memory leaks
  void reportLeaks() {
    // TODO: Check tracked allocations for unreleased memory
    // TODO: Display detailed leak information
    // TODO: Show total leaked bytes, number of leaks
    // TODO: Optionally write to log file
    cout << "\n=== Memory Leak Report ===" << endl;
    if (allocations.empty()) {
      cout << "✓ No memory leaks detected!" << endl;
      logToFile("LEAK REPORT: No leaks detected");
      return;
    }

    cout << "Memory Leaks detected: " << endl;
    size_t totalLeakedBytes = 0;
    int leakCount = 0;

    for (const auto &pair : allocations) {
      void *addr = pair.first;
      const AllocationInfo &info = pair.second;

      cout << " Leak #" << ++leakCount << ":" << endl;
      cout << "     Address: " << addr << endl;
      cout << "     Size: " << info.size << " bytes" << endl;
      cout << "     Types: " << (info.isArray ? "Array" : "Single") << endl;
      cout << "     Allocated: " << ctime(&info.timestamp);
      cout << endl;

      totalLeakedBytes += info.size;
    }

    cout << "Total leaked: " << leakCount << " blocks, " << totalLeakedBytes << " bytes" << endl;

    string leakLog = "LEAK REPORT: " + to_string(leakCount) + " leaks, " +
                     to_string(totalLeakedBytes) + " bytes";

    logToFile(leakLog);

  }

  // TODO: Method to get current statistics
  void printStatistics() {
    // TODO: Display current memory usage statistics
    // TODO: Show total allocations, deallocations, current usage
    cout << "\n=== Memory Usage Statistics ===" << endl;
           cout << "Total allocations: " << totalAllocations << endl;
           cout << "Total deallocations: " << totalDeallocations << endl;
           cout << "Current allocated bytes: " << currentAllocatedBytes << endl;
           cout << "Peak allocated bytes: " << peakAllocatedBytes << endl;
           cout << "Total bytes ever allocated: " << totalBytesAllocated << endl;
           cout << "Active allocations: " << allocations.size() << endl;

           if (totalAllocations > 0) {
             double avgAllocationSize =
                 static_cast<double>(totalBytesAllocated) / totalAllocations;
             cout << "Average allocation size: " << fixed << std::setprecision(2) << avgAllocationSize  << " bytes" << endl;
    }
  }



  // ========================================
  // STEP 4: Enhanced Debugging Information
  // ========================================

  // TODO: Method to log operations to file
  void logToFile(const string &message) {
    // TODO: Write memory operations to a log file with timestamps
    if (!logFile.is_open())
      return;
    time_t now = time(nullptr);
    char timeStr[26];
    ctime_r(&now, timeStr);
    timeStr[24] = '\0'; // remove new line

    logFile << "[" << timeStr << "] " << message << endl;
   logFile.flush();
  }

  // TODO: Method to validate memory integrity
  bool validateMemory() {
    // TODO: Check for corruption or inconsistencies
    // TODO: Return true if all memory is valid
    cout << "\n=== Memory Validation " << endl;

    if (totalDeallocations > totalAllocations) {
      cout << "ERROR: More deallocations than allocations!" << endl;
      return false;
    }

    size_t calculatedBytes = 0;
    for (const auto &pair : allocations) {
        calculatedBytes+=pair.second.size;
    }

    if (calculatedBytes != currentAllocatedBytes) {
      cout << "ERROR: Byte count mismatch!" << endl;
      return false;
    }

    if (calculatedBytes != currentAllocatedBytes) {
      cout << " ERROR: Byte count mismatch!" << endl;
      return false;
    }
    cout << "Memory validation passed" << endl;
    return true;
  }

  // Additional utility methods
  std::size_t getCurrentUsage() const { return currentAllocatedBytes; }
  std::size_t getPeakUsage() const { return peakAllocatedBytes; }
  std::size_t getActiveAllocations() const { return allocations.size();}
};

// Global instance of MemoryManager
// TODO: Create global instance
MemoryManager* globalMemoryManager = nullptr;

// ========================================
// STEP 2: Override Global New and Delete Operators
// ========================================

// TODO: Override global new operator
void* operator new(size_t size) {
  // TODO: Use globalMemoryManager->allocateMemory(size)
  // TODO: Handle case where globalMemoryManager is null
  if (globalMemoryManager) {
    return globalMemoryManager->allocateMemory(size, false);
  }

  // Fallback if manager not initialized
  return malloc(size);
}

// TODO: Override global delete operator
void operator delete(void* ptr) noexcept {
  // TODO: Use globalMemoryManager->deallocateMemory(ptr)
  // TODO: Handle case where globalMemoryManager is null
  if (globalMemoryManager) {
      globalMemoryManager->deallocateMemory(ptr, false);
  } else if (ptr) {
    free(ptr);
  }
}

// TODO: Override global new[] operator for arrays
void* operator new[](size_t size) {
  // TODO: Use globalMemoryManager->allocateArray(size)
  if (globalMemoryManager) {
      return globalMemoryManager->allocateMemory(size, true);
  }
  return malloc(size);
}

// TODO: Override global delete[] operator for arrays
void operator delete[](void* ptr) noexcept {
  // TODO: Use globalMemoryManager->deallocateArray(ptr)
  if (globalMemoryManager) {
      globalMemoryManager->deallocateArray(ptr);
  } else if(ptr) {
    free(ptr);
  }
}

// ========================================
// STEP 5: Testing Framework
// ========================================

class MemoryManagerTester {
public:
  // TODO: Test basic allocation and deallocation
  static void testBasicOperations() {
    cout << "\n--- Testing Basic Operations ---" << endl;
    // TODO: Allocate some memory
    // TODO: Deallocate it
    // TODO: Check that no leaks are reported
    int *singleInt = new int(42);
    cout << "Allocated single int with value: " << *singleInt << endl;
    delete singleInt;
    cout << "Deallocated single int " << endl;

    // test basic allocation pattern
    char *buffer = new char[100];
    strcpy(buffer, "Hello, Memory Manager!");
    cout << "Allocated and populated buffer: " << buffer << endl;
    delete[] buffer;
    cout << "Deallocated buffer" << endl;

    globalMemoryManager->validateMemory();
  }

  // TODO: Test memory leak detection
  static void testLeakDetection() {
    cout << "\n--- Testing Leak Detection ---" << endl;
    // TODO: Intentionally create memory leaks
    // TODO: Call reportLeaks() to verify detection
    int *leakyInt = new int(999);
    char *leakyBuffer = new char[50];
    double *leakyArray = new double[10];

    cout << "Created 3 intentional leaks" << endl;
    cout << "Running leak detection..." << endl;
    globalMemoryManager->reportLeaks();

    // clean up the leaks
    cout << "Cleaning up test leaks ... " << endl;
    delete leakyInt;
    delete[] leakyBuffer;
    delete[] leakyArray;
  }

  // TODO: Test array operations
  static void testArrayOperations() {
    cout << "\n--- Testing Array Operations ---" << endl;
    // TODO: Test new[] and delete[] operations
    int *smallArray = new int[5];
    for (int i = 0; i < 5; i++) {
        smallArray[i] = i * i;
    }

    int *mediumArray = new int[100];
    for (int i = 0; i < 100; i++) {
        smallArray[i] = i * 0.5f;
    }

    char *largeArray = new char[1000];
    memset(largeArray, 'A', 999);
    largeArray[999] = '\0';

    cout << "Created arrays of sizes 5, 100, and 1000" << endl;
    globalMemoryManager->printStatistics();

    delete[] smallArray;
    delete[] mediumArray;
    delete[] largeArray;

    cout << "Cleaned up all arrays " << endl;
  }

  // TODO: Test error conditions
  static void testErrorConditions() {
    cout << "\n--- Testing Error Conditions ---" << endl;
    // TODO: Test double deletion
    // TODO: Test deletion of untracked memory
    // TODO: Test null pointer deletion

    // Test null pointer deletion
    cout << "Testing null pointer deletion..." << endl;
    int* nullPtr = nullptr;
    delete nullPtr;  // Should handle gracefully

    // Test zero-size allocation
    cout << "Testing zero-size allocation..." << endl;
    void *zeroPtr = globalMemoryManager->allocateMemory(0);
    if (zeroPtr) {
        globalMemoryManager->deallocateMemory(zeroPtr, false);
    }

    // Test type mismatch (allocate as single, delete as array)
         cout << "Testing allocation type mismatch..." << endl;
         int* mismatchPtr = new int(123);
         // This should generate a warning
         globalMemoryManager->deallocateArray(mismatchPtr);

         cout << "Error condition tests completed" << endl;
  }

  // TODO: Run all tests
  static void runAllTests() {
    cout << "=== Memory Manager Test Suite ===" << endl;
    // TODO: Initialize global memory manager
    // TODO: Run each test method
    // TODO: Report final results
    // TODO: Clean up
    //
    testBasicOperations();
    testArrayOperations();
    testErrorConditions();
    testLeakDetection();
    cout << "\n=== Test Suite Complete ===" << endl;
    globalMemoryManager->printStatistics();

  }
};

// ========================================
// IMAGE PROCESSING SIMULATION (Scenario Context)
// ========================================

class ImageProcessor {
private:
  int width, height;
  unsigned char *imageData;

public:
  // TODO: Constructor that allocates image data
  ImageProcessor(int w, int h) : width(w), height(h), imageData(nullptr) {
    // TODO: Set width and height
    // TODO: Allocate memory for imageData (width * height * 3 for RGB)
    cout << "Creating " << w << "x" << h << " image processor..." << endl;
    size_t dataSize = width * height * 3; // RGB
    imageData = new unsigned char[dataSize];

    // Initialize with gradient pattern
    for (int i = 0; i < width * height * 3; i++) {
        imageData[i] = i % 256;
    }
    cout << "Allocated " << dataSize << " bytes for image data" << endl;
  }

  // TODO: Destructor that cleans up image data
  ~ImageProcessor() {
    // TODO: Delete imageData if allocated
    if (imageData) {
      cout << "Cleaning up image processor (" << width << "x" << height << ")"
           << endl;
      delete[] imageData;
      imageData = nullptr;
    }
  }

  // TODO: Method to process image (simulate work)
  void processImage() {
    // TODO: Simulate image processing work
    // TODO: Maybe allocate temporary buffers
    // TODO: Don't forget to clean up temporary allocations!

    cout << "Processing " << width << "x" << height << " image..." << endl;

    // Simulate processing by allocating temporary buffers
    size_t tempSize = width * height;
    unsigned char* tempBuffer1 = new unsigned char[tempSize];
    unsigned char* tempBuffer2 = new unsigned char[tempSize];

    // simulate some processing work
    for (size_t i = 0; i < tempSize; i++) {
        tempBuffer1[i] = imageData[i * 3]; // extract red channel
        tempBuffer1[i] = imageData[i * 3 + 1]; // extract green channel
    }

    cout << "Image processing complete (used " << (tempSize * 2)
         << " bytes temp memory) " << endl;

    // clean up temp buffers
    delete[] tempBuffer1;
    delete[] tempBuffer2;
  }

  // TODO: Method to resize image (demonstrates reallocation)
  void resize(int newWidth, int newHeight) {
    // TODO: Allocate new buffer
    // TODO: Copy data if needed (simulate)
    // TODO: Delete old buffer
    // TODO: Update dimensions
    //
    cout << "Resizing image from " << width << "x" << height << " to "
         << newWidth << "x" << newHeight << endl;
    size_t newSize = newWidth * newHeight * 3;
    unsigned char *newData = new unsigned char[newSize];


    // Simple resize simulation (just fill with pattern)
    for (size_t i = 0; i < newSize; ++i) {
        newData[i] = (i * 255) / newSize;
    }

    // clean up old data
    delete[] imageData;

    // update to new data;
    imageData = newData;
    width = newWidth;
    height = newHeight;

    cout << "Resize complete  " << endl;


  }
};

// ========================================
// MAIN FUNCTION
// ========================================

int main() {
  cout << "=== Memory Tracking and Management System ===" << endl;
  cout << "Image Processing Application Scenario" << endl;

  // TODO: Initialize global memory manager
  globalMemoryManager = new MemoryManager();

  cout << "\n--- Phase 1: Basic Memory Manager Testing ---" << endl;
  // TODO: Run basic tests
  MemoryManagerTester::testBasicOperations();

  cout << "\n--- Phase 2: Image Processing Simulation ---" << endl;
  // TODO: Create ImageProcessor instances
  // TODO: Perform various operations
  // TODO: Demonstrate memory tracking in action
  {
        ImageProcessor img1(800, 600);
        img1.processImage();

        ImageProcessor img2(1024, 768);
        img2.processImage();

        cout << "\nMemory usage during image processing:" << endl;
        globalMemoryManager->printStatistics();

        img1.resize(400, 300);
        img2.resize(512, 384);

    }  // ImageProcessor objects destroyed here

    globalMemoryManager->printStatistics();

  cout << "\n--- Phase 3: Comprehensive Testing ---" << endl;
  // TODO: Run full test suite
    MemoryManagerTester::runAllTests();

  cout << "\n--- Phase 4: Final Memory Report ---" << endl;
  // TODO: Generate final memory usage report
  // TODO: Check for any remaining leaks
    globalMemoryManager->validateMemory();
  // TODO: Clean up global memory manager
    delete globalMemoryManager;
    globalMemoryManager = nullptr;

  cout << "\n=== Program Complete ===" << endl;
  return 0;
}

/*
IMPLEMENTATION GUIDE:

STEP 1 - Memory Manager Class:
□ Define AllocationInfo struct with size, timestamp, and other metadata
□ Use std::map<void*, AllocationInfo> to track allocations
□ Add counters for total allocations, deallocations, bytes used
□ Implement allocateMemory() and deallocateMemory() methods

STEP 2 - Global Operator Overrides:
□ Override operator new to use MemoryManager
□ Override operator delete to use MemoryManager
□ Override operator new[] and delete[] for arrays
□ Handle null pointer cases gracefully

STEP 3 - Leak Detection:
□ Implement reportLeaks() to show unfreed memory
□ Display useful information: address, size, allocation time
□ Count total leaked blocks and bytes
□ Optionally write detailed reports to log file

STEP 4 - Enhanced Debugging:
□ Add timestamps to track allocation timing
□ Implement file logging for persistent debugging
□ Add memory validation checks
□ Include stack trace information if possible

STEP 5 - Testing Framework:
□ Test basic allocation/deallocation cycles
□ Intentionally create leaks to verify detection
□ Test edge cases: double deletion, null pointers
□ Test array operations separately
□ Stress test with many allocations

SAFETY CONSIDERATIONS:
- Handle null pointers gracefully
- Prevent double deletion crashes
- Ensure thread safety if needed
- Validate memory before operations
- Clean up tracking data structures

SUCCESS CRITERIA:
- All allocations are properly tracked
- Leaks are accurately detected and reported
- No crashes from memory errors
- Clear, useful debugging information
- Comprehensive test coverage
*/
