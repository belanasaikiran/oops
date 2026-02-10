#include <iostream>
#include <string>

class Character{
private:
    int health_;
    std::string name_;
    int level_;

public:
    Character(std::string name, int health, int level) : name_(name), health_(health), level_(level) {}

    void displayCharacterInfo(){
        std::cout << "Character: "<< name_ << std::endl;
        std::cout << "Health: "<< health_ << std::endl;
        std::cout << "Level: "<< level_ << std::endl;
    };

    // we can make the following an Interface
    void takeDamage(int damage){
        health_ -= damage;
        std::cout << name_ << "takes " << damage << " damage!" << std::endl;
    }
};




int main(){

    // HEAP Allocated Objects - must be freed
    // Character* knight = new Character("Knight", 100, 1);
    // Character* wizard = new Character("wizard", 100, 1);
    // knight->displayCharacterInfo(); // member functions are accessible using '->'


    // Stack Allocated Objects
    Character knight("Knight", 100, 5);
    Character wizard("Wizard", 80, 8);


    knight.displayCharacterInfo(); // member func() are accessible using '.'
    wizard.displayCharacterInfo();

    std::cout << "Battle Begins!" << std::endl;

    knight.takeDamage(25);
    wizard.takeDamage(15);

    knight.displayCharacterInfo();
    wizard.displayCharacterInfo();


    // delete knight, wizard;

    std::cin.get();
    return 0;
};
