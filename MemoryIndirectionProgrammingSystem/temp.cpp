#include <iostream>

using std::cout , std::endl;

int main(){
  int a = 10;
  int* p = &a;
  int &addr = a;

  addr++;
  (*p)++;
  cout << "addr: " << addr << endl;
  cout << "*p: " << *p << endl;

  return 0;
}
