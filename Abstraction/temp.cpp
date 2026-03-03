#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class base {
public:
  virtual void save(const std::string& data) const = 0;
};

class FileStorage : public virtual base {
public:
  void save(const std::string &data) const override {
      cout << "saving file a with data: " << data << endl;
    }
};

class DatabaseStorage : public virtual base {
public:
  void save(const std::string &data) const override {
      cout << "saving database with data: " << data << endl;
    }
};

class StorageCollection{
private:
  vector<unique_ptr<base>> collection;

public:
  StorageCollection() { cout << "storage collectiuon created\n"; }
  ~StorageCollection() { cout << "Storage collection destroyed\n"; }

  void addToCollection(unique_ptr<base> item) {
      collection.push_back(std::move(item));
  }

  void save(const std::string &data)  {
    cout << "saving file to file and database here\n";
    // WE can check the pointer type and save it
    for (auto& col : collection) {
        col->save(data);
    }
  }
};

class Logger : public FileStorage, public DatabaseStorage {
public:
  Logger() {
    cout << "Logger created!\n";
  }
  ~Logger() { cout <<  "Logger destroyed\n";}

  void save(const std::string& data) const override {
    cout << "LOG: Saving with data: " << data << endl;
    FileStorage::save(data);
    DatabaseStorage::save(data);
  }
};


// Pure virtual functions make the class an abstract class which makes sure the
// derived class must implement the functions from base virtual abstract class
// It helps in segregating the interface components.


// Coarse grained interfaces have many methods in a single interface which may
// force uncessary implementations in derived classes.
//
// Fine grained interfaces are the small, focused interfdaces which is good
// because, each interface does only one thing.
