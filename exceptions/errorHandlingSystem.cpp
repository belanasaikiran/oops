#include <iostream>
#include <exception>
#include <string>

// Base exception class
class BankingException : public std::exception {
protected:
    std::string errorMessage;
    int errorCode;

public:
    BankingException(const std::string& msg, int code = 0)
        : errorMessage(msg), errorCode(code) {}

    const char* what() const noexcept override {
        return errorMessage.c_str();
    }

    int getErrorCode() const { return errorCode; }
};

// Specific exception types
class InsufficientFundsException : public BankingException {
public:
    InsufficientFundsException(double requested, double available)
        : BankingException("Insufficient funds: requested $" + std::to_string(requested) +
                          ", available $" + std::to_string(available), 1001) {}
};

class InvalidAccountException : public BankingException {
public:
    InvalidAccountException(const std::string& accountId)
        : BankingException("Invalid account: " + accountId, 1002) {}
};

class TransactionLimitException : public BankingException {
public:
    TransactionLimitException(double amount, double limit)
        : BankingException("Transaction amount $" + std::to_string(amount) +
                          " exceeds limit $" + std::to_string(limit), 1003) {}
};

// Banking operations that can throw exceptions
void processWithdrawal(double amount, double balance, double dailyLimit, double dailyUsed) {
    if (amount > balance) {
        throw InsufficientFundsException(amount, balance);
    }
    if (amount + dailyUsed > dailyLimit) {
        throw TransactionLimitException(amount, dailyLimit - dailyUsed);
    }
    std::cout << "Withdrawal of $" << amount << " successful." << std::endl;
}

// Additional function for practice - transfer between accounts
void processTransfer(const std::string& fromAccount, const std::string& toAccount,
                    double amount, double fromBalance, double dailyLimit, double dailyUsed) {

    // Validate accounts
    if (fromAccount.empty()) {
        throw InvalidAccountException("Source account ID is empty");
    }
    if (toAccount.empty()) {
        throw InvalidAccountException("Destination account ID is empty");
    }
    if (fromAccount == toAccount) {
        throw InvalidAccountException("Cannot transfer to same account: " + fromAccount);
    }

    // Check funds and limits
    if (amount > fromBalance) {
        throw InsufficientFundsException(amount, fromBalance);
    }
    if (amount + dailyUsed > dailyLimit) {
        throw TransactionLimitException(amount, dailyLimit - dailyUsed);
    }

    std::cout << "Transfer of $" << amount << " from " << fromAccount
              << " to " << toAccount << " successful." << std::endl;
}

void testWithdrawalScenarios() {
    std::cout << "\n--- Testing Withdrawal Scenarios ---" << std::endl;

    // Test 1: Successful withdrawal
    try {
        std::cout << "Test 1: Normal withdrawal" << std::endl;
        processWithdrawal(200.0, 500.0, 1000.0, 100.0);
    } catch (const BankingException& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
    }

    // Test 2: Insufficient funds
    try {
        std::cout << "\nTest 2: Insufficient funds" << std::endl;
        processWithdrawal(600.0, 500.0, 1000.0, 100.0);
    } catch (const InsufficientFundsException& e) {
        std::cout << "Funds error: " << e.what() << " (Code: " << e.getErrorCode() << ")" << std::endl;
    } catch (const BankingException& e) {
        std::cout << "Unexpected banking error: " << e.what() << std::endl;
    }

    // Test 3: Transaction limit exceeded
    try {
        std::cout << "\nTest 3: Daily limit exceeded" << std::endl;
        processWithdrawal(400.0, 500.0, 1000.0, 700.0); // 400 + 700 = 1100 > 1000
    } catch (const TransactionLimitException& e) {
        std::cout << "Limit error: " << e.what() << " (Code: " << e.getErrorCode() << ")" << std::endl;
    } catch (const BankingException& e) {
        std::cout << "Unexpected banking error: " << e.what() << std::endl;
    }
}

void testTransferScenarios() {
    std::cout << "\n--- Testing Transfer Scenarios ---" << std::endl;

    // Test 1: Successful transfer
    try {
        std::cout << "Test 1: Normal transfer" << std::endl;
        processTransfer("ACC001", "ACC002", 250.0, 500.0, 1000.0, 200.0);
    } catch (const BankingException& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
    }

    // Test 2: Invalid account
    try {
        std::cout << "\nTest 2: Empty source account" << std::endl;
        processTransfer("", "ACC002", 100.0, 500.0, 1000.0, 200.0);
    } catch (const InvalidAccountException& e) {
        std::cout << "Account error: " << e.what() << " (Code: " << e.getErrorCode() << ")" << std::endl;
    } catch (const BankingException& e) {
        std::cout << "Unexpected banking error: " << e.what() << std::endl;
    }

    // Test 3: Same account transfer
    try {
        std::cout << "\nTest 3: Transfer to same account" << std::endl;
        processTransfer("ACC001", "ACC001", 100.0, 500.0, 1000.0, 200.0);
    } catch (const InvalidAccountException& e) {
        std::cout << "Account error: " << e.what() << " (Code: " << e.getErrorCode() << ")" << std::endl;
    } catch (const BankingException& e) {
        std::cout << "Unexpected banking error: " << e.what() << std::endl;
    }
}

void demonstrateCatchOrder() {
    std::cout << "\n--- Demonstrating Catch Block Order ---" << std::endl;

    try {
        // This will throw an InsufficientFundsException
        processWithdrawal(1000.0, 300.0, 2000.0, 500.0);

    } catch (const InsufficientFundsException& e) {
        std::cout << "Caught specific exception: " << e.what() << " (Code: " << e.getErrorCode() << ")" << std::endl;

    } catch (const TransactionLimitException& e) {
        std::cout << "Caught limit exception: " << e.what() << " (Code: " << e.getErrorCode() << ")" << std::endl;

    } catch (const InvalidAccountException& e) {
        std::cout << "Caught account exception: " << e.what() << " (Code: " << e.getErrorCode() << ")" << std::endl;

    } catch (const BankingException& e) {
        std::cout << "Caught general banking exception: " << e.what() << " (Code: " << e.getErrorCode() << ")" << std::endl;

    } catch (const std::exception& e) {
        std::cout << "Caught system exception: " << e.what() << std::endl;
    }

    std::cout << "Note: The most specific exception (InsufficientFundsException) was caught first." << std::endl;
}

int main() {
    std::cout << "=== Task 2: Exception Hierarchy Design ===" << std::endl;

    testWithdrawalScenarios();
    testTransferScenarios();
    demonstrateCatchOrder();

    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "✓ Exception hierarchy with base BankingException" << std::endl;
    std::cout << "✓ Specific exception types with meaningful error messages" << std::endl;
    std::cout << "✓ Error codes for programmatic handling" << std::endl;
    std::cout << "✓ Proper catch block ordering (specific to general)" << std::endl;

    return 0;
}
