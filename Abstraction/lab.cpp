#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <fstream>
using namespace std;

// ========================================
// SCENARIO 1: DATA PERSISTENCE LAYER - SOLUTION
// ========================================

class IDataReader {
public:
    virtual ~IDataReader() = default;

    virtual string read() = 0;
    virtual bool hasNext() = 0;
    virtual void close() = 0;
    virtual string getDataSource() const = 0;
    virtual void reset() = 0;  // Reset to beginning
};

class IDataWriter {
public:
    virtual ~IDataWriter() = default;

    virtual void write(const string& data) = 0;
    virtual void flush() = 0;
    virtual void close() = 0;
    virtual string getDataDestination() const = 0;
    virtual size_t getBytesWritten() const = 0;
};

class SQLDataReader : public IDataReader {
private:
    string connectionString;
    vector<string> resultSet;
    size_t currentIndex;
    bool isOpen;

public:
    SQLDataReader(const string& connStr, const vector<string>& mockData = {})
        : connectionString(connStr), resultSet(mockData), currentIndex(0), isOpen(true) {
        if (resultSet.empty()) {
            // Mock SQL data
            resultSet = {
                "ID:1|Name:John|Age:25",
                "ID:2|Name:Jane|Age:30",
                "ID:3|Name:Bob|Age:35"
            };
        }
        cout << "SQLDataReader connected to: " << connectionString << endl;
    }

    ~SQLDataReader() {
        if (isOpen) close();
    }

    string read() override {
        if (!isOpen) {
            throw runtime_error("Reader is closed");
        }
        if (hasNext()) {
            return resultSet[currentIndex++];
        }
        return "";
    }

    bool hasNext() override {
        return isOpen && currentIndex < resultSet.size();
    }

    void close() override {
        if (isOpen) {
            isOpen = false;
            cout << "SQLDataReader connection closed" << endl;
        }
    }

    string getDataSource() const override {
        return "SQL Database: " + connectionString;
    }

    void reset() override {
        currentIndex = 0;
    }

    // SQL-specific methods
    void executeQuery(const string& query) {
        cout << "Executing SQL query: " << query << endl;
    }

    size_t getRecordCount() const {
        return resultSet.size();
    }
};

class FileDataWriter : public IDataWriter {
private:
    string filePath;
    stringstream buffer;
    bool isOpen;
    size_t bytesWritten;

public:
    FileDataWriter(const string& path)
        : filePath(path), isOpen(true), bytesWritten(0) {
        cout << "FileDataWriter opened: " << filePath << endl;
    }

    ~FileDataWriter() {
        if (isOpen) close();
    }

    void write(const string& data) override {
        if (!isOpen) {
            throw runtime_error("Writer is closed");
        }
        buffer << data << endl;
        bytesWritten += data.length() + 1; // +1 for newline
        cout << "Written to buffer: " << data.substr(0, 50)
             << (data.length() > 50 ? "..." : "") << endl;
    }

    void flush() override {
        if (!isOpen) {
            throw runtime_error("Writer is closed");
        }
        cout << "Flushing " << bytesWritten << " bytes to " << filePath << endl;
        // In real implementation, write buffer to actual file
        cout << "Buffer contents:" << endl;
        cout << buffer.str() << endl;
    }

    void close() override {
        if (isOpen) {
            flush();
            isOpen = false;
            cout << "FileDataWriter closed: " << filePath << endl;
        }
    }

    string getDataDestination() const override {
        return "File: " + filePath;
    }

    size_t getBytesWritten() const override {
        return bytesWritten;
    }
};

class JSONDataReader : public IDataReader {
private:
    string jsonFile;
    vector<string> jsonRecords;
    size_t currentIndex;
    bool isOpen;

public:
    JSONDataReader(const string& file)
        : jsonFile(file), currentIndex(0), isOpen(true) {
        // Mock JSON data
        jsonRecords = {
            R"({"id": 1, "name": "Alice", "role": "Manager"})",
            R"({"id": 2, "name": "Charlie", "role": "Developer"})",
            R"({"id": 3, "name": "Diana", "role": "Tester"})"
        };
        cout << "JSONDataReader opened: " << jsonFile << endl;
    }

    string read() override {
        if (hasNext()) {
            return jsonRecords[currentIndex++];
        }
        return "";
    }

    bool hasNext() override {
        return isOpen && currentIndex < jsonRecords.size();
    }

    void close() override {
        if (isOpen) {
            isOpen = false;
            cout << "JSONDataReader closed" << endl;
        }
    }

    string getDataSource() const override {
        return "JSON File: " + jsonFile;
    }

    void reset() override {
        currentIndex = 0;
    }
};

// ========================================
// SCENARIO 2: COMMUNICATION PROTOCOL - SOLUTION
// ========================================

class IMessageReceiver {
public:
    virtual ~IMessageReceiver() = default;

    virtual string receive() = 0;
    virtual void listen(int port) = 0;
    virtual void stop() = 0;
    virtual bool isListening() const = 0;
    virtual int getPort() const = 0;
};

class IMessageSender {
public:
    virtual ~IMessageSender() = default;

    virtual void send(const string& message, const string& destination) = 0;
    virtual void connect(const string& address) = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    virtual string getConnectionInfo() const = 0;
};

class TCPMessageHandler : public IMessageReceiver, public IMessageSender {
private:
    string serverAddress;
    int port;
    bool connected;
    bool listening;
    queue<string> messageQueue;

public:
    TCPMessageHandler(const string& address, int portNum)
        : serverAddress(address), port(portNum), connected(false), listening(false) {
        cout << "TCPMessageHandler created for " << address << ":" << port << endl;
    }

    ~TCPMessageHandler() {
        disconnect();
        stop();
    }

    // IMessageReceiver implementation
    string receive() override {
        if (!listening) {
            throw runtime_error("Not listening for messages");
        }

        if (messageQueue.empty()) {
            // Simulate receiving a message
            static int msgCount = 0;
            messageQueue.push("TCP_MSG_" + to_string(++msgCount) + "_FROM_" + serverAddress);
        }

        string message = messageQueue.front();
        messageQueue.pop();
        cout << "TCP received: " << message << endl;
        return message;
    }

    void listen(int listenPort) override {
        port = listenPort;
        listening = true;
        cout << "TCP listening on port " << port << endl;
    }

    void stop() override {
        if (listening) {
            listening = false;
            cout << "TCP stopped listening on port " << port << endl;
        }
    }

    bool isListening() const override {
        return listening;
    }

    int getPort() const override {
        return port;
    }

    // IMessageSender implementation
    void send(const string& message, const string& destination) override {
        if (!connected) {
            throw runtime_error("Not connected to server");
        }
        cout << "TCP sending to " << destination << ": " << message << endl;
    }

    void connect(const string& address) override {
        serverAddress = address;
        connected = true;
        cout << "TCP connected to " << serverAddress << endl;
    }

    void disconnect() override {
        if (connected) {
            connected = false;
            cout << "TCP disconnected from " << serverAddress << endl;
        }
    }

    bool isConnected() const override {
        return connected;
    }

    string getConnectionInfo() const override {
        return "TCP://" + serverAddress + ":" + to_string(port);
    }
};

class UDPMessageSender : public IMessageSender {
private:
    string targetAddress;
    bool connected;

public:
    UDPMessageSender() : connected(false) {
        cout << "UDPMessageSender created" << endl;
    }

    void send(const string& message, const string& destination) override {
        cout << "UDP sending to " << destination << ": " << message << endl;
        // UDP is connectionless, so we can send without being "connected"
    }

    void connect(const string& address) override {
        targetAddress = address;
        connected = true;
        cout << "UDP target set to " << targetAddress << endl;
    }

    void disconnect() override {
        if (connected) {
            connected = false;
            cout << "UDP target cleared" << endl;
        }
    }

    bool isConnected() const override {
        return connected;
    }

    string getConnectionInfo() const override {
        return connected ? "UDP://" + targetAddress : "UDP://not_connected";
    }
};

class WebSocketReceiver : public IMessageReceiver {
private:
    int wsPort;
    bool listening;
    vector<string> mockMessages;
    size_t messageIndex;

public:
    WebSocketReceiver() : wsPort(0), listening(false), messageIndex(0) {
        mockMessages = {
            "WS_HELLO_CLIENT",
            "WS_DATA_UPDATE",
            "WS_HEARTBEAT_PING"
        };
        cout << "WebSocketReceiver created" << endl;
    }

    string receive() override {
        if (!listening) {
            throw runtime_error("WebSocket not listening");
        }

        if (messageIndex >= mockMessages.size()) {
            messageIndex = 0; // Cycle through messages
        }

        string message = mockMessages[messageIndex++];
        cout << "WebSocket received: " << message << endl;
        return message;
    }

    void listen(int port) override {
        wsPort = port;
        listening = true;
        cout << "WebSocket listening on port " << wsPort << endl;
    }

    void stop() override {
        if (listening) {
            listening = false;
            cout << "WebSocket stopped listening" << endl;
        }
    }

    bool isListening() const override {
        return listening;
    }

    int getPort() const override {
        return wsPort;
    }
};

// ========================================
// SCENARIO 3: PROCESSING PIPELINE - SOLUTION
// ========================================

class IProcessorStage {
public:
    virtual ~IProcessorStage() = default;

    virtual string process(const string& input) = 0;
    virtual bool canProcess(const string& input) const = 0;
    virtual string getStageType() const = 0;
    virtual int getPriority() const = 0;
};

class IDataConverter {
public:
    virtual ~IDataConverter() = default;

    virtual string convert(const string& input) = 0;
    virtual string getSourceFormat() const = 0;
    virtual string getDestinationFormat() const = 0;
    virtual bool canConvert(const string& fromFormat, const string& toFormat) const = 0;
};

class NormalizationStage : public IProcessorStage {
private:
    int priority;

public:
    NormalizationStage(int prio = 1) : priority(prio) {
        cout << "NormalizationStage created with priority " << priority << endl;
    }

    string process(const string& input) override {
        if (!canProcess(input)) {
            throw runtime_error("Cannot process this input");
        }

        // Normalize: lowercase, trim, remove extra spaces
        string result = input;
        transform(result.begin(), result.end(), result.begin(), ::tolower);

        // Remove extra spaces
        size_t start = result.find_first_not_of(" \t");
        if (start != string::npos) {
            result = result.substr(start);
        }
        size_t end = result.find_last_not_of(" \t");
        if (end != string::npos) {
            result = result.substr(0, end + 1);
        }

        cout << "Normalized: \"" << input << "\" -> \"" << result << "\"" << endl;
        return result;
    }

    bool canProcess(const string& input) const override {
        return !input.empty();
    }

    string getStageType() const override {
        return "Normalization";
    }

    int getPriority() const override {
        return priority;
    }
};

class ValidationStage : public IProcessorStage {
private:
    vector<string> validPatterns;
    int priority;

public:
    ValidationStage(const vector<string>& patterns = {}, int prio = 2)
        : validPatterns(patterns), priority(prio) {
        if (validPatterns.empty()) {
            validPatterns = {"email", "phone", "text"};
        }
        cout << "ValidationStage created with " << validPatterns.size() << " patterns" << endl;
    }

    string process(const string& input) override {
        if (!canProcess(input)) {
            throw runtime_error("Validation failed for input: " + input);
        }

        cout << "Validated: \"" << input << "\" - PASSED" << endl;
        return input; // Return unchanged if valid
    }

    bool canProcess(const string& input) const override {
        // Simple validation: non-empty and contains some text
        return !input.empty() && input.length() >= 3;
    }

    string getStageType() const override {
        return "Validation";
    }

    int getPriority() const override {
        return priority;
    }
};

class EncodingConverter : public IDataConverter {
private:
    string sourceFormat;
    string destinationFormat;

public:
    EncodingConverter(const string& srcFmt = "UTF-8", const string& dstFmt = "ASCII")
        : sourceFormat(srcFmt), destinationFormat(dstFmt) {
        cout << "EncodingConverter created: " << srcFmt << " -> " << dstFmt << endl;
    }

    string convert(const string& input) override {
        if (!canConvert(sourceFormat, destinationFormat)) {
            throw runtime_error("Cannot convert from " + sourceFormat + " to " + destinationFormat);
        }

        // Mock conversion - in reality this would do actual encoding conversion
        string result = input;

        if (destinationFormat == "ASCII") {
            // Remove non-ASCII characters
            result.erase(remove_if(result.begin(), result.end(),
                [](char c) { return static_cast<unsigned char>(c) > 127; }), result.end());
        }

        cout << "Converted encoding: " << sourceFormat << " -> " << destinationFormat << endl;
        return result;
    }

    string getSourceFormat() const override {
        return sourceFormat;
    }

    string getDestinationFormat() const override {
        return destinationFormat;
    }

    bool canConvert(const string& fromFormat, const string& toFormat) const override {
        // Support common conversions
        vector<string> supportedFormats = {"UTF-8", "ASCII", "ISO-8859-1"};
        return find(supportedFormats.begin(), supportedFormats.end(), fromFormat) != supportedFormats.end() &&
               find(supportedFormats.begin(), supportedFormats.end(), toFormat) != supportedFormats.end();
    }
};

class ProcessingPipeline {
private:
    vector<unique_ptr<IProcessorStage>> stages;
    vector<unique_ptr<IDataConverter>> converters;

public:
    ProcessingPipeline() {
        cout << "ProcessingPipeline created" << endl;
    }

    void addStage(unique_ptr<IProcessorStage> stage) {
        cout << "Adding stage: " << stage->getStageType() << endl;
        stages.push_back(move(stage));

        // Sort stages by priority
        sort(stages.begin(), stages.end(),
            [](const unique_ptr<IProcessorStage>& a, const unique_ptr<IProcessorStage>& b) {
                return a->getPriority() < b->getPriority();
            });
    }

    void addConverter(unique_ptr<IDataConverter> converter) {
        cout << "Adding converter: " << converter->getSourceFormat()
             << " -> " << converter->getDestinationFormat() << endl;
        converters.push_back(move(converter));
    }

    void removeStage(const string& stageType) {
        auto it = remove_if(stages.begin(), stages.end(),
            [&stageType](const unique_ptr<IProcessorStage>& stage) {
                return stage->getStageType() == stageType;
            });

        if (it != stages.end()) {
            cout << "Removing stage: " << stageType << endl;
            stages.erase(it, stages.end());
        }
    }

    string processData(const string& input) {
        cout << "\n=== Processing Pipeline Started ===" << endl;
        cout << "Input: \"" << input << "\"" << endl;

        string currentData = input;

        // Process through all stages
        for (const auto& stage : stages) {
            try {
                currentData = stage->process(currentData);
            } catch (const exception& e) {
                cout << "Error in " << stage->getStageType() << ": " << e.what() << endl;
                throw;
            }
        }

        // Apply converters
        for (const auto& converter : converters) {
            try {
                currentData = converter->convert(currentData);
            } catch (const exception& e) {
                cout << "Conversion error: " << e.what() << endl;
                throw;
            }
        }

        cout << "Final output: \"" << currentData << "\"" << endl;
        cout << "=== Processing Pipeline Completed ===" << endl;
        return currentData;
    }

    void displayPipeline() const {
        cout << "\n--- Pipeline Configuration ---" << endl;
        cout << "Processing Stages (" << stages.size() << "):" << endl;
        for (const auto& stage : stages) {
            cout << "  " << stage->getPriority() << ". " << stage->getStageType() << endl;
        }

        cout << "Data Converters (" << converters.size() << "):" << endl;
        for (const auto& converter : converters) {
            cout << "  " << converter->getSourceFormat()
                 << " -> " << converter->getDestinationFormat() << endl;
        }
    }
};

// ========================================
// COMPREHENSIVE DEMO FUNCTIONS
// ========================================

void demonstrateDataPersistence() {
    cout << "\n🗃️ === DATA PERSISTENCE LAYER DEMO === 🗃️" << endl;

    // Create different readers
    vector<unique_ptr<IDataReader>> readers;
    readers.push_back(make_unique<SQLDataReader>("mysql://localhost:3306/testdb"));
    readers.push_back(make_unique<JSONDataReader>("users.json"));

    // Create different writers
    vector<unique_ptr<IDataWriter>> writers;
    writers.push_back(make_unique<FileDataWriter>("output.txt"));

    // Demonstrate polymorphic behavior
    cout << "\nReading data from multiple sources:" << endl;
    for (auto& reader : readers) {
        cout << "\nSource: " << reader->getDataSource() << endl;
        while (reader->hasNext()) {
            string data = reader->read();
            cout << "  Data: " << data << endl;

            // Write to all writers
            for (auto& writer : writers) {
                writer->write(data);
            }
        }
        reader->close();
    }

    // Close all writers
    for (auto& writer : writers) {
        writer->close();
    }
}

void demonstrateCommunication() {
    cout << "\n📡 === COMMUNICATION PROTOCOL DEMO === 📡" << endl;

    // Create TCP handler (multiple inheritance)
    auto tcpHandler = make_unique<TCPMessageHandler>("192.168.1.100", 8080);

    // Use as both sender and receiver
    tcpHandler->connect("192.168.1.100");
    tcpHandler->listen(8080);

    // Send some messages
    tcpHandler->send("Hello, TCP World!", "192.168.1.101");
    tcpHandler->send("Data packet #1", "192.168.1.102");

    // Receive some messages
    for (int i = 0; i < 3; i++) {
        string message = tcpHandler->receive();
        cout << "Processed message: " << message << endl;
    }

    // Create other communication objects
    auto udpSender = make_unique<UDPMessageSender>();
    auto wsReceiver = make_unique<WebSocketReceiver>();

    // Demonstrate interface polymorphism
    vector<IMessageSender*> senders;
    senders.push_back(tcpHandler.get());
    senders.push_back(udpSender.get());

    cout << "\nPolymorphic message sending:" << endl;
    for (auto* sender : senders) {
        sender->connect("test.server.com");
        sender->send("Polymorphic message", "broadcast");
        cout << "Connection: " << sender->getConnectionInfo() << endl;
    }

    vector<IMessageReceiver*> receivers;
    receivers.push_back(tcpHandler.get());
    receivers.push_back(wsReceiver.get());

    cout << "\nPolymorphic message receiving:" << endl;
    for (auto* receiver : receivers) {
        receiver->listen(9090);
        if (receiver->isListening()) {
            string msg = receiver->receive();
            cout << "Received on port " << receiver->getPort() << ": " << msg << endl;
        }
    }
}

void demonstrateProcessingPipeline() {
    cout << "\n⚙️ === PROCESSING PIPELINE DEMO === ⚙️" << endl;

    // Create pipeline
    ProcessingPipeline pipeline;

    // Add stages (will be sorted by priority)
    pipeline.addStage(make_unique<ValidationStage>());
    pipeline.addStage(make_unique<NormalizationStage>());

    // Add converters
    pipeline.addConverter(make_unique<EncodingConverter>("UTF-8", "ASCII"));

    pipeline.displayPipeline();

    // Process different types of data
    vector<string> testData = {
        "  Hello World!  ",
        "UPPERCASE TEXT with Émojis! 🚀",
        "validation@test.com",
        "  Multiple    Spaces   Between   Words  "
    };

    cout << "\nProcessing test data through pipeline:" << endl;
    for (const string& data : testData) {
        try {
            string result = pipeline.processData(data);
            cout << "SUCCESS: \"" << data << "\" -> \"" << result << "\"" << endl;
        } catch (const exception& e) {
            cout << "FAILED: \"" << data << "\" - " << e.what() << endl;
        }
        cout << string(50, '-') << endl;
    }
}

// ========================================
// MAIN FUNCTION - COMPREHENSIVE SOLUTION
// ========================================

int main() {
    cout << "=== Interface-Based System Architecture - SOLUTION ===" << endl;
    cout << "Comprehensive demonstration of all three scenarios" << endl;

    try {
        // Demonstrate all three architectural scenarios
        // demonstrateDataPersistence();
        demonstrateCommunication();
        // demonstrateProcessingPipeline();

        // ========================================
        // ARCHITECTURE ANALYSIS
        // ========================================
        cout << "\n📊 === ARCHITECTURE ANALYSIS === 📊" << endl;

        cout << "\nDesign Principles Applied:" << endl;
        cout << "✓ Interface Segregation Principle - Focused, single-responsibility interfaces" << endl;
        cout << "✓ Open/Closed Principle - Easy to add new implementations without modifying existing code" << endl;
        cout << "✓ Dependency Inversion Principle - High-level modules depend on abstractions" << endl;
        cout << "✓ Single Responsibility Principle - Each class has one reason to change" << endl;
        cout << "✓ Liskov Substitution Principle - Any implementation can replace its interface" << endl;

        cout << "\nPolymorphic Benefits Demonstrated:" << endl;
        cout << "✓ Runtime behavior selection through virtual dispatch" << endl;
        cout << "✓ Easy extensibility - new implementations integrate seamlessly" << endl;
        cout << "✓ Loose coupling between components" << endl;
        cout << "✓ Testability through interface mocking and substitution" << endl;
        cout << "✓ Container-based polymorphism with interface pointers" << endl;

        cout << "\nMultiple Inheritance Success:" << endl;
        cout << "✓ TCPMessageHandler implements both IMessageReceiver and IMessageSender" << endl;
        cout << "✓ No diamond problem - interfaces contain only pure virtual functions" << endl;
        cout << "✓ Clear interface contracts with no ambiguous method calls" << endl;
        cout << "✓ Proper virtual destructors ensure safe polymorphic deletion" << endl;

        cout << "\nArchitectural Flexibility:" << endl;
        cout << "✓ New data sources can be added by implementing IDataReader" << endl;
        cout << "✓ New communication protocols can be added by implementing message interfaces" << endl;
        cout << "✓ New processing stages can be added to the pipeline dynamically" << endl;
        cout << "✓ All systems support configuration changes at runtime" << endl;

    } catch (const exception& e) {
        cout << "Demo error: " << e.what() << endl;
        return 1;
    }

    cout << "\n🎯 Interface-Based System Architecture Demo Complete! 🎯" << endl;
    cout << "\nKey Takeaways:" << endl;
    cout << "• Interfaces provide contracts for flexible system design" << endl;
    cout << "• Multiple inheritance can be powerful when used correctly" << endl;
    cout << "• Polymorphism enables runtime flexibility and extensibility" << endl;
    cout << "• SOLID principles guide robust architectural decisions" << endl;
    cout << "• Interface-based design supports testing, maintenance, and evolution" << endl;

    return 0;
}

/*
SOLUTION HIGHLIGHTS:

SCENARIO 1: DATA PERSISTENCE LAYER
✓ IDataReader and IDataWriter interfaces with focused responsibilities
✓ SQLDataReader, JSONDataReader, and FileDataWriter implementations
✓ Polymorphic data processing with different sources and destinations
✓ Proper resource management with RAII principles
✓ Error handling for closed connections and invalid operations

SCENARIO 2: COMMUNICATION PROTOCOL
✓ IMessageReceiver and IMessageSender interfaces for communication
✓ TCPMessageHandler with successful multiple inheritance
✓ UDPMessageSender and WebSocketReceiver for protocol diversity
✓ Polymorphic message handling through interface pointers
✓ Connection management and state tracking

SCENARIO 3: PROCESSING PIPELINE
✓ IProcessorStage and IDataConverter interfaces for data processing
✓ NormalizationStage, ValidationStage, and EncodingConverter implementations
✓ ProcessingPipeline class managing stages with priority ordering
✓ Dynamic stage addition/removal and flexible data flow
✓ Error handling and recovery in processing chains

ADVANCED FEATURES:
✓ Comprehensive error handling with exceptions
✓ RAII resource management with smart pointers
✓ Priority-based processing stage ordering
✓ Mock data generation for realistic demonstrations
✓ Professional logging and status reporting

DESIGN PATTERNS DEMONSTRATED:
✓ Strategy Pattern - Different algorithms through interfaces
✓ Chain of Responsibility - Processing pipeline stages
✓ Factory Pattern potential - Object creation through interfaces
✓ Observer Pattern potential - Message notification systems

MULTIPLE INHERITANCE SUCCESS:
✓ TCPMessageHandler cleanly implements both communication interfaces
✓ No diamond problem - pure virtual interfaces only
✓ Clear method resolution without ambiguity
✓ Proper virtual destructor chains for safe deletion

REAL-WORLD APPLICATIONS:
✓ Database abstraction layers with multiple drivers
✓ Network protocol stacks with different transport layers
✓ Data processing frameworks with pluggable stages
✓ Microservice communication with various protocols
✓ ETL (Extract, Transform, Load) systems with flexible pipelines

This solution demonstrates professional-level interface-based
architecture suitable for complex enterprise systems requiring
high flexibility, maintainability, and extensibility.
*/
