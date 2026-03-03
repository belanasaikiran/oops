#include <exception>
#include <iostream>
#include <string>

class FileIOException : public std::exception {
private:
  std::string message;

public:
  explicit FileIOException(const std::string &msg)
      : message("File IO Exception: " + msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};
void readFile(const std::string &filePath) {
  // Simulate file error
  bool fileError = true;
  if (fileError) {
    throw FileIOException("Unable to open file " + filePath);
  }
}
int main() {
  try {
    readFile("log.txt");
  } catch (const FileIOException &e) {
    std::cerr << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  return 0;
}
