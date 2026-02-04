#include <iostream>
#include <string>
#include <vector>

class Student {
private:
  std::string name;
  int studentId;
  std::vector<double> grades;

public:
  Student(std::string stName, int id) : name(stName), studentId(id) {
    std::cout << "Student " << name << "enrolled with ID: " << id << std::endl;
  }

  // Add a grade
  void addGrade(double grade) {
    if (grade >= 0.0 && grade <= 100.0) {
      grades.push_back(grade);
      std::cout << "Grade " << grade << " added for " << name
                            << std::endl;
    }
    else {
      std::cout << "Invalid grade. Must be between 0 and 100."
                            << std::endl;

    }

  }

 // Calculate average grade
double calculateAverage() const {
    if (grades.empty()) return 0.0;

double sum = 0.0;
    for (double grade : grades) {
      sum += grade;

    }
    return sum / grades.size();

  }

 // Display student information
void displayStudent() const {
    std::cout << "Student: " << name << " (ID: " << studentId << ")"
                      << std::endl;
    std::cout << "Number of grades: " << grades.size() << std::endl;
    std::cout << "Average grade: " << calculateAverage() << std::endl;

  }
};
// Complete this implementation
int main() {
   // Your code here: Create student objects and demonstrate functionality
  
  Student s1("Sanju" , 1);
  Student s2("Siva", 2);
  s1.addGrade(70.1);
  s1.addGrade(75.2);
  s2.addGrade(70.0);
  s2.addGrade(74.6);

  s1.displayStudent();
  s2.displayStudent();


return 0;
}
