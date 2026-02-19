#include <cinttypes>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cmath> // Required for std::numbers::pi

using namespace std;
int main() {
  cout << "=== Classification System Hierarchy Design ===" << endl;
  cout << "Duration: 45-60 minutes" << endl;
  cout << "Your task: Design and implement a complete inheritance hierarchy"
       << endl;

  // ========================================
  // STEP 1: Choose Your Classification System
  // ========================================
  cout << "\n--- Step 1: System Selection ---" << endl;
  cout << "Choose ONE of the following systems to implement:" << endl;
  cout << "A) Animal Kingdom (Mammal, Bird, Fish)" << endl;
  cout << "B) Geometric Shapes (Circle, Rectangle, Triangle)" << endl;
  cout << "C) Employee Management (Manager, Developer, Designer)" << endl;
  cout << "D) Media Library (Book, Movie, Song)" << endl;

  // TODO: Uncomment ONE system below and implement it completely

  // ========================================
  // OPTION A: ANIMAL KINGDOM SY       // TODO: Virtual destructor

  // TODO: Virtual methods for polymorphism
  // virtual void makeSound() = 0;  // Pure virtual for abstract base
  // virtual void move() { }
  // virtual void displayInfo() { }

  // TODO: Getter methods for private attributes
  //
  //
  //
  cout << "\nImplementing Geometric Shapes System..." << endl;
};

class Animal {
private:
  string name;
  int age;
  double weight;

public:
  Animal(const string &n, int a, double w) : name(n), age(a), weight(w) {
    cout << "created a new " << name << endl;
  }
  virtual ~Animal() { cout << "Animal Destructor called!" << endl; }
  virtual void makeSound() = 0;    // Pure virtual for abstract base
  virtual void move(int steps) {}; // Pure virtual for abstract base
  virtual void displayInfo() {};
  string getAnimal() const { return name; }
  int getAge() const { return age; }
  double getWeight() const { return weight; }
};

// TODO: Define Mammal derived class
// Requirements:
// - Additional attribute: furColor (string)
// - Constructor calling base constructor
// - Override makeSound() and move()
// - Add unique method: giveBirth()

class Mammal : public Animal {
private:
  // TODO: Add mammal-specific attributes
  string furColor;
  string sound;
  int pos;

public:
  // TODO: Constructor with base class initialization
  Mammal(const string &n, int a, double w, const string &color, const string &s)
      : Animal(n, a, w), furColor(color), sound(s) {
    cout << "Created mammal " << Animal::getAnimal() << endl;
  }

  ~Mammal() { cout << "Destroying mammal " << Animal::getAnimal() << endl; }

  // TODO: Override virtual methods
  void makeSound() override {
    cout << Animal::getAnimal() << " says " << sound << endl;
  }

  void move(int steps) override {
    pos += steps;
    cout << "Animal::getAnimal() moved to " << pos << endl;
  }

  // TODO: Add mammal-specific methods
  Mammal *giveBirth(const string &child) {
    cout << Animal::getAnimal() << " is giving birth to " << child << endl;
    return new Mammal(child, 0, 2, furColor, sound);
  }
};

// TODO: Define Bird derived class
// Requirements:
// - Additional attribute: wingSpan (double)
// - Constructor calling base constructor
// - Override makeSound() and move()
// - Add unique method: fly()

class Bird : public Animal {
private:
  // TODO: Add bird-specific attributes
  double wingSpan;
  int pos;
  int sound;

public:
  // TODO: Constructor with base class initialization

  Bird(const string &n, int a, double w, double wingSp)
      : Animal(n, a, w), wingSpan(wingSp) {
    cout << "new bird " << Animal::getAnimal() << " created" << endl;
  }

  ~Bird() { cout << "Destroying bird " << Animal::getAnimal() << endl; }

  // TODO: Override virtual methods
  void makeSound() override {
    cout << Animal::getAnimal() << " says " << sound << endl;
  }

  void move(int steps) override {
    pos += steps;
    cout << Animal::getAnimal() << " moved to " << pos << endl;
  }

  // TODO: Add bird-specific methods
  void fly(int distance) {
    pos += distance;
    cout << Animal::getAnimal() << " flown to " << pos << endl;
  }
};

// TODO: Define Fish derived class
// Requirements:
// - Additional attribute: waterType (string) - "fresh" or "salt"
// - Constructor calling base constructor
// - Override makeSound() and move()
// - Add unique method: swim()

class Fish : public Animal {
private:
  // TODO: Add fish-specific attributes
  string waterType;
  int pos;

public:
  // TODO: Constructor with base class initialization
  Fish(const string &n, int a, double w, string type)
      : Animal(n, a, w), waterType(type) {
    cout << "new fish " << Animal::getAnimal() << " created" << endl;
  }

  ~Fish() { cout << "Destroying fish " << Animal::getAnimal() << endl; }
  // TODO: Override virtual methods
  void makeSound() override {
    cout << Animal::getAnimal() << " says ~~~" << endl;
  }

  void move(int steps) override {
    pos += steps;
    cout << Animal::getAnimal() << " moved to " << pos << endl;
  }
  // TODO: Add fish-specific methods
  void swim(int p) {
    pos += p;
    cout << Animal::getAnimal() << " moved to " << pos << endl;
  }
};

// ========================================
// OPTION B: GEOMETRIC SHAPES SYSTEM
// ========================================

// TODO: Define Shape base class
// Requirements:

// - Attributes: color (string), borderWidth (double)
// - Constructor that initializes attributes
// - Virtual destructor
// - Pure virtual methods: calculateArea(), calculatePerimeter()
// - Virtual method: displayInfo()
class Shape {
protected:
  // TODO: Add protected attributes
  string color;
  double borderwidth;

public:
  // TODO: Constructor
  Shape(const string &col, double &bw) : color(col), borderwidth(bw) {
    cout << "created a new shape with properties: color - " << color
         << " | border width: " << borderwidth << endl;
  }

  // TODO: Virtual destructor
  virtual ~Shape();

  // TODO: Pure virtual methods (= 0)
  virtual double calculateArea();
  virtual double calculatePerimeter();
  // TODO: Virtual methods with default implementation
  virtual void displayInfo() {
      cout << "shape color - " << color
           << " | border width: " << borderwidth << endl;
  }
  // TODO: Getter methods
};

// TODO: Define Circle derived class
// Requirements:
// - Additional attribute: radius (double)
// - Constructor calling base constructor
// - Implement calculateArea() and calculatePerimeter()
// - Override displayInfo()

class Circle : public Shape {
private:
  // TODO: Add circle-specific attributes
  double radius;

public:
  // TODO: Constructor with base class initialization
  Circle(const string &col, double &bw, double rad)
      : Shape(col, bw), radius(rad) {
    cout << "creating a new circle" << color << " with radius " << radius <<  endl;
  }
  // TODO: Implement pure virtual methods
  double calculateArea() override { return M_PI * radius * radius; };

  double calculatePerimeter() override {
      return 2 * M_PI * radius;
   };
  // TODO: Override virtual methods
 void displayInfo() override {
     cout << "Shape: Circle | color - " << color
          << " | border width: " << borderwidth << " | radius: " << radius << endl;
  }
};

// TODO: Define Rectangle derived class
// Requirements:
// - Additional attributes: width (double), height (double)
// - Constructor calling base constructor
// - Implement calculateArea() and calculatePerimeter()
// - Override displayInfo()

class Rectangle : public Shape {
private:
  // TODO: Add rectangle-specific attributes

public:
  // TODO: Constructor with base class initialization

  // TODO: Implement pure virtual methods

  // TODO: Override virtual methods
};

// TODO: Define Triangle derived class
// Requirements:
// - Additional attributes: side1, side2, side3 (double)
// - Constructor calling base constructor
// - Implement calculateArea() using Heron's formula
// - Implement calculatePerimeter()
// - Override displayInfo()

class Triangle : public Shape {
private:
  // TODO: Add triangle-specific attributes

public:
  // TODO: Constructor with base class initialization

  // TODO: Implement pure virtual methods

  // TODO: Override virtual methods
};

// ========================================
// OPTION C: EMPLOYEE MANAGEMENT SYSTEM
// ========================================
/*
cout << "\nImplementing Employee Management System..." << endl;

// TODO: Define Employee base class
// Requirements:
// - Attributes: name (string), id (int), salary (double)
// - Constructor that initializes attributes
// - Virtual destructor
// - Virtual methods: calculateBonus(), displayInfo(), getJobTitle()

class Employee {
protected:
    // TODO: Add protected attributes

private:
    // TODO: Add private attributes

public:
    // TODO: Constructor

    // TODO: Virtual destructor

    // TODO: Virtual methods

    // TODO: Getter/setter methods
};

// TODO: Define Manager derived class
// Requirements:
// - Additional attribute: teamSize (int)
// - Constructor calling base constructor
// - Override calculateBonus() - based on team size
// - Override getJobTitle() to return "Manager"
// - Add unique method: conductMeeting()

class Manager : public Employee {
private:
    // TODO: Add manager-specific attributes

public:
    // TODO: Implementation
};

// TODO: Define Developer derived class and Designer derived class
// Similar requirements to Manager but with appropriate specializations

class Developer : public Employee {
    // TODO: Implementation
};

class Designer : public Employee {
    // TODO: Implementation
};
*/

// ========================================
// OPTION D: MEDIA LIBRARY SYSTEM
// ========================================
/*
cout << "\nImplementing Media Library System..." << endl;

// TODO: Define Media base class
// Requirements:
// - Attributes: title (string), creator (string), releaseYear (int)
// - Constructor that initializes attributes
// - Virtual destructor
// - Virtual methods: play(), displayInfo(), getMediaType()

class Media {
    // TODO: Implementation
};

// TODO: Define Book, Movie, Song derived classes
// Each with appropriate specializations

class Book : public Media {
    // TODO: Add attributes like pageCount, genre
    // TODO: Implementation
};

class Movie : public Media {
    // TODO: Add attributes like duration, director
    // TODO: Implementation
};

class Song : public Media {
    // TODO: Add attributes like duration, album
    // TODO: Implementation
};
*/

// ========================================
// STEP 4: Testing Your Hierarchy
// ========================================
cout << "\n--- Step 4: Testing Your Implementation ---" << endl;

// TODO: Create instances of your classes
// TODO: Test all methods and inheritance relationships
// TODO: Demonstrate polymorphism using base class pointers
// TODO: Test constructor/destructor chaining

/*
// Example testing pattern (adapt for your chosen system):

cout << "Creating objects..." << endl;
// TODO: Create specific instances

cout << "\nTesting individual objects..." << endl;
// TODO: Call methods on each object

cout << "\nTesting polymorphism..." << endl;
// TODO: Use base class pointers/references
// vector<BaseClass*> objects = {&obj1, &obj2, &obj3};
// for (auto* obj : objects) {
//     obj->virtualMethod();
// }

cout << "\nTesting scope and destruction..." << endl;
{
    // TODO: Create objects in nested scope to test destruction order
}
*/

cout << "\n=== Implementation Complete ===" << endl;
cout << "Verify your implementation meets these criteria:" << endl;
cout << "□ Well-defined base class with common attributes" << endl;
cout << "□ Clear inheritance structure with appropriate access control" << endl;
cout << "□ Correct constructor chaining" << endl;
cout << "□ Virtual methods for polymorphism" << endl;
cout << "□ Unique attributes and methods in derived classes" << endl;
cout << "□ Functional testing demonstrates all capabilities" << endl;

return 0;
}

/*
IMPLEMENTATION GUIDE:

STEP 1: Choose Your System
□ Select ONE classification system that interests you
□ Ensure it has clear hierarchical relationships
□ Plan for at least 3-4 classes (1 base, 2-3 derived)

STEP 2: Design Your Base Class
□ Identify common attributes shared by all derived classes
□ Choose appropriate access levels (private, protected, public)
□ Include virtual destructor for proper inheritance
□ Plan virtual methods for polymorphic behavior
□ Consider pure virtual methods for abstract base classes

STEP 3: Implement Derived Classes
□ Add specialized attributes unique to each class
□ Use constructor initialization lists
□ Call base class constructor appropriately
□ Override virtual methods with meaningful implementations
□ Add unique methods specific to each derived class
□ Apply correct access specifiers

STEP 4: Test Thoroughly
□ Create instances of each class
□ Test all methods and attribute access
□ Demonstrate polymorphism with base class pointers
□ Verify constructor/destructor order
□ Test edge cases and validation

DESIGN PRINCIPLES:
- Keep base class focused on common characteristics
- Make derived classes add meaningful specialization
- Use virtual methods for behavior that varies by type
- Apply appropriate access control for real-world modeling
- Ensure proper resource management with virtual destructors

SUCCESS CRITERIA:
□ Code compiles without errors or warnings
□ Inheritance relationships work correctly
□ Polymorphism functions as expected
□ Access control prevents inappropriate usage
□ All classes have clear, logical purpose
□ Testing demonstrates full functionality
□ Code follows good C++ practices
*/
