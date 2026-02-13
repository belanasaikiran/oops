#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class ServerConfig {
private:
  string serverName;
  int port;
  string environment;
public:
  ServerConfig(const string & name, int p, const string& env) : serverName(name), port(p), environment(env){
    cout << "Server config created: " << serverName << " on port " << endl;
  }
  ~ServerConfig(){
    cout << "Server config destroyed: " << serverName << endl;
  }
  void displayInfo() const{
    cout << "Server: " << serverName << " | Port: " << port << " | Environment: " << environment << endl;
  }
    string getName() const { return serverName; }
    int getPort() const { return port; }
};

class ServerManager{
private:
  unique_ptr<ServerConfig> config;
  string managerId;
public:
  ServerManager(const string& id) : managerId(id){
    cout <<  "ServerManager " << managerId << " created" << endl;
  }

  // Take exclusive ownership of server configuration
  void takeOwnership(unique_ptr<ServerConfig> serverConfig){
    if(config){
      cout << "Warning: " << managerId << " releasing previous configuration " << endl;
    }
    config = std::move(serverConfig);
    cout << "Manager: " << managerId << " took ownership of " << config->getName() << endl;
  }

  // Transfer ownership to another Manager
  unique_ptr<ServerConfig> transferOwnership(){
    if(!config){
      cout << "No configuration to transfer from " << managerId << endl;
      return nullptr;
    }
    cout << "Manager " << managerId << " took ownership of " << config->getName() << endl;
    return std::move(config);
  }

  void manageServer(){
    if(config){
      cout << "Manager " << managerId << " managing server: " << endl;
      config->displayInfo();
    }else{
      cout << "Manager " << managerId << " has no server to manage" << endl;
    }
  }

  ~ServerManager(){
    cout << "ServerManager " << managerId << " destroyed" << endl;
    if(config){
      cout << " (automatically cleaning up " << config->getName() << ")" << endl;
    }
  }
};

// Factory function for creating server configuration
unique_ptr<ServerConfig> createServerConfig(const string& name, int port, const string& env){
  cout << "Factory creating server configuration..." << endl;
  return make_unique<ServerConfig>(name, port, env);
}


// Factory function returning unique_ptr - Factory abstraction pattern
template <typename T, typename... Args>
unique_ptr<T> createResource(Args &&...args) {
    return make_unique<T>(std::forward<Args>(args)...);
}



int main(){
  cout << "=== Cloud Server Management System ===" << endl;
  // Create server configurations
  cout << "\n--- Creating Server Configurations ---" << endl;

  auto webConfig = createServerConfig("WebServer-01", 8080, "Production");
  auto dbConfig = createServerConfig("DatabaseServer-01", 5432, "Production");


  // Create managers
  cout << "\n--- Creating Server Managers ---" << endl;
  ServerManager webManager("WEB-MGR-001");
  ServerManager dbManager("DB-MGR-001");

  // Vector of unique_ptr for managing multiple servers
  vector<unique_ptr<ServerConfig>> serverPool;
  serverPool.push_back(createServerConfig("Server1", 8080, "Production"));

  // Assign exclusive ownership
  cout << "\n--- Assigning Exclusive Ownership ---" << endl;
  webManager.takeOwnership(std::move(webConfig));
  dbManager.takeOwnership(std::move(dbConfig));

  // verify original pointers are now null
  cout << "original webconfig pointer is " << (webConfig ? "valid" : "null" ) << " after move" << endl;
  cout << "original dbConfig pointer is " << (dbConfig ? "valid" : "null" ) << " after move" << endl;

  // Manage Servers
  cout << "\n--- Managing Servers ---" << endl;
  webManager.manageServer();
  dbManager.manageServer();

  // transfer ownership
  cout << "\n--- Transferring Ownership ---" << endl;
  auto transferredConfig = webManager.transferOwnership();
  cout << "After transfer: " << endl;
  webManager.manageServer(); // should show no server
  if(transferredConfig){
    cout << "Transferred configuration details: "<< endl;
    transferredConfig->displayInfo();
  }

  cout << "\n--- Program ending (automatic cleanup) ---" << endl;
  return 0;

}
