# include <iostream>
#include <string>

class DatabaseConnection{
private:
  std::string connectionString;
  std::string databaseName;
  bool isConnected;
  int connectionId;
  static int nextId;

  // Simulate connection Establishment
  bool establishConnection(){
    std::cout << "Establishing connection to " << databaseName << "..." << std::endl;
    isConnected = true;
    return true;
  }


  // simulate connection cleanup
  void closeConnection(){
    if(isConnected){
      std::cout << "Closing database connection [ID: " << connectionId << "]" << std::endl;
      isConnected = false;
    }
  }

public:

  // Default ctr
  DatabaseConnection() : connectionString("localhost:5432"), databaseName("defaultDB"), isConnected(false), connectionId(++nextId){
    std::cout << "creating default database connection [ID: " << connectionId << "]\n";
    isConnected = false;
  }

  // Parameterized Ctr
  DatabaseConnection(const std::string& connStr, const std::string& dbName) : connectionString(connStr), databaseName(dbName), isConnected(false), connectionId(++nextId){
    std::cout << "Creating database connection [ID: " << connectionId << "] to " << databaseName << std::endl;
    establishConnection();
  }

  // Copy Ctr
  DatabaseConnection(const DatabaseConnection& other) : connectionString(other.connectionString), databaseName(other.databaseName), connectionId(++nextId), isConnected(false) {
    std::cout << "Creating copied database connection [ID: " << connectionId
              << "] based on connection " << other.connectionId << std::endl;
    establishConnection();
  }



  // Destructor
  ~DatabaseConnection(){
    std::cout << "Destroying database connection [ID: " << connectionId << "]" << std::endl;
    closeConnection();
  }



  // Member Functions
   void executeQuery(const std::string& query){
    if(isConnected){
      std::cout << "Executing on " << databaseName << ": " << query << std::endl;
    }else{
    std::cout << "Cannot execute - connection not established" << std::endl;
    }
  }

  bool getConnectionStatus(){
    return isConnected;
  }
  int getId() const{
    return connectionId;
  }
};


// INitialize the static member
int DatabaseConnection::nextId = 0;

int main(){
  std::cout << "=== Database connection Manager ===" << std::endl;


  std::cout << "database connection init" << std::endl;


  {
  std::cout << "Default ctr\n";
  DatabaseConnection d1;
  }


  std::cout << "Parameterized Ctr calling\n";
  DatabaseConnection d2("mysql:15431", "school");
  std::cout << "Copy Ctr calling...";
  DatabaseConnection d3 = d2;


  std::cout << "Check D2 status and close it\n";
  d2.getConnectionStatus();
  d2.getId();


  std::cout << "Execute a query on D3\n";
  d3.getId();
  d3.getConnectionStatus();
  d3.executeQuery("SELECT * from tableX");


  std::cout << "Closing all active connections\n";



  return 0;
}
