#include <iostream>
#include <string>

using std::cout, std::endl, std::string;

class Character {
private:
  string name;

protected:
  int health;
  int speed;

public:
  Character(const string &n, int h, int s) : name(n), health(h), speed(s) {
    cout << "Character " << name << " created" << endl;
  }

  // Virtual destructor - essential for inheritance
  virtual ~Character() { cout << "Character " << name << " destroyed" << endl; }

  // virtual method for polymorphism
  virtual void displayStats() const {
    cout << "Name: " << name << ", Health: " << health << ", Speed: " << speed
         << endl;
  }

  // controlled access to private members
  string getName() const { return name; }

  // validation methods for protected members
  void setHealth(int h) { health = (h >= 0) ? h : 0; }
  int getHealth() const { return health; }
};

class Warrior : public Character {
private:
  int strength;

public:
  Warrior(const string &name, int health, int speed, int str)
      : Character(name, health, speed), strength(str) {
    cout << "Warrior " << getName() << " created with strength " << strength
         << endl;
  }

  ~Warrior() { cout << "Warrior " << getName() << " destroyed" << endl; }

  void displayStats() {
    Character::displayStats();
    cout << "Strength: " << strength << ", Class: Warrior" << endl;
  }

  void attack() {
    int damage = strength + (speed / 2);
    cout << getName() << " attacks for " << damage << " damage!" << endl;
  }
};

class Mage : public Character {
private:
  int mana;

public:
  Mage(const string &name, int health, int speed, int m)
      : Character(name, health, speed), mana(m) {
    cout << "Mage " << getName() << " created with " << mana << " mana" << endl;
  }
  ~Mage() { cout << "Mage " << getName() << " destroyed" << endl; }

  void displayStats() const override {
    Character::displayStats();
    cout << "Mana: " << mana << ", Class: Mage" << endl;
  }
  void castSpell() {
    if (mana >= 10) {
      int damage = mana / 2 + speed;
      mana -= 10;
      cout << getName() << " casts spell for " << damage << " damage!" << endl;
    } else {
      cout << getName() << " has insufficient mana!" << endl;
    }
  }
};

int main() {

  Character c("base", 1000, 100);
  Warrior w("warrior", 1000, 200, 40);
  Mage m("mage", 1000, 300, 60);

  c.getName();
  c.displayStats();
  cout << "\n";
  w.getName();
  w.displayStats();
  w.attack();
  w.displayStats();
  cout << "\n";
  m.getName();
  m.displayStats();
  m.castSpell();
  m.displayStats();

  return 0; };
