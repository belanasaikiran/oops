#include <iostream>
#include <memory>
#include <vector>

using namespace std;
class Shape {
private:
public:
  virtual void draw() const {}
};

class Circle : public Shape {
private:
  int radius;
public:
  Circle(int r) : radius(r) {
    cout << "new Circle created with radius "<< radius << endl;
    }
  void draw() const override {
    cout << "Drawing a circle with radius " << radius << endl;
  }

};

class Rectangle : public Shape {
private:
  int length;
  int breadth;

public:
  Rectangle(int l, int b) : length(l), breadth(b) {
      cout << "new Rectangle created with length, breadth: "<< length << "," << breadth << endl;

}

  void draw() const override {
    cout << "Drawing a Rectangle with length, breadth: "<< length << "," << breadth << endl;
  }
};

int main(){
  cout << "===RTTI Demonstration===" << endl;

  vector<unique_ptr<Shape>> shapes;
  shapes.push_back(make_unique<Circle>(5));
  shapes.push_back(make_unique<Rectangle>(10, 20));

  for (const auto &shape : shapes) {
    Shape* s = shape.get();
    cout << "We are gonna draw the shape: " << typeid(*s).name() << endl;
    shape->draw();
  }
}
