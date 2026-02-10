#include <atomic>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// custom memory pool for the laboratory system that pre-allocates memory blocks
// to improve performance and facilitate better error tracking.

class MemoryPool {
private:
  struct Block {
    void *memory;
    size_t size;
    bool inUse;
    string owner;
    Block(size_t s) : size(s), inUse(false), owner("") {
      memory = malloc(size);
    }
    ~Block() {
      if (memory) {
        free(memory);
      }
    }
  };
  vector<Block *> pool;
  size_t totalMemory;
  size_t usedMemory;

public:
  MemoryPool(size_t initialSize = 1024 * 1024) : totalMemory(0), usedMemory(0) {
    cout << "Initializing memory pool with " << initialSize << "bytes..."
         << endl;
    // preallocate some common block sizes
    vector<size_t> blockSizes = {64, 256, 1024, 4096, 16384};
    for (size_t size : blockSizes) {
      for (int i = 0; i < 4; i++) { // 4 blocks of each size
        Block *block = new Block(size);
        if (block->memory) {
          pool.push_back(block);
          totalMemory += size;
          cout << "Pre-allocated block: " << size << " bytes at "
               << block->memory << endl;
        } else {
          delete block;
          cout << "Failed to pre-allocate block of size " << size << endl;
        }
      }
    }
    cout << "Memory pool initialized with " << pool.size() << " blocks ("
         << totalMemory << " bytes)" << endl;
  }

  void *allocate(size_t requestedSize, const string &requester) {
    if (requestedSize == 0) {
      cout << "Error: Cannot allocate 0 bytes for " << requester << endl;
      return nullptr;
    }
    // Find Suitable block
    for (Block *block : pool) {
      if (!block->inUse && block->size >= requestedSize) {
        block->inUse = true;
        block->owner = requester;
        usedMemory += block->size;
        cout << "Allocated " << block->size << " bytes to " << requester
             << requester << " at address " << block->memory << endl;
        // Clear memory for safety
        memset(block->memory, 0, block->size);
        return block->memory;
      }
    }

    // No suitable block found - allocate new one
    cout << "No suitable block found, creating new block of " << requestedSize
         << " bytes" << endl;
    Block *newBlock = new Block(requestedSize);
    if (newBlock->memory) {
      newBlock->inUse = true;
      newBlock->owner = requester;
      pool.push_back(newBlock);
      totalMemory += requestedSize;
      usedMemory += requestedSize;
      memset(newBlock->memory, 0, requestedSize);
      cout << "✓ Created and allocated new block to " << requester << " at "
           << newBlock->memory << endl;
      return newBlock->memory;
    } else {
      delete newBlock;
      cout << "✗ Failed to allocate " << requestedSize << " bytes for "
           << requester << endl;
      return nullptr;
    }
  }

  bool deallocate(void *ptr, const string &requester) {
    if (ptr == nullptr) {
      cout << "Warning: Attempted to deallocate null pointer by " << requester
           << endl;
      return false;
    }
    for (Block *block : pool) {
      if (block->memory == ptr) {
        if (!block->inUse) {
          cout << "ERROR: Double dellocation attempted by " << requester
               << " at address " << ptr << endl;
          return false;
        }
        if (block->owner != requester) {
          cout << "WARNING: " << requester
               << " is deallocating memory owned by " << block->owner << endl;
        }
        block->inUse = false;
        block->owner = "";
        // clear memory for security
        memset(block->memory, 0xFF, block->size);
        cout << "Deallocated " << block->size << " bytes from " << requester
             << endl;
        return true;
      }
    }
    cout << "✗ Error: Attempted to deallocate untracked memory by " << requester
         << " at address " << ptr << endl;
    return false;
  }

  void displayPoolStatus() {
    cout << "\n=== Memory Pool Status ===" << endl;
    cout << "Total pool size: " << totalMemory << " bytes" << endl;
    cout << "Used memory: " << usedMemory << " bytes" << endl;
    cout << "Free memory: " << (totalMemory - usedMemory) << " bytes" << endl;
    cout << "Memory utilization: " << fixed << setprecision(1)
         << (double(usedMemory) / totalMemory * 100) << "%" << endl;
    cout << "\nBlock details:" << endl;
    for (size_t i = 0; i < pool.size(); i++) {
      Block *block = pool[i];
      cout << "Block " << i << ": " << block->size << " bytes, "
           << (block->inUse ? ("USED by " + block->owner) : "FREE") << " at "
           << block->memory << endl;
    }
  }
  void detectLeaks() {
    cout << "\n=== Memory Leak Detection ===" << endl;
    bool leaksFound = false;
    for (Block *block : pool) {
      if (block->inUse) {
        cout << "⚠ LEAK: " << block->size << " bytes owned by '" << block->owner
             << "' at " << block->memory << endl;
        leaksFound = true;
      }
    }
    if (!leaksFound) {
      cout << "✓ No memory leaks detected!" << endl;
    }
  }
  ~MemoryPool() {
    cout << "Destroying memory pool..." << endl;
    detectLeaks();
    for (Block *block : pool) {
      delete block;
    }
    cout << "Memory pool destroyed. Total blocks cleaned: " << pool.size()
         << endl;
  }
};

int main() {
  cout << "=== Advanced Memory Management System ===" << endl;
  MemoryPool pool;
  // Simulate laborator operations
  cout << "\n--- Laboratory Data Processing ---" << endl;
  void *tempBuffer = pool.allocate(1024, "Temperature Processor");
  void *humiddityBuffer = pool.allocate(512, "Humidity Analyzer");
  void *pressureBuffer = pool.allocate(2048, "Pressure Processor");
  pool.displayPoolStatus();

  // Simulate some processing work
  if (tempBuffer) {
    int *tempData = static_cast<int *>(tempBuffer);
    for (int i = 0; i < 256; i++) {
      tempData[i] = i * 2;
    }
    cout << "Temperature data processed. First few values: ";
    for (int i = 0; i < 5; i++) {
      cout << tempData[i] << " ";
    }
    cout << endl;
  }
  // Test error conditions
  cout << "\n--- Testing Error Conditions ---" << endl;
  // Double deallocation
  pool.deallocate(humiddityBuffer, "HumidityAnalyzer");
  pool.deallocate(humiddityBuffer, "HumidityAnalyzer");
  // Deallocate with wrong owner
  pool.deallocate(pressureBuffer, "WrongProcessor"); // should warn

  // deallocate null pointer
  pool.deallocate(nullptr, "NullTester");

  // proper cleanup
  cout << "\n--- Proper Cleanup ---" << endl;
  pool.deallocate(tempBuffer, "TemperatureProcessor");
  // Note: pressureBuffer already deallocated by "WrongProcessor"
  pool.displayPoolStatus();
  // Allocate more to test pool expansion
  cout << "\n--- Testing Pool Expansion ---" << endl;
  void *bigBuffer =
      pool.allocate(32768, "BigDataProcessor"); // Large allocation
  pool.displayPoolStatus();
  if (bigBuffer) {
    pool.deallocate(bigBuffer, "BigDataProcessor");
  }
  cout << "\n--- Final Status ---" << endl;
  pool.detectLeaks();
  pool.displayPoolStatus();
  return 0;
}
