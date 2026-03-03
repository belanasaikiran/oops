#include <exception>
#include <iostream>
#include <string>

class BankingException : public std::exception {
protected:
  std::string errorMessage;
  int errorCode;

public:
  BankingException(const std::string &msg, int code = 0)
      : errorMessage(msg), errorCode(code) {}
  const char *what() const noexcept override { return errorMessage.c_str(); }
  int getErrorCode() const { return errorCode; }
};
// Specific exception types
class InsufficientFundsException : public BankingException {
public:
  InsufficientFundsException(double requested, double available)
      : BankingException("Insufficient funds: requested $" +
                             std::to_string(requested) + ", available $" +
                             std::to_string(available),
                         1001) {}
};
class InvalidAccountException : public BankingException {
public:
  InvalidAccountException(const std::string &accountId)
      : BankingException("Invalid account: " + accountId, 1002) {}
};
class TransactionLimitException : public BankingException {
public:
  TransactionLimitException(double amount, double limit)
      : BankingException("Transaction amount $" + std::to_string(amount) +
                             " exceeds limit $" + std::to_string(limit),
                         1003) {}
};
// Banking operations that can throw exceptions
void processWithdrawal(double amount, double balance, double dailyLimit,
                       double dailyUsed) {
  if (amount > balance) {
    throw InsufficientFundsException(amount, balance);
  }
  if (amount + dailyUsed > dailyLimit) {
    throw TransactionLimitException(amount, dailyLimit - dailyUsed);
  }
  std::cout << "Withdrawal of $" << amount << " successful." << std::endl;
}
int main() {
  try {
    // Test different exception scenarios
    processWithdrawal(500.0, 300.0, 1000.0,
                      200.0); // Should throw InsufficientFundsException
  } catch (const InsufficientFundsException &e) {
    std::cout << "Funds error: " << e.what() << " (Code: " << e.getErrorCode()
              << ")" << std::endl;
  } catch (const TransactionLimitException &e) {
    std::cout << "Limit error: " << e.what() << " (Code: " << e.getErrorCode()
              << ")" << std::endl;
  } catch (const BankingException &e) {
    std::cout << "Banking error: " << e.what() << " (Code: " << e.getErrorCode()
              << ")" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "System error: " << e.what() << std::endl;
  }
  return 0;
}
