#include <iostream>
class Character {
private:
    std::string name;
    int hitPoints;
    int attackStrength;

  public:

    Character(const std::string &n, int points, int strength)
        : name(n) {
      if (points < 0 || strength < 0) {
        std::cout << "points and strength cannot be less than zero\n";
      }
      setStrength(strength);
      setPoints(points);
    }

    int getStrength() const {return attackStrength;}
    int getPoints() const {return hitPoints;}
    void setStrength(int st) {
        if (st > 0)
          attackStrength = st;
        else {
            std::cout << "strength should be greater than 0\n";
        }
    }
    void setPoints(int p) {
        if (p > 0)
          hitPoints = p;
        else {
          std::cout << "points should be greater than 0\n";
        }
    }

  void displayInfo() {
    std::cout << "Name: " << name << ", HP: " << hitPoints
              << ", Attack: " << attackStrength << std::endl;
  }


  ~Character() {
      std::cout << "Destroying character " << name << std::endl;
  }


  friend void addHP(Character& c, int hpBoost);
};

void addHP(Character& c, int hpBoost){
  std::cout << "adding extra " << hpBoost <<  " health points to " << c.name << std::endl;
  c.hitPoints += hpBoost;
  std::cout << c.name << " now has " << c.getPoints() << " hitPoints\n";
};


int main(){
  std::cout << "=== POINTS GAME ===\n";

  Character pawn("pawn", 20, 10);
  Character bishop("bishop", 60, 50);
  Character knight("knight", 70, 40);


  std::cout << "\n1. Display Characters:\n";
  pawn.displayInfo();
  bishop.displayInfo();
  knight.displayInfo();


  std::cout << "\n2. Adding extra health points\n";
  addHP(bishop, 20);
  addHP(bishop, 20);
  std::cout << "\n--- ending program ---\n";
  return 0;
}
