#include <iostream>
#include <string>

class Book{
private:
    std::string title;
    std::string author;
    int pages;
    bool isAvailable;
public:
    Book(std::string bookTitle, std::string bookAuthor, int pageCount) : title(bookTitle),  author(bookAuthor),  isAvailable(true) {
        if(pageCount > 0 ) pages = pageCount;
        else pages = 0;
    }

    // Getter Methods
    std::string getTitle() const {return title;}
    std::string getAuthor() const {return author;}

    int getPages() const {return pages;}
    bool getAvailability() const {return isAvailable;}

    // Setter methods
    void SetPages(int newPages){
        if(newPages > 0) pages = newPages;
    }

    void setAvailability(bool status){ isAvailable = status;}

    void displayInfo() const {
        std::cout << "Title: " << title << std::endl;
        std::cout << "Author: " << author << std::endl;
        std::cout << "Pages: " << pages << std::endl;
        std::cout << "Available: " << (isAvailable ? "Yes" : "No") << std::endl;
        std::cout << "------------------------------------------" << std::endl;
    }
};


int main(){
    Book b1("The C++ Programming Language", "Bjarne Stroustrup", 1376);
    Book b2("Clean Code", "Robert C. Martin", 464);

    b1.displayInfo();
    b2.displayInfo();

    std::cout << "Checking out: " << b1.getTitle() << std::endl;
    b1.setAvailability(false);
    b1.displayInfo();
    return 0;
}
