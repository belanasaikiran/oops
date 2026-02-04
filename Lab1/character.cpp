#include <iostream>
#include <string>


using std::cout, std::endl, std::string;

class Character{
private:
  string name;
  int health;
public:
  Character(string n, int h) : name(n), health(h) {}
  void displayStatus(){
    cout << "Character: " << name << endl;
    cout << "Health: " << health << endl;
    cout << "-------------------------" << endl;
  }
};


int main(){

  Character knight("Knight", 70);
  Character King("King", 100);

  knight.displayStatus();
  King.displayStatus();

  return 0;
}
