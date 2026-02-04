#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

class Environement{
  std::string description;
  std::vector<int> exits;
  std::vector<int> items;

public:

  Environement(std::string desc) : description(desc) {
    std::cout << "Description: " << description << std::endl;
  }


  void enter(int id){
    std::cout << id << " has entered the environment " << std::endl;
    items.push_back(id);
  }

  void listExit(){
    std:: cout << "Exit List: [";
    for (int i = 0; i < exits.size(); i++) {
      std::cout << exits[i];
      if(i < exits.size() - 1) {
          std::cout << ",";
      }
    }
     std::cout << "]\n";

  }

  void exit(int id){
    items.erase(remove(items.begin(), items.end(), id), items.end());
    exits.push_back(id);
  }




};


int main(){
  Environement e1("This is Austin");


  e1.enter(1);
  e1.enter(2);

  e1.listExit();

  e1.exit(1);
  e1.listExit();


  return 0;
}
