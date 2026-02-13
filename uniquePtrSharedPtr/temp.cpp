#include <iostream>
#include <memory>

using namespace std;

int main(){
  auto arr = std::make_unique<int>(10);
  cout << *arr << endl;
  auto newArr = std::move(arr);
  cout << *newArr << endl;

  if (arr) {
      cout << "arr: " << *arr << endl;
  } else {
    cout << "expired arr" << endl;
  }

  // RAII - Resource Acquistion is Initialization
  // it ties the resource lifespan to the life of an object. When the object is
  // destroyed, the resources are releases automatically.
  // It makes sure memory allocated using the unique_ptr will be released when
  // it is no longer needed.
  // Unique_ptr is used for objects that only need single reference.
  // It cannot be copied but it can be moved.
  // Unique_ptr enables a single ownership for an 0bject




  return 0;
}
