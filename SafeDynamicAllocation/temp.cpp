#include <iostream>

using namespace std;

struct IntArrayManager {
  size_t size;
  int* ptr;
  IntArrayManager(size_t s) : size(s) {}

  void memoryAlloc() {
    ptr = new int[size];

    int *temp_ptr = ptr;
    int s = static_cast<int>(size);
    while (s > 0) {
      *temp_ptr = 0;
      temp_ptr++;
      s--;
    }
  }

  void printArr() {
      int *temp_ptr = ptr;
      int s = static_cast<int>(size);
      cout << "Array Contents: [";
      while (s > 0) {
        cout << *temp_ptr;
        temp_ptr++;
        s--;
        if (s > 0) {
          cout << ", ";
        }
      }
      cout << "]" << endl;
  }

  ~IntArrayManager() {
    cout << "Cleaning up the array " << endl;
    delete[] ptr;
    ptr = nullptr;
  }

};

int main() {
IntArrayManager mgr(10);
mgr.memoryAlloc();
mgr.printArr();


cout << "Program ending..." << endl;
return 0;
}
