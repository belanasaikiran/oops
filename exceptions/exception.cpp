#include <iostream>
#include <exception>

class MyException : public std::exception{
  public:
    const char* what() const noexcept override{
      return "Custom Exception occured!";
    }
};

void riskyOperation(bool throwExcep){
  if(throwExcep){
    throw MyException();
  }else{
    std::cout << "Operation successful!" << std::endl;
  }
}

int main(){
  try{
    riskyOperation(false);
  }catch(const MyException& e){
    std::cerr << "caught an exception: " << e.what() << std::endl;
  }catch(...){
    std::cout << "An unknown exception occured." << std::endl;
  }
  std::cerr << "Continuing program execution..." << std::endl;
  return 0;
}
