#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class ConnectionPool {
private:
  string poolName;
  int maxConnections;
  int activeConnections;

public:
  ConnectionPool(const string &name, int maxConn)
      : poolName(name), maxConnections(maxConn), activeConnections(0) {
    cout << "Connection pool '" << poolName
         << "' created (max: " << maxConnections << ")" << endl;
  }
  ~ConnectionPool() {
    cout << "Connection pool '" << poolName << "' destroyed" << endl;
  }
  bool getConnection() {
    if (activeConnections < maxConnections) {
      activeConnections++;
      cout << "Connections acquired from " << poolName << " ("
           << activeConnections << "/" << maxConnections << " active)" << endl;
      return true;
    }
    cout << "No connections available in " << poolName << endl;
    return false;
  }

  void releaseConnection() {
    if (activeConnections > 0) {
      activeConnections--;
      cout << "Connection released to " << poolName << " (" << activeConnections
           << "/" << maxConnections << " active)" << endl;
    }
  }

  void displayStatus() const {
    cout << "Pool: " << poolName << " | Active: " << activeConnections << "/"
         << maxConnections << endl;
  }
  string getName() const { return poolName; }
};

class ServiceComponent : public enable_shared_from_this<ServiceComponent> {
private:
  string componentName;
  shared_ptr<ConnectionPool> pool;
  weak_ptr<ServiceComponent> parent;
  vector<shared_ptr<ServiceComponent>> children;

public:
  ServiceComponent(const string &name,
                   shared_ptr<ConnectionPool> connectionPool)
      : componentName(name), pool(connectionPool) {
    cout << "Service component '" << componentName << "' created" << endl;
  }
  ~ServiceComponent() {
    cout << "Service component '" << componentName << "' destroyed" << endl;
  }
  void addChild(shared_ptr<ServiceComponent> child) {
    child->parent = shared_from_this();
    children.push_back(child);
    cout << "Added child component to " << componentName << endl;
  }
  void usePool() {
    if (pool) {
      cout << componentName << " using connection pool:" << endl;
      pool->getConnection();
      pool->displayStatus();
    } else {
      cout << componentName << " has no connection pool!" << endl;
    }
  }

  void releasePoolConnection() {
    if (pool) {
      pool->releaseConnection();
    }
  }

  void displayHierarchy(int depth = 0) const {
    string indent(depth * 2, ' ');
    cout << indent << "Component: " << componentName;
    if (auto p = parent.lock()) {
      cout << " (parent: " << p->componentName << ")";
    }
    cout << " | Pool refs: " << (pool ? pool.use_count() : 0) << endl;
    for (const auto &child : children) {
      child->displayHierarchy(depth + 1);
    }
  }

  string getName() const { return componentName; }

  // Enable shared_from_this functionality
  shared_ptr<ServiceComponent> getShared() { return shared_from_this(); }

private:
  // This allows shared_from_this to work
  class EnableSharedFromThis
      : public std::enable_shared_from_this<ServiceComponent> {};
};

// Fix the shared_from_this issue by using enable_shared_from_this

class ModernServiceComponent
    : public std::enable_shared_from_this<ModernServiceComponent> {
private:
  string componentName;
  shared_ptr<ConnectionPool> pool;
  weak_ptr<ModernServiceComponent> parent;
  vector<shared_ptr<ModernServiceComponent>> children;

public:
  ModernServiceComponent(const string &name,
                         shared_ptr<ConnectionPool> connectionPool)
      : componentName(name), pool(connectionPool) {
    cout << "Modern service component '" << componentName << "' created"
         << endl;
  }
  ~ModernServiceComponent() {
    cout << "Modern service component '" << componentName << "' destroyed"
         << endl;
  }

  void addChild(shared_ptr<ModernServiceComponent> child) {
    child->parent = shared_from_this();
    children.push_back(child);
    cout << "Added child to " << componentName
         << " (parent refs: " << shared_from_this().use_count() << ")" << endl;
  }

  void usePool() {
    if (pool) {
      cout << componentName << " accessing shared connection pool:" << endl;
      pool->getConnection();
      cout << "  Pool reference count: " << pool.use_count() << endl;
    }
  }
  void releasePoolConnection() {
    if (pool) {
      pool->releaseConnection();
    }
  }
  void displayInfo() const {
    cout << "Component: " << componentName;
    if (auto p = parent.lock()) {
      cout << " | Parent: " << p->componentName;
    } else {
      cout << " | Parent: none or expired";
    }
    cout << " | Children: " << children.size();
    cout << " | Pool refs: " << (pool ? pool.use_count() : 0) << endl;
  }
  string getName() const { return componentName; }
};

int main() {
  cout << "=== Shared Resource Management System ===" << endl;
  // Create shared connection pool
  cout << "\n--- Creating Shared Connection Pool ---" << endl;
  auto sharedPool = make_shared<ConnectionPool>("MainPool", 10);

  cout << "Initial pool reference count: " << sharedPool.use_count() << endl;
  // Create service components sharing the pool
  cout << "\n--- Creating Service Components ---" << endl;
  auto authService =
      make_shared<ModernServiceComponent>("AuthService", sharedPool);
  auto userService =
      make_shared<ModernServiceComponent>("UserService", sharedPool);
  auto dataService =
      make_shared<ModernServiceComponent>("DataService", sharedPool);
  cout << "Pool reference count after creating components: "
       << sharedPool.use_count() << endl;

  // Create component hierarchy
  cout << "\n--- Building Component Hierarchy ---" << endl;
  authService->addChild(userService);
  authService->addChild(dataService);

  cout << "\n--- Using Shared Resources ---" << endl;
  authService->usePool();
  userService->usePool();
  dataService->usePool();
  sharedPool->displayStatus();

  // Demonstrate weak_ptr  functionality
  cout << "\n--- Testing weak_ptr  Behavior ---" << endl;
  weak_ptr<ConnectionPool> poolObserver = sharedPool;
  weak_ptr<ModernServiceComponent> componentObserver = userService;
  cout << "Pool observer expired? " << (poolObserver.expired() ? "Yes" : "No")
       << endl;
  cout << "Component observer expired ? "
       << (componentObserver.expired() ? "Yes" : "No") << endl;

  // Release some references
  cout << "\n--- Releasing References ---" << endl;
  cout << "Before releasing userService, pool refs: " << sharedPool.use_count()
       << endl;
  userService.reset();
  cout << "After releasing userService:" << endl;
  cout << "  Pool reference count: " << sharedPool.use_count() << endl;
  cout << "  Component observer expired? "
       << (componentObserver.expired() ? "Yes" : "No") << endl;
  // Clean up connections
  cout << "\n--- Cleaning Up Connections ---" << endl;
  authService->releasePoolConnection();
  dataService->releasePoolConnection();

  sharedPool->displayStatus();
  cout << "\n--- Program Ending ---" << endl;
  cout << "Final pool reference count: " << sharedPool.use_count() << endl;
  return 0;
}
