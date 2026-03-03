#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

// abstract base class defining core device contract
using namespace std;

class Device {
protected:
  string deviceID;
  string manufacturer;
  bool powered;

public:
  Device(const string &id, const string &mfg)
      : deviceID(id), manufacturer(mfg), powered(false) {
    cout << "Device registered: " << deviceID << " by " << manufacturer << endl;
  }

  // virtual Destructor essential for polymorphic cleanup
  virtual ~Device() { cout << "Device unregistered: " << deviceID << endl; }

  // Pure virtual functions - must be implemented by derived classes
  virtual void turnOn() = 0;
  virtual void turnOff() = 0;
  virtual void displayStatus() const = 0;
  virtual string getDeviceType() const = 0;

  // virtula functions with default implementation
  virtual void performDiagnostic() const {
    cout << "Running standard diagnostic on " << deviceID << endl;
    cout << "Power status: " << (powered ? "ON" : "OFF") << endl;
    cout << "Manufacturer: " << manufacturer << endl;
  }

  // Non virtual utility methods
  string getDeviceID() const { return deviceID; }
  string getManufacturer() const { return manufacturer; }
  bool isPowered() const { return powered; }
};

// Interface for network-enabled devices
class NetworkCapable {
public:
  virtual ~NetworkCapable() = default;
  virtual void connect(const string &network) = 0;
  virtual void disconnect() = 0;
  virtual bool isConnected() const = 0;
  virtual string getIPAddress() const = 0;
};

// Interface for devices with charging capability
class Chargable {
public:
  virtual ~Chargable() = default;
  virtual void startCharging() = 0;
  virtual void stopCharging() = 0;
  virtual int getBatteryLevel() const = 0;
  virtual bool isCharging() const = 0;
};

// interface for devices with storaga capability
class StorageCapable {
public:
  virtual ~StorageCapable() = default;
  virtual void writeData(const string &data) = 0;
  virtual string readData(const string &filename) = 0;
  virtual long getStorageCapacity() const = 0;
  virtual long getAvailableStorage() const = 0;
};

// concrete implementation: SMARTPHONE
class SmartPhone : public Device,
                   public NetworkCapable,
                   public Chargable,
                   public StorageCapable {
private:
  bool networkConnected;
  string ipAddress;
  int batteryLevel;
  bool charging;
  long storageCapacity;
  long usedStorage;

public:
  SmartPhone(const string &id, const string &mfg, long storage)
      : Device(id, mfg), networkConnected(false), ipAddress(""),
        batteryLevel(60), charging(false), storageCapacity(storage),
        usedStorage(0) {
    cout << "SmartPhone initialized with " << storage << "GB storage" << endl;
  }
  ~SmartPhone() {
    cout << "SmartPhone " << deviceID << " powering down" << endl;
  }

  // implement device Interface
  void turnOn() override {
    powered = true;
    cout << "📱 SmartPhone " << deviceID << " powered on\n";
    cout << "OS booting... Welcome screen displayed\n";
  }
  void turnOff() override {
    powered = false;
    networkConnected = false;
    cout << " 📱 SmartPhone " << deviceID << " shutting down" << endl;
  }
  void displayStatus() const override {
    cout << "\n--- SmartPhone Status ---" << endl;
    cout << "ID: " << deviceID << endl;
    cout << "Manufacturer: " << manufacturer << endl;
    cout << "Power: " << (powered ? "ON" : "OFF") << endl;
    cout << "Network: "
         << (networkConnected ? "Connected (" + ipAddress + ")"
                              : "Disconnected")
         << endl;
    cout << "Battery: " << batteryLevel << "%" << (charging ? "(Charging)" : "")
         << endl;
    cout << "Storage: " << usedStorage << "/" << storageCapacity << "GB used"
         << endl;
  }

  string getDeviceType() const override { return "Laptop"; }

  // implement network capable Interface
  void connect(const string &network) override {
    if (powered) {
      networkConnected = true;
      ipAddress = "192.168.1." + to_string(rand() % 254 + 1);
      cout << "🌎 Connected to " << network << " (IP: " << ipAddress << ")"
           << endl;
    } else {
      cout << "❌ Cannot connect - laptop is powered off" << endl;
    }
  }

  void disconnect() override {
    networkConnected = false;
    ipAddress = "";
    cout << "📵 Network disconnected" << endl;
  }
  bool isConnected() const override { return networkConnected; }
  string getIPAddress() const override { return ipAddress; }
  // Implement Chargeable interface
  void startCharging() override {
    charging = true;
    cout << "🔋 Charging started - current level: " << batteryLevel << "%"
         << endl;
  }
  void stopCharging() override {
    charging = false;
    cout << "🔌 Charging stopped - battery level: " << batteryLevel << "%"
         << endl;
  }
  int getBatteryLevel() const override { return batteryLevel; }
  bool isCharging() const override { return charging; }
  // Implement StorageCapable interface
  void writeData(const string &data) override {
    if (powered && usedStorage < storageCapacity) {
      usedStorage += data.length() / (1024 * 1024); // Simulate MB usage
      cout << "💾 Data written to storage: " << data.substr(0, 20) << "..."
           << endl;
    } else {
      cout << "❌ Cannot write - device off or storage full" << endl;
    }
  }
  string readData(const string &filename) override {
    if (powered) {
      cout << "📂 Reading file: " << filename << endl;
      return "File content from " + filename;
    }
    cout << "❌ Cannot read - device is powered off" << endl;
    return "";
  }
  long getStorageCapacity() const override { return storageCapacity; }
  long getAvailableStorage() const override {
    return storageCapacity - usedStorage;
  }
  // Smartphone-specific methods
  void makeCall(const string &number) {
    if (powered && networkConnected) {
      cout << "📞 Calling " << number << "..." << endl;
    } else {
      cout << "❌ Cannot make call - check power and network" << endl;
    }
  }
  void sendMessage(const string &message) {
    if (powered && networkConnected) {
      cout << "💬 Message sent: " << message << endl;
    } else {
      cout << "❌ Cannot send message - check power and network" << endl;
    }
  }
};

// Concrete implementation : Laptop
class Laptop : public Device,
               public NetworkCapable,
               public Chargable,
               public StorageCapable {
private:
  bool networkConnected;
  string ipAddress;
  int batteryLevel;
  bool charging;
  long storageCapacity;
  long usedStorage;
  string operatingSystem;

public:
  Laptop(const string &id, const string &mfg, long storage, const string &os)
      : Device(id, mfg), networkConnected(false), ipAddress(""),
        batteryLevel(100), charging(false), storageCapacity(storage),
        usedStorage(0), operatingSystem(os) {
    cout << "Laptop initialized: " << os << " with " << storage << "GB storage"
         << endl;
  }
  ~Laptop() { cout << "Laptop " << deviceID << " shutting down" << endl; }
  // Implement Device interface
  void turnOn() override {
    powered = true;
    cout << "💻 Laptop " << deviceID << " booting " << operatingSystem << endl;
    cout << "Boot sequence complete - desktop loaded" << endl;
  }
  void turnOff() override {
    powered = false;
    networkConnected = false;
    cout << "💻 Laptop " << deviceID << " shutting down " << operatingSystem
         << endl;
  }
  void displayStatus() const override {
    cout << "\n--- Laptop Status ---" << endl;
    cout << "ID: " << deviceID << endl;
    cout << "Manufacturer: " << manufacturer << endl;
    cout << "OS: " << operatingSystem << endl;
    cout << "Power: " << (powered ? "ON" : "OFF") << endl;
    cout << "Network: "
         << (networkConnected ? "Connected (" + ipAddress + ")"
                              : "Disconnected")
         << endl;
    cout << "Battery: " << batteryLevel << "%"
         << (charging ? " (Charging)" : "") << endl;
    cout << "Storage: " << usedStorage << "/" << storageCapacity << "GB used"
         << endl;
  }
  string getDeviceType() const override { return "Laptop"; }
  // Implement NetworkCapable interface
  void connect(const string &network) override {
    if (powered) {
      networkConnected = true;
      ipAddress = "192.168.1." + to_string(rand() % 254 + 1);
      cout << "🌐 Connected to " << network << " (IP: " << ipAddress << ")"
           << endl;
    } else {
      cout << "❌ Cannot connect - laptop is powered off" << endl;
    }
  }
  void disconnect() override {
    networkConnected = false;
    ipAddress = "";
    cout << "📡 Network disconnected" << endl;
  }
  bool isConnected() const override { return networkConnected; }
  string getIPAddress() const override { return ipAddress; }
  // Implement Chargeable interface
  void startCharging() override {
    charging = true;
    cout << "🔌 Power adapter connected - charging at " << batteryLevel << "%"
         << endl;
  }
  void stopCharging() override {
    charging = false;
    cout << "🔌 Power adapter disconnected - battery: " << batteryLevel << "%"
         << endl;
  }
  int getBatteryLevel() const override { return batteryLevel; }
  bool isCharging() const override { return charging; }
  // Implement StorageCapable interface
  void writeData(const string &data) override {
    if (powered && usedStorage < storageCapacity) {
      usedStorage += data.length() / (1024 * 1024);
      cout << "💾 File saved: " << data.substr(0, 30) << "..." << endl;
    } else {
      cout << "❌ Write failed - check power and storage space" << endl;
    }
  }
  string readData(const string &filename) override {
    if (powered) {
      cout << "📁 Opening file: " << filename << endl;
      return "Document content from " + filename;
    }
    cout << "❌ Cannot access file - laptop is powered off" << endl;
    return "";
  }
  long getStorageCapacity() const override { return storageCapacity; }
  long getAvailableStorage() const override {
    return storageCapacity - usedStorage;
  }
  // Laptop-specific methods
  void runApplication(const string &appName) {
    if (powered) {
      cout << "🖥️ Launching " << appName << " on " << operatingSystem << endl;
    } else {
      cout << "❌ Cannot run application - laptop is off" << endl;
    }
  }
  void compileCode(const string &language) {
    if (powered) {
      cout << "⚙️ Compiling " << language << " code..." << endl;
    } else {
      cout << "❌ Cannot compile - laptop is off" << endl;
    }
  }
};

class SecureDevice {
public:
  virtual ~SecureDevice() = default;
  virtual bool authenticate(const string &credentials) = 0;
  virtual void lockDevice() = 0;
  virtual void unlockDevice() = 0;
  virtual bool isLocked() const = 0;
};

// Interface for devices supporting remote management
class RemoteManageable {
public:
  virtual ~RemoteManageable() = default;
  virtual void executeRemoteCommand(const string &command) = 0;
  virtual string getRemoteStatus() const = 0;
  virtual bool enableRemoteAccess(const string &adminKey) = 0;
  virtual void disableRemoteAccess() = 0;
};

// Enterprise tablet with comprehensive interface implementation
class EnterpriseTablet : public Device,
                         public NetworkCapable,
                         public Chargable,
                         public StorageCapable,
                         public SecureDevice,
                         public RemoteManageable {
private:
  bool networkConnected;
  string ipAddress;
  int batteryLevel;
  bool charging;
  long storageCapacity;
  long usedStorage;
  bool deviceLocked;
  string secureHash;
  bool remoteAccessEnabled;
  string adminKey;

public:
  EnterpriseTablet(const string &id, const string &mfg, long storage)
      : Device(id, mfg), networkConnected(false), ipAddress(""),
        batteryLevel(100), charging(false), storageCapacity(storage),
        usedStorage(0), deviceLocked(true), secureHash(""),
        remoteAccessEnabled(false), adminKey("") {
    cout << "Enterprise Table initialized: " << storage
         << "GB, Security Enabled" << endl;
  }
  ~EnterpriseTablet() {
    cout << "Enterprise Tablet " << deviceID << " securely powered down"
         << endl;
  }
  // Device interface implementation
  void turnOn() override {
    powered = true;
    deviceLocked = true; // Always start locked for security
    cout << "🔒 Enterprise Tablet " << deviceID << " powered on (locked)"
         << endl;
  }
  void turnOff() override {
    powered = false;
    networkConnected = false;
    remoteAccessEnabled = false;
    cout << "🔒 Enterprise Tablet " << deviceID << " secured and powered off"
         << endl;
  }
  void displayStatus() const override {
    cout << "\n--- Enterprise Tablet Status ---" << endl;
    cout << "ID: " << deviceID << endl;
    cout << "Manufacturer: " << manufacturer << endl;
    cout << "Power: " << (powered ? "ON" : "OFF") << endl;
    cout << "Security: " << (deviceLocked ? "LOCKED" : "UNLOCKED") << endl;
    cout << "Network: "
         << (networkConnected ? "Connected (" + ipAddress + ")"
                              : "Disconnected")
         << endl;
    cout << "Battery: " << batteryLevel << "%"
         << (charging ? " (Charging)" : "") << endl;
    cout << "Storage: " << usedStorage << "/" << storageCapacity << "GB used"
         << endl;
    cout << "Remote Access: " << (remoteAccessEnabled ? "ENABLED" : "DISABLED")
         << endl;
  }
  string getDeviceType() const override { return "Enterprise Tablet"; }
  // NetworkCapable interface
  void connect(const string &network) override {
    if (powered && !deviceLocked) {
      networkConnected = true;
      ipAddress =
          "10.0.1." + to_string(rand() % 254 + 1); // Enterprise IP range
      cout << "🔐 Securely connected to " << network << " (IP: " << ipAddress
           << ")" << endl;
    } else {
      cout << "❌ Connection failed - device must be powered and unlocked"
           << endl;
    }
  }
  void disconnect() override {
    networkConnected = false;
    remoteAccessEnabled = false; // Disable remote access on disconnect
    ipAddress = "";
    cout << "🔐 Secure disconnect completed" << endl;
  }
  bool isConnected() const override { return networkConnected; }
  string getIPAddress() const override { return ipAddress; }
  // Chargeable interface
  void startCharging() override {
    charging = true;
    cout << "🔋 Enterprise charging initiated - " << batteryLevel << "%"
         << endl;
  }
  void stopCharging() override {
    charging = false;
    cout << "🔋 Enterprise charging stopped - " << batteryLevel << "%" << endl;
  }
  int getBatteryLevel() const override { return batteryLevel; }
  bool isCharging() const override { return charging; }
  // StorageCapable interface
  void writeData(const string &data) override {
    if (powered && !deviceLocked && usedStorage < storageCapacity) {
      usedStorage += data.length() / (1024 * 1024);
      cout << "🔐 Encrypted data written: " << data.substr(0, 20) << "..."
           << endl;
    } else {
      cout << "❌ Write denied - check power, security, and storage" << endl;
    }
  }
  string readData(const string &filename) override {
    if (powered && !deviceLocked) {
      cout << "🔐 Reading encrypted file: " << filename << endl;
      return "Decrypted content from " + filename;
    }
    cout << "❌ Read denied - device must be powered and unlocked" << endl;
    return "";
  }
  long getStorageCapacity() const override { return storageCapacity; }
  long getAvailableStorage() const override {
    return storageCapacity - usedStorage;
  }

  // SecureDevice interface
  bool authenticate(const string &credentials) override {
    // simple hash simulation for demo
    if (powered && credentials.length() >= 8) {
      secureHash = "hash_" + credentials;
      cout << "🔑 Authentication successful" << endl;
      return true;
    }
    cout << "❌ Authnetication failed" << endl;
    return false;
  }

  void lockDevice() override {
    deviceLocked = true;
    cout << "🔒 Device locked for security" << endl;
  }
  void unlockDevice() override {
    if (!secureHash.empty()) {
      deviceLocked = false;
      cout << "🔓 Device unlocked - access granted" << endl;
    } else {
      cout << "❌ Unlock failed - authentication required" << endl;
    }
  }
  bool isLocked() const override { return deviceLocked; }
  // RemoteManageable interface
  void executeRemoteCommand(const string &command) override {
    if (powered && networkConnected && remoteAccessEnabled && !deviceLocked) {
      cout << "🌐 Executing remote command: " << command << endl;
      if (command == "status") {
        displayStatus();
      } else if (command == "restart") {
        cout << "🔄 Remote restart initiated..." << endl;
      } else if (command == "lock") {
        lockDevice();
      } else {
        cout << "Unknown command: " << command << endl;
      }
    } else {
      cout << "❌ Remote command denied - check network, permissions, and "
              "security"
           << endl;
    }
  }
  string getRemoteStatus() const override {
    if (remoteAccessEnabled) {
      return "Remote access enabled - " + ipAddress;
    }
    return "Remote access disabled";
  }

  bool enableRemoteAccess(const string &key) override {
    if (powered && networkConnected && !deviceLocked && key.length() >= 12) {
      adminKey = key;
      remoteAccessEnabled = true;
      cout << "🌐 Remote access enabled with admin key" << endl;
      return true;
    }
    cout << "❌ Remote access denied - insufficient privileges or security"
         << endl;
    return false;
  }
  void disableRemoteAccess() override {
    remoteAccessEnabled = false;
    adminKey = "";
    cout << "🌐 Remote access disabled" << endl;
  }
};

// Device management system for polymorphic operations
class DeviceManager {
private:
  vector<unique_ptr<Device>> devices;

public:
  DeviceManager() { cout << "Device Management System initialized " << endl; }
  ~DeviceManager() {
    cout << "Device Management System shutting down..." << endl;
  }
  void registerDevice(unique_ptr<Device> device) {
    cout << "Registering device: " << device->getDeviceID() << endl;
    devices.push_back(std::move(device));
  }
  void powerOnAllDevices() {
    cout << "\n=== Powering On All Devices ===\n";
    for (auto &device : devices) {
      device->turnOn();
    }
  }
  void runSystemDiagnostics() {
    cout << "\n=== System Diagnostics ===" << endl;
    for (auto &device : devices) {
      cout << "\nDiagnosing " << device->getDeviceType() << " "
           << device->getDeviceID() << ":" << endl;
      device->performDiagnostic();
      device->displayStatus();
    }
  }

  void manageNetworkDevices() {
    cout << "\n=== Network Management ===\n";
    for (auto &device : devices) {
      // use dynamic cast to safely check for network capability
      if (auto *networkDevice = dynamic_cast<NetworkCapable *>(device.get())) {
        cout << "\nManaging network for " << device->getDeviceID() << ":"
             << endl;
        networkDevice->connect("Corporate_WIFI");
        cout << "Connection status: "
             << (networkDevice->isConnected() ? "Connected" : "Failed") << endl;
      }
    }
  }

  void manageChargeableDevices() {
    cout << "\n=== Battery Management ===" << endl;
    for (auto &device : devices) {
      if (auto *chargeableDevice = dynamic_cast<Chargable *>(device.get())) {
        cout << "\nCharging " << device->getDeviceID() << ":" << endl;
        chargeableDevice->startCharging();
        cout << "Battery level: " << chargeableDevice->getBatteryLevel() << "%"
             << endl;
      }
    }
  }

  void performSecurityAudit() {
    cout << "\n=== Security Audit ===" << endl;
    for (auto &device : devices) {
      if (auto *secureDevice = dynamic_cast<SecureDevice *>(device.get())) {
        cout << "\nAuditing " << device->getDeviceID() << ":" << endl;
        cout << "Lock Status: "
             << (secureDevice->isLocked() ? "SECURED" : "UNSECURED") << endl;
        if (!secureDevice->isLocked()) {
          cout << "⚠️ WARNING: Device is unlocked!" << endl;
          secureDevice->lockDevice();
        }
      }
    }
  }
  size_t getDeviceCount() const { return devices.size(); }
};

// Base interface that might be inherited by multiple paths

class Identifiable {
protected:
  string uniqueID;

public:
  Identifiable(const string &id) : uniqueID(id) {
    cout << "Identifiable created: " << uniqueID << endl;
  }
  virtual ~Identifiable() {
    cout << "Identifiable destroyed: " << uniqueID << endl;
  }
  virtual string getID() const { return uniqueID; }
  virtual void displayIdentity() const { cout << "ID: " << uniqueID << endl; }
};

// Multiple inheritance with virtual inheritanbce to avoid diamond problem
class NetworkIdentifiable : public virtual Identifiable {
protected:
  string networkAddress;

public:
  NetworkIdentifiable(const string &id, const string &addr)
      : Identifiable(id), networkAddress(addr) {
    cout << "Network identity established: " << networkAddress << endl;
  }
  virtual ~NetworkIdentifiable() {
    cout << "Network identity cleared: " << networkAddress << endl;
  }
  virtual string getNetworkAddress() const { return networkAddress; }
  void displayIdentity() const override {
    Identifiable::displayIdentity();
    cout << "Network Address: " << networkAddress << endl;
  }
};

class SecureIdentifiable : public virtual Identifiable {
protected:
  string securityToken;

public:
  SecureIdentifiable(const string &id, const string &token)
      : Identifiable(id), securityToken(token) {
    cout << "Security identity established: " << token.substr(0, 8) << "..."
         << endl;
  }
  virtual ~SecureIdentifiable() { cout << "Security identity cleared" << endl; }
  virtual string getSecurityToken() const { return securityToken; }
  virtual bool validateSecurity() const { return !securityToken.empty(); }
  void displayIdentity() const override {
    Identifiable::displayIdentity();
    cout << "Security Token: " << securityToken.substr(0, 8) << "..." << endl;
  }
};

// Diamond inheritance resolve with virtual inheritance
class SecureNetworkDevice : public NetworkIdentifiable,
                            public SecureIdentifiable {
private:
  string deviceName;
  bool secureConnectionActive;

public:
  // Must explicitly call virtual base ctr
  SecureNetworkDevice(const string &id, const string &name, const string &addr,
                      const string &token)
      : Identifiable(id), // virtual base ctr
        NetworkIdentifiable(id, addr), SecureIdentifiable(id, token),
        deviceName(name), secureConnectionActive(false) {
    cout << "Secure Network Device created: " << deviceName << endl;
  }
  ~SecureNetworkDevice() {
    cout << "Secure Network Device destroyed: " << deviceName << endl;
  }
  void displayIdentity() const override {
      cout << "\n--- Secure Network Device Identity ---" << endl;
      cout << "Device Name: " << deviceName << endl;
      Identifiable::displayIdentity();
      cout << "Network Address: " << getNetworkAddress() << endl;
      cout << "Security Status: " << (validateSecurity() ? "VALID" : "INVALID") << endl;
      cout << "Secure Connection: " << (secureConnectionActive ? "ACTIVE" : "INACTIVE") << endl;
  }

  void establishSecureConnection() {
    if (validateSecurity()) {
        secureConnectionActive = true;
        cout << "🔐 Secure connection established for " << deviceName << endl;
        cout << "Network: " << getNetworkAddress() << endl;
        cout << "Security validated ✓" << endl;
        } else {
        cout << "❌ Secure connection failed - invalid security token" << endl;
        }
  }
  void terminateConnection() {
    secureConnectionActive = false;
    cout << "🔐 Secure connection terminated for " << deviceName << endl;
  }

  bool isSecurelyConnected() const {
    return secureConnectionActive && validateSecurity();
  }
  string getDeviceName() const {return deviceName; }
};


// Demonstrate function for virtual inheritance concepts
void demonstrateVirtualInheritance() {
    cout << "\n🔹 Virtual Inheritance Demonstration 🔹" << endl;
    cout << "Resolving diamond problem with virtual inheritance" << endl;
    // Create secure network device
    SecureNetworkDevice device("DEV001", "Enterprise Router", "192.169.1.1",
                               "SecureToken123456");
    cout << "\n--- Testing polymorphic access ---" << endl;
    // Access through different base class pointers
    Identifiable *identPtr = &device;
    NetworkIdentifiable* networkPtr = &device;
    SecureIdentifiable *securePtr = &device;
    cout << "\nAccessing through Identifiable pointer:" << endl;
    cout << "ID: " << identPtr->getID() << endl;
    cout << "\nAccessing through NetworkIdentifiable pointer:" << endl;
    cout << "Network Address: " << networkPtr->getNetworkAddress() << endl;
    cout << "\nAccessing through SecureIdentifiable pointer:" << endl;
    cout << "Security Valid: " << (securePtr->validateSecurity() ? "Yes" : "No") << endl;
    cout << "\n--- Testing virtual function dispatch ---" << endl;
    identPtr->displayIdentity();
    cout << "\n--- Testing secure connection ---" << endl;
    device.establishSecureConnection();
    cout << "Connection Status: " << (device.isSecurelyConnected() ? "Secure" : "Insecure") << endl;
    device.terminateConnection();
}
