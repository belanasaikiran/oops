#include <exception>
#include <iostream>
#include <string>
class BankingException : public std::exception {
private:
  std::string message;

public:
  explicit BankingException(const std::string &msg) : message(msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};
void testBasicException() {
  try {
    throw BankingException("Basic banking error occurred");
  } catch (const BankingException &e) {
    std::cout << "Caught exception: " << e.what() << std::endl;
  }
}
int main() {
  testBasicException();
  return 0;
}
