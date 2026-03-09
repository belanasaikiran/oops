#include <exception>
#include <iostream>
#include <stdexcept>


// Defines a custom exception class (e.g., InvalidDataException) that inherits
// from std::exception.
// Implements a function that processes input data and
// throws the custom exception if invalid data is detected.
// Uses a try-catch
// block in main() to handle the exception gracefully, printing a meaningful
// error message.
// Demonstrates at least one other standard exception (e.g.,
// std::runtime_error or std::out_of_range) being thrown and caught in the same
// program.

class customException : public std::exception {
    std::string message;
  public:
    explicit customException(const std::string &mesg) : message(mesg) {}
    const char *what() const noexcept override {
      return message.c_str();
    }
};

void divide(int a, int b) {
  if (b == 0) {
    throw std::runtime_error("[ERROR]: Division by zero is not allowed");
  }
  std::cout << "Res: " << a / b << std::endl;
}


int main() {
  int i = 99;
  try {
    if (i % 2 != 0) {
      std::string message = "[ERROR]: number "  + std::to_string(i) + " divided by 2 is not zero";
      throw customException(message);
      }
  } catch (const customException &e) {
    std::cerr << e.what() << std::endl;
  } catch (const std::exception e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }


  // usong std::runtime_error
  try {
    divide(4, 0);
  } catch (const std::runtime_error &e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
  }

  return 0;
}


// Why using exceptions is generally more robust than using error codes.
// Exceptions provide improveness in correctness, safety and code clarity.
// If we use error codes, the programmer must manually check the return value.
// if they forget, the program silently continues to an invalid state.

// One situation where using error codes instead of exceptions might be more
// appropriate. Exceptions are generally expensive since it needs to unwind the
// stack to find the right catch. So for critical systems which need peformance, it's better to use errorcodes.
