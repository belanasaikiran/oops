#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

// ========================================
// LEGACY CODE - XYZ Software Solutions
// WARNING: This code has memory management issues!
// Your task: Modernize this code using smart pointers
// ========================================

// Legacy Resource Classes
class DatabaseConnection {
private:
    char* connectionString;
    bool connected;
    int connectionId;
    
public:
    DatabaseConnection(const char* connStr, int id) : connected(false), connectionId(id) {
        connectionString = new char[strlen(connStr) + 1];
        strcpy(connectionString, connStr);
        connected = true;
        cout << "Database connection " << connectionId << " established: " << connectionString << endl;
    }
    
    // PROBLEM: No destructor! Memory leak guaranteed!
    
    void query(const char* sql) {
        if (connected) {
            cout << "Executing query on connection " << connectionId << ": " << sql << endl;
        }
    }
    
    void disconnect() {
        if (connected) {
            connected = false;
            cout << "Database connection " << connectionId << " closed" << endl;
            // PROBLEM: connectionString never freed!
        }
    }
    
    bool isConnected() const { return connected; }
    int getId() const { return connectionId; }
};

class FileHandler {
private:
    FILE* file;
    char* filename;
    
public:
    FileHandler(const char* fname, const char* mode) : file(nullptr) {
        filename = new char[strlen(fname) + 1];
        strcpy(filename, fname);
        file = fopen(fname, mode);
        if (file) {
            cout << "File opened: " << filename << endl;
        } else {
            cout << "Failed to open file: " << filename << endl;
        }
    }
    
    // PROBLEM: No destructor! File handles and memory leaked!
    
    bool write(const char* data) {
        if (file) {
            fprintf(file, "%s", data);
            return true;
        }
        return false;
    }
    
    void close() {
        if (file) {
            fclose(file);
            file = nullptr;
            cout << "File closed: " << filename << endl;
        }
        // PROBLEM: filename never freed!
    }
};

class Logger {
private:
    FileHandler* logFile;  // Raw pointer - ownership unclear!
    
public:
    Logger(const char* logFilename) {
        logFile = new FileHandler(logFilename, "a");
    }
    
    // PROBLEM: No destructor! FileHandler leaked!
    
    void log(const char* message) {
        if (logFile) {
            logFile->write(message);
            logFile->write("\n");
        }
    }
    
    void closeLog() {
        if (logFile) {
            logFile->close();
            delete logFile;  // Manual delete - error prone!
            logFile = nullptr;
        }
    }
};

// Legacy Application Class with Multiple Issues
class LegacyApplication {
private:
    DatabaseConnection* primaryDb;    // Raw pointer
    DatabaseConnection* backupDb;     // Raw pointer
    vector<DatabaseConnection*> connectionPool;  // Vector of raw pointers!
    Logger* appLogger;                // Raw pointer
    char* applicationName;            // Raw C-string
    
public:
    LegacyApplication(const char* name) : primaryDb(nullptr), backupDb(nullptr), appLogger(nullptr) {
        // Manual string copying - error prone!
        applicationName = new char[strlen(name) + 1];
        strcpy(applicationName, name);
        
        cout << "Starting legacy application: " << applicationName << endl;
        
        // Create connections manually
        primaryDb = new DatabaseConnection("primary://localhost:5432", 1);
        backupDb = new DatabaseConnection("backup://localhost:5433", 2);
        
        // Create logger
        appLogger = new Logger("application.log");
        appLogger->log("Application started");
        
        // Populate connection pool
        for (int i = 0; i < 5; i++) {
            char connStr[100];
            sprintf(connStr, "pool://localhost:%d", 6000 + i);
            connectionPool.push_back(new DatabaseConnection(connStr, 100 + i));
        }
    }
    
    // PROBLEM: Destructor manually manages everything - error prone!
    ~LegacyApplication() {
        cout << "Shutting down application: " << applicationName << endl;
        
        // Manual cleanup - what could go wrong?
        if (primaryDb) {
            primaryDb->disconnect();
            delete primaryDb;
        }
        
        if (backupDb) {
            backupDb->disconnect();
            delete backupDb;
        }
        
        // Clean up connection pool manually
        for (size_t i = 0; i < connectionPool.size(); i++) {
            connectionPool[i]->disconnect();
            delete connectionPool[i];
        }
        connectionPool.clear();
        
        if (appLogger) {
            appLogger->log("Application shutting down");
            appLogger->closeLog();
            delete appLogger;
        }
        
        delete[] applicationName;
        cout << "Legacy application cleanup complete" << endl;
    }
    
    void performDatabaseWork() {
        appLogger->log("Starting database operations");
        
        if (primaryDb && primaryDb->isConnected()) {
            primaryDb->query("SELECT * FROM users");
            primaryDb->query("UPDATE user_stats SET last_login = NOW()");
        }
        
        // Use connection from pool
        if (!connectionPool.empty()) {
            connectionPool[0]->query("SELECT COUNT(*) FROM transactions");
        }
        
        appLogger->log("Database operations completed");
    }
    
    void simulateWork() {
        performDatabaseWork();
        
        // Simulate some processing
        for (int i = 0; i < 3; i++) {
            char logMsg[100];
            sprintf(logMsg, "Processing batch %d", i + 1);
            appLogger->log(logMsg);
        }
    }
};

// Legacy Factory Functions - Return raw pointers!
DatabaseConnection* createDatabaseConnection(const char* server, int id) {
    char connStr[200];
    sprintf(connStr, "factory://%s:3306", server);
    return new DatabaseConnection(connStr, id);  // Raw pointer returned!
}

Logger* createLogger(const char* filename) {
    return new Logger(filename);  // Raw pointer returned!
}

// Circular Reference Problem (Advanced Issue)
class Node {
public:
    int data;
    Node* next;      // Raw pointer - potential circular reference!
    Node* parent;    // Raw pointer - potential circular reference!
    
    Node(int value) : data(value), next(nullptr), parent(nullptr) {
        cout << "Node " << data << " created" << endl;
    }
    
    ~Node() {
        cout << "Node " << data << " destroyed" << endl;
        // PROBLEM: Manual deletion could cause double-delete or stack overflow!
        delete next;  // This could cause deep recursion or crashes!
    }
};

Node* createCircularList(int size) {
    if (size <= 0) return nullptr;
    
    Node* first = new Node(1);
    Node* current = first;
    
    // Create chain
    for (int i = 2; i <= size; i++) {
        current->next = new Node(i);
        current->next->parent = current;
        current = current->next;
    }
    
    // Make it circular - PROBLEM: This creates a leak!
    current->next = first;
    first->parent = current;
    
    return first;  // Who owns this? When to delete?
}

// ========================================
// MAIN FUNCTION - Legacy Demo
// ========================================

int main() {
    cout << "=== LEGACY CODE DEMONSTRATION ===" << endl;
    cout << "WARNING: This code has memory management issues!" << endl;
    cout << "Your task: Modernize using smart pointers" << endl;
    
    // Test the legacy application
    {
        cout << "\n--- Testing Legacy Application ---" << endl;
        LegacyApplication* app = new LegacyApplication("XYZ Legacy System");
        
        app->simulateWork();
        
        // Manual cleanup required!
        delete app;
    }
    
    // Test factory functions
    {
        cout << "\n--- Testing Legacy Factory Functions ---" << endl;
        DatabaseConnection* conn = createDatabaseConnection("test-server", 999);
        Logger* logger = createLogger("factory-test.log");
        
        conn->query("SELECT 1");
        logger->log("Factory test completed");
        
        // Manual cleanup required!
        conn->disconnect();
        delete conn;
        logger->closeLog();
        delete logger;
    }
    
    // Demonstrate circular reference problem
    {
        cout << "\n--- Testing Circular Reference Problem ---" << endl;
        cout << "Creating circular list (WARNING: This will leak memory!)" << endl;
        
        Node* list = createCircularList(3);
        
        // PROBLEM: How do we safely delete a circular structure?
        // Deleting 'list' would cause infinite recursion or double-delete!
        // For demo purposes, we'll just abandon it (memory leak)
        
        cout << "Circular list created but not cleaned up - MEMORY LEAK!" << endl;
    }
    
    cout << "\n=== LEGACY DEMO COMPLETE ===" << endl;
    cout << "Issues identified:" << endl;
    cout << "1. Missing destructors causing memory leaks" << endl;
    cout << "2. Raw pointers with unclear ownership" << endl;
    cout << "3. Manual memory management - error prone" << endl;
    cout << "4. No RAII - resources not automatically cleaned up" << endl;
    cout << "5. Potential circular references causing leaks" << endl;
    cout << "6. Factory functions returning raw pointers" << endl;
    cout << "\nYour task: Fix these issues using modern C++ smart pointers!" << endl;
    
    return 0;
}

/*
MODERNIZATION TASKS:

STEP 1: ANALYZE THE LEGACY CODE
□ Identify all raw pointer usage
□ Find missing destructors
□ Locate potential memory leaks
□ Identify unclear ownership semantics
□ Find circular reference problems

STEP 2: REFACTOR WITH SMART POINTERS
□ Replace raw pointers with unique_ptr for exclusive ownership
□ Use shared_ptr for shared ownership scenarios
□ Apply weak_ptr to break circular references
□ Update factory functions to return smart pointers
□ Ensure proper RAII implementation

STEP 3: IMPLEMENT CUSTOM DELETERS
□ Create custom deleters for DatabaseConnection cleanup
□ Implement file handle cleanup with custom deleters
□ Add logging to deleters for debugging
□ Handle special cleanup requirements

MEMORY ISSUES TO FIX:
⚠ DatabaseConnection: Missing destructor, connectionString leak
⚠ FileHandler: Missing destructor, filename and FILE* leak
⚠ Logger: Missing destructor, FileHandler leak
⚠ LegacyApplication: Complex manual cleanup, error-prone
⚠ Factory functions: Return raw pointers, unclear ownership
⚠ Circular references: Node structure causes leaks
⚠ Vector of raw pointers: Manual cleanup required

SUCCESS CRITERIA:
✓ All raw pointers replaced with appropriate smart pointers
✓ Automatic resource cleanup (RAII)
✓ Clear ownership semantics
✓ No memory leaks
✓ Proper handling of circular references
✓ Exception-safe code
✓ Simplified and maintainable code structure
*/
