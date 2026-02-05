#include <iostream>
#include <fstream>


class SaveFile{
  std::fstream fileStream;
  std::string filePath;
  bool fileOpen;
public:
  // default ctr
  SaveFile() : fileStream(nullptr), filePath(""), fileOpen(false) {
    std::cout << "default ctr called with no file " << std::endl;
  }

  // Parameterized Ctr
  SaveFile(const std::string& path) : filePath(path), fileOpen(false) {
    fileStream = new std::fstream();
    fileStream->open(filePath, std::ios::in | std::ios::out | std::ios::app);
    if(fileStream->is_open()){
      fileOpen=true;
      std::cout << "File is Open: "<< filePath << std::endl;
    }else{
      std::cout << "Failed to open file: " << filePath << std::endl;
      delete fileStream;
      fileStream = nullptr;
    }
  }

    ~SaveFile(){
      if(fileOpen && fileStream){
        fileStream->close();
        std::cout << "file " << filePath << " is released/closed\n";
        fileOpen = false;
      }
      delete fileStream;
      std::cout << "resource destroyed\n";
    }

    bool isFileOpen() const {
      return fileOpen;
    }


    void saveGameProgress(const std::string& gameProgressData){
      if(fileOpen && fileStream){
        *fileStream << gameProgressData << std::endl;
        fileStream->flush(); // Saving without closing
        std::cout << filePath << " has been saved with data: " << gameProgressData << std::endl;
      }else{
        std::cout << "Unable to save the game data since file is_opent open\n";
      }
    }
};

int main(){

  std::cout << "--- SAVING GAME DATA ---\n";

  // default ctr
  {
    SaveFile s1;
    std::cout << "s1 open ? " << s1.isFileOpen() << std::endl;
    s1.saveGameProgress("Can't save here");
    std::cout << "-- Going out of scope --\n";
  }


  // Parameterized ctr
  SaveFile s2("test_file.txt");
  std::cout << "s2 open ? " << s2.isFileOpen() << std::endl;
  s2.saveGameProgress("UConn Huskies won tonight!");


  std::cout << "--- Program ending ---\n";

  // save game data to file
  return 0;
}
