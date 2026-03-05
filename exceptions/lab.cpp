#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <thread>
#include <random>
using namespace std;
using namespace std::chrono;


// TODO: Create base AppException class
// Requirements:
// - Inherit from std::exception
// - Store error message and error code
// - Override what() method
// - Include timestamp when exception was created

class AppException : public std::exception /* TODO: Add inheritance */ {
protected:
  // TODO: Add protected members
  string errorMessage;
  int errorCode;
  string timestamp;

public:
  // TODO: Constructor with message and optional error code
  AppException(const string &message, int code = 0)
      : errorMessage(message), errorCode(code),
        timestamp(getCurrentTimestamp()) {
    cout << "AppException created: " << message << " [Code: " << code << "]"
         << endl;
  }

  // TODO: Override what() method
  const char *what() const noexcept override { return errorMessage.c_str(); }

  // TODO: Additional methods
  int getErrorCode() const { return errorCode; }
  string getTimestamp() const { return timestamp; }

  virtual string getExceptionType() const { return "AppException"; }
  virtual string getDetailedMessage() const {
    stringstream ss;
    ss << "[" << timestamp << "]" << getExceptionType()
       << " (Code: " << errorCode << "): " << errorMessage;
    return ss.str();
  }

private:
  // TODO: Helper method to get current timestamp
  string getCurrentTimestamp() const {
    auto now = system_clock::now();
    auto time_t = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    stringstream ss;
    ss << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << setfill('0') << setw(3) << ms.count();
    return ss.str();
  }
};
// TODO: Create FileErrorException class
// Requirements:
// - Inherit from AppException
// - Store filename and operation type
// - Provide specific error messages for file operations

class FileErrorException : public AppException /* TODO: Add inheritance */ {
private:
  // TODO: Add file-specific members
  string filename;
  string operation; // "read", "write", "open", "close"

public:
  // TODO: Constructor
  FileErrorException(const string &file, const string &op,
                     const string &message, int code = 1001)
      : AppException("File Error: " + message + " (File: " + file +
                         ", Operation: " + op + ")",
                     code),
        filename(file), operation(op) {
    cout << "FileErrorException created for file: " << file
         << ", operation: " << op << endl;
  }

  // TODO: Getters
  string getFilename() const { return filename; }
  string getOperation() const { return operation; }
  string getExceptionType() const override { return "FileErrorException"; }

  string getDetailedMessage() const override {
    stringstream ss;
    ss << AppException::getDetailedMessage() << " | File: " << filename
       << " | Operation: " << operation;
    return ss.str();
  }
};

// TODO: Create ValidationError class
// Requirements:
// - Inherit from AppException
// - Store field name and invalid value
// - Provide validation-specific error details

class ValidationError : public AppException /* TODO: Add inheritance */ {
private:
  // TODO: Add validation-specific members
  string fieldName;
  string invalidValue;
  string validationRule;

public:
  // TODO: Constructor
  ValidationError(const string &field, const string &value, const string &rule,
                  int code = 2001)
      : AppException("Validation Error: Field '" + field + "' with value '" +
                         value + "' violates rule: " + rule,
                     code),
        fieldName(field), invalidValue(value), validationRule(rule) {
    cout << "ValidationError created for field: " << field
         << ", value: " << value << endl;
  }

  // TODO: Getters
  string getFieldName() const { return fieldName; }
  string getInvalidValue() const { return invalidValue; }
  string getValidationRule() const { return validationRule; }
  string getExceptionType() const override { return "ValidationError"; }
  string getDetailedMessage() const override {
    stringstream ss;
    ss << AppException::getDetailedMessage() << " | Field: " << fieldName
       << " | Value: '" << invalidValue << "' | Rule: " << validationRule;
    return ss.str();
  }
};

// TODO: Create ProcessingError class
// Requirements:
// - Inherit from AppException
// - Store processing stage and context information
// - Handle general processing faults

class ProcessingError : public AppException /* TODO: Add inheritance */ {
private:
  // TODO: Add processing-specific members
  string processingStage;
  string contextInfo;

public:
  // TODO: Constructor
  ProcessingError(const string &stage, const string &context,
                  const string &message, int code = 3001)
      : AppException("Processing Error in stage '" + stage + "': " + message,
                     code),
        processingStage(stage), contextInfo(context) {
    cout << "ProcessingError created for stage: " << stage << endl;
  }

  string getProcessingStage() const { return processingStage; }

  string getContextInfo() const { return contextInfo; }

  string getExceptionType() const override { return "ProcessingError"; }

  string getDetailedMessage() const override {
    stringstream ss;
    ss << AppException::getDetailedMessage() << " | Stage: " << processingStage
       << " | Context: " << contextInfo;
    return ss.str();
  }
};

// TODO: Create ErrorLogger class
class ErrorLogger {
private:
  // TODO: Add logging-specific members
  string logFilename;
  static ErrorLogger *instance; // singleton pattern

public:
  // TODO: Singleton getInstance method
  static ErrorLogger &
  getInstance(const string &logFile = "application_error.log") {
    if (instance == nullptr)
      instance = new ErrorLogger(logFile);
    return *instance;
  }

  // TODO: Log exception method
  void logException(const AppException &ex, const string &context = "") {
    //     TODO: Write exception details to log file
    //     TODO: Include timestamp, exception type, message, context
    string logEntry = formatLogEntry("ERROR", ex.getDetailedMessage());
    if (!context.empty()) {
      logEntry += " | Context: " + context;
    }
    writeToLog(logEntry);
    cout << "LOGGED EXCEPTION: " << ex.getExceptionType() << endl;
  }

  void displayRecentLogs(int count = 5) {
      ifstream logFile(logFilename);
      vector<string> lines;
      string line;

      while (getline(logFile, line)) {
          lines.push_back(line);
      }

      cout << "\n--- Recent Log Entries (last " << min(count, (int)lines.size()) << ") ---" << endl;
      int start = max(0, (int)lines.size() - count);
      for (int i = start; i < lines.size(); i++) {
          cout << lines[i] << endl;
      }
  }

  // TODO: Log general error method
  void logError(const string &message, const string &severity = "ERROR") {
    //     TODO: Log general error messages
  }

  void logWarning(const string &message) {
    writeToLog(formatLogEntry("WARNING", message));
    cout << "LOGGED WARNING: " << message << endl;
  }

  // TODO: Log info method
  void logInfo(const string &message) {
    //     TODO: Log informational messages
  }

private:
  // TODO: Private constructor for singleton
  ErrorLogger(const string &logFile) : logFilename(logFile) {
    ofstream file(logFile, ios::app);
    if (file.is_open()) {
      file << "\n=== Application Error Log Session Started === " << endl;
      file << "Timestamp: " << getCurrentTimeStamp() << endl;
      file << string(50, '=') << endl;
    }
    cout << "ErrorLogger initialized with file:  " << logFilename << endl;
  }

  // TODO: Helper method to format log entry
  string formatLogEntry(const string &level, const string &message) const {
    //     TODO: Format: [TIMESTAMP] [LEVEL] MESSAGE
    stringstream ss;
    ss << "[" << getCurrentTimeStamp() << "] [" << level << "] " << message;
    return ss.str();
  }

  void writeToLog(const string &logEntry) {
    ofstream file(logFilename, ios::app);
    if (file.is_open()) {
      file << logEntry << endl;
    }
  }
  string getCurrentTimeStamp() const {
    auto now = system_clock::now();
    auto time_t = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    stringstream ss;
    ss << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << setfill('0') << setw(3) << ms.count();
    return ss.str();
  }
};

// Initialisze static member
ErrorLogger *ErrorLogger::instance = nullptr;

// TODO: Create RecoveryManager class
class RecoveryManager {
private:
  // TODO: Add recovery-specific members
  int maxRetries;
  vector<string> backupFiles;
  ErrorLogger &logger;

public:
  // TODO: Constructor
  RecoveryManager(int retries = 3)
      : maxRetries(retries), logger(ErrorLogger::getInstance()) {
    logger.logInfo("RecoveryManager initialized with max retries: " +
                   to_string(retries));
    cout << "RecoveryManager created with " << retries << " max retries"
         << endl;
  }

  // TODO: Retry mechanism for file operations
  bool retryFileOperation(function<void()> operation,
                          const string &operationName, int maxAttempts = 3) {
    logger.logInfo("Starting retry operation: " + operationName);

    for (int attempt = 1; attempt <= maxAttempts; attempt++) {
      try {
        logger.logInfo("Attempt " + to_string(attempt) + " of " +
                       to_string(maxAttempts) +
                       " for operation: " + operationName);

        operation();

        logger.logInfo("Operation succeeded on attempt " + to_string(attempt));
        cout << "✅ Operation '" << operationName << "' succeeded on attempt "
             << attempt << endl;
        return true;

      } catch (const AppException &ex) {
        logger.logException(ex, "Retry attempt " + to_string(attempt) +
                                    " for " + operationName);

        if (attempt == maxAttempts) {
          logger.logError("All retry attempts failed for operation: " +
                              operationName,
                          "CRITICAL");
          cout << "❌ All " << maxAttempts << " attempts failed for '"
               << operationName << "'" << endl;
          throw;
        }

        // Exponential backoff
        int waitTime = attempt * attempt * 100; // 100ms, 400ms, 900ms, etc.
        logger.logInfo("Waiting " + to_string(waitTime) + "ms before retry");
        cout << "⏳ Waiting " << waitTime << "ms before retry..." << endl;
        this_thread::sleep_for(milliseconds(waitTime));
      }
    }

    return false;
  }

  // TODO: Recovery strategy for validation errors
  string handleValidationError(const ValidationError &ex,
                               const string &originalData) {
    //     TODO: Prompt user for correction or provide default values
    //     TODO: Attempt to auto-correct simple validation issues
    //     TODO: Log recovery attempts
    logger.logException(ex, "Attempting validation error recovery");
    string field = ex.getFieldName();
    string value = ex.getInvalidValue();
    string rule = ex.getValidationRule();

    cout << "\n Validation Error Recovery: " << endl;
    cout << "Field: " << field << ", Invalid Value: '" << value
         << "', Rule: " << rule << endl;

    // Simple auto-correction strategies
    if (rule.find("non-empty") != string::npos && value.empty()) {
      string correctedValue = "default_" + field;
      logger.logInfo("Auto-corrected empty field '" + field +
                     "' to: " + correctedValue);
      cout << "✅ Auto-corrected empty field to: " << correctedValue << endl;
      return correctedValue;
    }

    if (rule.find("numeric") != string::npos) {
      // Extract numbers from the value
      string numbers;
      for (char c : value) {
        if (isdigit(c) || c == '.') {
          numbers += c;
        }
      }
      if (!numbers.empty()) {
        logger.logInfo("Auto-corrected non-numeric value '" + value +
                       "' to: " + numbers);
        cout << "✅ Extracted numeric value: " << numbers << endl;
        return numbers;
      }
    }

    if (rule.find("email") != string::npos) {
      if (value.find("@") == string::npos) {
        string corrected = value + "@example.com";
        logger.logInfo("Auto-corrected email format: " + corrected);
        cout << "✅ Auto-corrected email format: " << corrected << endl;
        return corrected;
      }
    }

    // If auto-correction fails, prompt user (simulated)
    logger.logWarning("Could not auto-correct field '" + field +
                      "', using default recovery");
    cout << "⚠️ Could not auto-correct, using default value for field: " << field
         << endl;
    return "RECOVERED_" + field + "_VALUE";
  }

  // TODO: Rollback mechanism for processing errors
  void rollbackProcessing(const ProcessingError &ex,
                          vector<string> &processedData) {
    logger.logException(ex, "Initiating processing rollback");

    string stage = ex.getProcessingStage();
    string context = ex.getContextInfo();

    cout << "\n🔄 Processing Rollback:" << endl;
    cout << "Failed Stage: " << stage << ", Context: " << context << endl;

    // Create backup before rollback
    vector<string> backup = processedData;
    string backupName = "rollback_backup_" + to_string(time(nullptr)) + ".bak";
    backupFiles.push_back(backupName);

    logger.logInfo("Created rollback backup: " + backupName);

    // Rollback strategies based on stage
    if (stage == "normalization") {
      // Restore to previous state before normalization
      logger.logInfo("Rolling back normalization stage");
      cout << "✅ Rolled back normalization processing" << endl;
    } else if (stage == "validation") {
      // Keep processed data but mark as unvalidated
      logger.logInfo("Rolling back validation stage");
      cout << "✅ Rolled back validation processing" << endl;
    } else if (stage == "transformation") {
      // Remove transformed records
      int originalSize = processedData.size();
      processedData.clear();
      logger.logInfo("Cleared " + to_string(originalSize) +
                     " transformed records");
      cout << "✅ Cleared " << originalSize << " transformed records" << endl;
    }

    logger.logInfo("Processing rollback completed for stage: " + stage);
  }

  // TODO: Create backup before processing
  void createBackup(const string &originalFile) {
    //     TODO: Create backup copy of original data
    //     TODO: Log backup creation
    string backupName = originalFile + ".backup_" + to_string(time(nullptr));
    backupFiles.push_back(backupName);

    // Simulate backup creation
    logger.logInfo("Created backup: " + backupName +
                   " for file: " + originalFile);
    cout << "📦 Created backup: " << backupName << endl;
  }

  // TODO: Restore from backup
  bool restoreFromBackup(const string &backupFile) {
    //     TODO: Restore data from backup
    //     TODO: Log restoration process
    auto it = find(backupFiles.begin(), backupFiles.end(), backupFile);
    if (it != backupFiles.end()) {
      logger.logInfo("Restoring from backup: " + backupFile);
      cout << "📋 Restored from backup: " << backupFile << endl;
      return true;
    } else {
      logger.logError("Backup file not found: " + backupFile);
      cout << "❌ Backup file not found: " << backupFile << endl;
      return false;
    }
  }

  void displayBackups() const {
    cout << "\n📦 Available Backups (" << backupFiles.size() << "):" << endl;
    for (const string &backup : backupFiles) {
      cout << "  - " << backup << endl;
    }
  }

  int getMaxRetries() const { return maxRetries; }
  size_t getBackupCount() const { return backupFiles.size(); }
};

// TODO: Create DataProcessor class
class DataProcessor {
private:
  // TODO: Add private members
  vector<string> processedData;
  string currentFile;
  int recordsProcessed;
  ErrorLogger &logger;
  RecoveryManager recoveryManager;
  random_device rd;
  mutable mt19937 gen;

public:
  // TODO: Constructor
  DataProcessor()
      : recordsProcessed(0), logger(ErrorLogger::getInstance()),
        recoveryManager(3), gen(rd()) {
    logger.logInfo("DataProcessor initialized");
    cout << "DataProcessor created with error handling and recovery" << endl;
  }

  // TODO: File operation method that can throw FileErrorException
  void loadDataFromFile(const string &filename) {
    // TODO: Simulate file operations
    // TODO: Check if file exists, readable, etc.
    // TODO: Throw FileErrorException for various file issues
    currentFile = filename;

    logger.logInfo("Attempting to load data from file: " + filename);

    // Simulate various file error conditions
    uniform_int_distribution<> dis(1, 10);
    int scenario = dis(gen);

    if (filename.empty()) {
      throw FileErrorException("", "open", "Filename cannot be empty", 1001);
    }

    if (filename.find(".txt") == string::npos &&
        filename.find(".csv") == string::npos) {
      throw FileErrorException(filename, "open", "Unsupported file format",
                               1002);
    }

    if (scenario <= 3) { // 30% chance of file not found
      throw FileErrorException(filename, "open",
                               "File not found or access denied", 1003);
    }

    if (scenario <= 5) { // 20% chance of read error
      throw FileErrorException(filename, "read",
                               "File is corrupted or unreadable", 1004);
    }

    // Success case
    logger.logInfo("File loaded successfully: " + filename);
    cout << "✅ File loaded successfully: " << filename << endl;
  }

  // TODO: Data validation method that can throw ValidationError
  void validateData(const string &data) {
    //     TODO: Implement validation rules
    //     TODO: Check for empty data, invalid formats, etc.
    //     TODO: Throw ValidationError for validation failures
    logger.logInfo("Validating data: " + data.substr(0, 50) + "...");

    // Validation rules
    if (data.empty()) {
      throw ValidationError("data", data, "non-empty", 2001);
    }

    if (data.length() < 3) {
      throw ValidationError("data", data, "minimum length of 3 characters",
                            2002);
    }

    // Check for email format if it looks like an email field
    if (data.find("email:") != string::npos) {
      string email = data.substr(data.find(":") + 1);
      if (email.find("@") == string::npos || email.find(".") == string::npos) {
        throw ValidationError("email", email, "valid email format", 2003);
      }
    }

    // Check for numeric fields
    if (data.find("age:") != string::npos || data.find("id:") != string::npos) {
      string value = data.substr(data.find(":") + 1);
      if (value.find_first_not_of("0123456789") != string::npos) {
        throw ValidationError("numeric_field", value, "numeric value only",
                              2004);
      }
    }

    logger.logInfo("Data validation passed");
    cout << "✅ Data validation passed for: " << data.substr(0, 30) << "..."
         << endl;
  }

  // TODO: Processing method that can throw ProcessingError
  string processRecord(const string &record) {
    //     TODO: Implement data processing logic
    //     TODO: Simulate processing that might fail
    //     TODO: Throw ProcessingError for processing issues
    logger.logInfo("Processing record: " + record);
    // Simulate different processing stages that might fail
    uniform_int_distribution<> dis(1, 10);
    int scenario = dis(gen);

    // Normalization stage
    if (scenario <= 2) { // 20% chance of normalization error
      throw ProcessingError("normalization",
                            "record_id=" + to_string(recordsProcessed),
                            "Failed to normalize record format", 3001);
    }

    // Validation stage
    if (scenario <= 4) { // 20% chance of validation error during processing
      throw ProcessingError("validation",
                            "record_content=" + record.substr(0, 20),
                            "Record failed business rule validation", 3002);
    }

    // Transformation stage
    if (scenario <= 6) { // 20% chance of transformation error
      throw ProcessingError("transformation", "transform_type=uppercase",
                            "Data transformation failed", 3003);
    }

    // Success - transform the record
    string processed = "PROCESSED_" + record;
    transform(processed.begin(), processed.end(), processed.begin(), ::toupper);

    recordsProcessed++;
    logger.logInfo("Record processed successfully");
    cout << "✅ Record processed: " << processed.substr(0, 50) << "..." << endl;

    return processed;
  }

  // TODO: Main processing function with comprehensive error handling
  void processDataFile(const string &filename) {
    //     TODO: Use try-catch blocks to handle different exception types
    //     TODO: Implement specific handling for each exception type
    //     TODO: Include generic catch-all for unexpected exceptions
    logger.logInfo("Starting comprehensive data processing for file: " +
                   filename);
    cout << "\n🚀 Starting data processing for: " << filename << endl;

    recoveryManager.createBackup(filename);

    try {
      // Step 1: Load data with retry mechanism
      auto loadOperation = [this, filename]() {
        this->loadDataFromFile(filename);
      };

      if (!recoveryManager.retryFileOperation(loadOperation,
                                              "file_load_" + filename, 3)) {
        logger.logError("Failed to load file after all retries: " + filename,
                        "CRITICAL");
        return;
      }

      // Step 2: Process mock data records
      vector<string> mockData = {
          "id:1,name:John,email:john@example.com,age:25",
          "id:2,name:Jane,email:jane-invalid-email,age:30",
          "id:3,name:Bob,email:bob@test.com,age:invalid_age",
          "id:4,name:Alice,email:alice@company.com,age:28",
          "", // Empty record to trigger validation error
          "id:5,name:Charlie,email:charlie@domain.com,age:35"};

      logger.logInfo("Processing " + to_string(mockData.size()) +
                     " data records");

      for (size_t i = 0; i < mockData.size(); i++) {
        string record = mockData[i];

        try {
          // Validate each record
          validateData(record);

          // Process each record
          string processedRecord = processRecord(record);
          processedData.push_back(processedRecord);

        } catch (const ValidationError &ex) {
          logger.logException(ex, "Record #" + to_string(i));

          // Attempt recovery for validation errors
          try {
            string recoveredData =
                recoveryManager.handleValidationError(ex, record);
            string processedRecord = processRecord(recoveredData);
            processedData.push_back(processedRecord);

            logger.logInfo("Successfully recovered from validation error");

          } catch (const exception &recoveryEx) {
            logger.logError("Recovery failed for record #" + to_string(i) +
                                ": " + recoveryEx.what(),
                            "WARNING");
            cout << "⚠️ Skipping record #" << i << " due to recovery failure"
                 << endl;
          }

        } catch (const ProcessingError &ex) {
          logger.logException(ex, "Record #" + to_string(i));

          // Attempt rollback for processing errors
          recoveryManager.rollbackProcessing(ex, processedData);

          // Continue with next record
          logger.logWarning("Continuing with next record after rollback");

        } catch (const AppException &ex) {
          logger.logException(ex, "Unexpected application error for record #" +
                                      to_string(i));
          cout << "❌ Unexpected error processing record #" << i << ": "
               << ex.what() << endl;

        } catch (const std::exception &ex) {
          logger.logError("System exception for record #" + to_string(i) +
                              ": " + ex.what(),
                          "CRITICAL");
          cout << "💥 System exception: " << ex.what() << endl;

        } catch (...) {
          logger.logError("Unknown exception for record #" + to_string(i),
                          "CRITICAL");
          cout << "❓ Unknown exception occurred" << endl;
        }
      }

    } catch (const FileErrorException &ex) {
      logger.logException(ex, "File processing failed");
      cout << "📁 File Error: " << ex.what() << endl;

    } catch (const AppException &ex) {
      logger.logException(ex, "Application error during file processing");
      cout << "⚠️ Application Error: " << ex.what() << endl;

    } catch (const std::exception &ex) {
      logger.logError("System exception during file processing: " +
                          string(ex.what()),
                      "CRITICAL");
      cout << "💥 System Exception: " << ex.what() << endl;

    } catch (...) {
      logger.logError("Unknown exception during file processing", "CRITICAL");
      cout << "❓ Unknown exception occurred during processing" << endl;
    }

    // Final processing summary
    logger.logInfo("Processing completed. Records processed: " +
                   to_string(recordsProcessed) +
                   ", Total in output: " + to_string(processedData.size()));
    cout << "\n📊 Processing Summary:" << endl;
    cout << "Records processed successfully: " << recordsProcessed << endl;
    cout << "Records in final output: " << processedData.size() << endl;

    recoveryManager.displayBackups();
  }

  // TODO: Getter methods
  int getRecordsProcessed() const { return recordsProcessed; }
  vector<string> getProcessedData() const { return processedData; }
  void displayProcessedData() const {
    cout << "\n📋 Processed Data (" << processedData.size()
         << " records):" << endl;
    for (size_t i = 0; i < processedData.size(); i++) {
      cout << "  " << (i + 1) << ". " << processedData[i].substr(0, 60)
           << (processedData[i].length() > 60 ? "..." : "") << endl;
    }
  }
};
// ========================================
// COMPREHENSIVE INTEGRATION DEMO - SOLUTION
// ========================================

void demonstrateErrorHandlingSystem() {
  cout << "\n🎯 === COMPREHENSIVE ERROR HANDLING DEMONSTRATION === 🎯" << endl;

  ErrorLogger &logger = ErrorLogger::getInstance();
  logger.logInfo("Starting comprehensive error handling system demonstration");

  // Test different file scenarios
  vector<string> testFiles = {
      "valid_data.csv",    // Should work after retries
      "",                  // Empty filename error
      "invalid.exe",       // Unsupported format
      "missing_file.txt",  // File not found
      "corrupted_data.csv" // Read error
  };

  DataProcessor processor;

  for (const string &filename : testFiles) {
    cout << "\n" << string(60, '=') << endl;
    cout << "🔍 Testing file: '" << filename << "'" << endl;
    cout << string(60, '=') << endl;

    try {
      processor.processDataFile(filename);

    } catch (const AppException &ex) {
      cout << "🚨 Caught AppException: " << ex.getExceptionType() << endl;
      cout << "   Message: " << ex.what() << endl;
      logger.logException(ex, "Main processing loop");

    } catch (const std::exception &ex) {
      cout << "💥 Caught std::exception: " << ex.what() << endl;
      logger.logError("Standard exception in main: " + string(ex.what()),
                      "CRITICAL");

    } catch (...) {
      cout << "❓ Caught unknown exception" << endl;
      logger.logError("Unknown exception in main processing loop", "CRITICAL");
    }

    // Brief pause between tests
    this_thread::sleep_for(milliseconds(500));
  }

  // Display final results
  cout << "\n📈 === FINAL PROCESSING RESULTS === 📈" << endl;
  processor.displayProcessedData();

  // Show recent log entries
  logger.displayRecentLogs(10);

  logger.logInfo("Comprehensive error handling demonstration completed");
}

int main() {
  cout << "=== Comprehensive Error Handling System ===" << endl;
  cout << "Duration: 45-60 minutes" << endl;
  cout << "Scenario: Data Analytics Application with Robust Error Management"
       << endl;

  try {
    // Step 1: Test Exception Hierarchy
    cout << "\n--- Step 1: Testing Exception Hierarchy ---" << endl;

    try {
      throw FileErrorException("test.txt", "read", "Test file error");
    } catch (const AppException &ex) {
      cout << "Caught and handled FileErrorException" << endl;
      cout << "Details: " << ex.getDetailedMessage() << endl;
    }

    try {
      throw ValidationError("email", "invalid-email",
                            "valid email format required");
    } catch (const AppException &ex) {
      cout << "Caught and handled ValidationError" << endl;
      cout << "Details: " << ex.getDetailedMessage() << endl;
    }

    try {
      throw ProcessingError("transformation", "data_batch_001",
                            "Transformation failed");
    } catch (const AppException &ex) {
      cout << "Caught and handled ProcessingError" << endl;
      cout << "Details: " << ex.getDetailedMessage() << endl;
    }

    // Step 2-5: Comprehensive Integration Demo
    demonstrateErrorHandlingSystem();

    // Success Checklist Verification
    cout << "\n✅ === SUCCESS CHECKLIST VERIFICATION === ✅" << endl;
    cout << "✓ Custom exception hierarchy with meaningful messages" << endl;
    cout << "✓ Robust error handling in data processing functions" << endl;
    cout << "✓ Comprehensive logging strategy implemented" << endl;
    cout << "✓ Recovery mechanisms for different error types" << endl;
    cout
        << "✓ Integration demonstration showing all components working together"
        << endl;

    cout << "\n🏆 Error Handling Principles Successfully Applied:" << endl;
    cout << "✓ Specific exception types for different error categories" << endl;
    cout << "✓ Detailed error messages with context information" << endl;
    cout << "✓ Logging with timestamps and severity levels" << endl;
    cout << "✓ Retry mechanisms with exponential backoff" << endl;
    cout << "✓ Graceful degradation and recovery strategies" << endl;
    cout << "✓ Comprehensive exception safety throughout application" << endl;

  } catch (const std::exception &ex) {
    cout << "💥 Unhandled exception in main: " << ex.what() << endl;
    ErrorLogger::getInstance().logError(
        "Unhandled exception in main: " + string(ex.what()), "CRITICAL");
    return 1;

  } catch (...) {
    cout << "❓ Unknown exception in main" << endl;
    ErrorLogger::getInstance().logError("Unknown exception in main",
                                        "CRITICAL");
    return 1;
  }

  cout << "\n🎯 Comprehensive Error Handling System Demo Complete! 🎯" << endl;


  return 0;
}

/*
IMPLEMENTATION GUIDE:

STEP 1: Custom Exception Hierarchy
□ Create AppException base class inheriting from std::exception
□ Override what() method to provide meaningful error messages
□ Add timestamp and error code support
□ Create specific exception classes for different error types
□ Include context-specific information in each exception

STEP 2: Error Handling in Data Processing
□ Implement DataProcessor class with methods that can throw exceptions
□ Use appropriate exception types for different error conditions
□ Implement comprehensive try-catch blocks
□ Include specific handling for each exception type
□ Add generic catch-all for unexpected exceptions

STEP 3: Logging Strategy
□ Implement ErrorLogger singleton class
□ Log exception details with context information
□ Include timestamps and severity levels
□ Write logs to file for persistence
□ Format log entries consistently

STEP 4: Recovery Mechanisms
□ Implement RecoveryManager class
□ Add retry logic with exponential backoff
□ Implement validation error recovery
□ Add rollback mechanisms for processing errors
□ Create backup and restore functionality

STEP 5: Integration
□ Demonstrate all components working together
□ Test various error scenarios
□ Show logging and recovery in action
□ Verify graceful error handling throughout

KEY CONCEPTS TO UNDERSTAND:
- Custom exceptions provide specific error handling
- Logging helps with debugging and maintenance
- Recovery mechanisms improve application resilience
- Comprehensive error handling improves user experience

COMMON PITFALLS TO AVOID:
□ Generic exception handling without specificity
□ Missing error context information
□ Inadequate logging for debugging
□ No recovery strategies for common errors
□ Catching exceptions without proper handling
□ Not using RAII for resource management

SUCCESS CRITERIA:
□ All custom exception classes properly implemented
□ Comprehensive error handling in data processing
□ Effective logging system with proper formatting
□ Recovery mechanisms that actually work
□ Integration demo shows resilient application behavior
□ Code follows best practices for exception safety
*/
