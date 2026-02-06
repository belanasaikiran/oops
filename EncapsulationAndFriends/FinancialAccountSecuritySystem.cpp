#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// VULNERABLE CLASS - Current implementation with security Flaws

class InSecureAccount{
public:
  std::string accountNumber;
  std::string ownerName;
  double balance;
  std::string accountType;
  bool isActive;
  void displayAccount(){
    std::cout << "Account: " << accountNumber << " | Owner: " << ownerName << " | Balance: $" << std::fixed << std::setprecision(2)  << balance << std::endl;
  }
};

// SECURE REFACTORED CLASS - Good implementation

class SecureAccount{
private:
  std::string accountNumber;
  std::string ownerName;
  double balance;
  std::string accountType;
  bool isActive;
  std::vector<std::string> transactionHistory;

  // private helper to log transactions
  void logTransaction(const std::string& transaction){
    transactionHistory.push_back(transaction);
    if(transactionHistory.size() > 50) {
      transactionHistory.erase(transactionHistory.begin());
    }
  }
public:
  // Constructor with validation
  SecureAccount(const std::string& accNum, const std::string& owner, const std::string& type, double initialBalance = 0.0) : accountNumber(accNum), ownerName(owner), accountType(type), balance(initialBalance), isActive(true) {
    if(accNum.empty() || owner.empty()){
      throw std::invalid_argument("Account number and owner name cannot be empty");
    }
    if(initialBalance < 0) {
      balance = 0.0;
      std::cout << "Warning: Negative initial balance set to $0.00\n";
    }
    logTransaction("Account created with initial balance: $" + std::to_string(initialBalance));
  }

  // Getter with appropriate access levels
  std::string  getAccountNumber()  const { return accountNumber; }
  std::string getOwnerName() const { return ownerName; }
  double getBalance() const { return balance; }
  std::string getAccountType() const { return accountType; }
  bool getActiveStatus() const { return isActive; }

  // secured transaction methods with validation
  bool deposit(double amount){
    if(!isActive){
      std::cout << "Error: Cannot deposit to Inactive Account "<< std::endl;
      return false;
    }
    if(amount <= 0){
      std::cout << "Error: Deposit amount must be positive" << std::endl;
      return false;
    }
    if(amount > 50000) {
      std::cout <<"Error: Depost exceeds daily limit of $50,000" << std::endl;
      return false;
    }
    balance += amount;
    logTransaction("Deposit: +$" + std::to_string(amount));
    std::cout << "Successfully deposited $" << std::fixed << std::setprecision(2) << amount << ". New balance: $" << balance << std::endl;
    return true;
  }

  bool withdraw(double amount){
    if(!isActive){
      std::cout << "Error: Cannot withdraw from Inactive Account "<< std::endl;
      return false;
    }
    if(amount <= 0){
      std::cout << "Error: withdraw amount must be positive" << std::endl;
      return false;
    }
    if(amount > balance) {
      std::cout << "Error: Insufficient funds. Current Balance: $"
        << std::fixed << std::setprecision(2) << balance << std::endl;
      return false;
    }

    if(amount > 10000){
      std::cout << "Error: Withdrawl exceeds daily limit of $10,000" << std::endl;
      return false;
    }
    balance -= amount;
    logTransaction("Withdrawl: -$" + std::to_string(amount));
    std::cout << "Successfully withdrew $" << std::fixed <<  std::setprecision(2) << amount << ". New Balance: $" << balance << std::endl;
    return true;
  }

  // Account Management methods
  void setAccountStatus(bool status){
    isActive = status;
    logTransaction(status ? "Account activated" : "Account Deactivated");
    std::cout << "Account " << (status ? "activated" : "deactivated") << std::endl;
  }

  void displayAccount() const{
    std::cout << "=== Account Information ===" << std::endl;
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Owner: " << ownerName << std::endl;
    std::cout << "Type: " << accountType << std::endl;
    std::cout << "Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    std::cout << "Status: " << (isActive  ? "active" : "inactive") << std::endl;
    std::cout << "Recent Transactions: " << transactionHistory.size() << std::endl;
  }

  // Friend Function for regulatory compliance and auditing
  friend class ComplianceAuditor;
  friend void emergencyFreeze(SecureAccount &account,
                              const std::string &reason);

  friend void unfreeze(SecureAccount & account, const std::string& reason);

};



// Friend function for emergency account freezing (compliance requirement)
void emergencyFreeze(SecureAccount& account, const std::string& reason){
  account.isActive = false;
  account.logTransaction("EMERGENCY FREEZE: " + reason);
  std::cout << "EMERGENCY: Account " << account.accountNumber << " frozen due to " << reason << std::endl;
}

void unfreeze(SecureAccount &account, const std::string &reason) {
  std::cout << "unfreezing account due to: " << reason << std::endl;
  account.isActive = true;
}


// Friend Class for compliance auditing
class ComplianceAuditor{
public:
  static void performAudit(const SecureAccount& account){
    std::cout << "\n=== COMPLICANCE AUDIT === " << std::endl;
    std::cout << "Account: " << account.accountNumber << std::endl;
    std::cout << "Balance: " << std::fixed << std::setprecision(2)  << account.balance << std::endl;
    std::cout << "Transaction History Count: " << account.transactionHistory.size() << std::endl;
    int displayCount = std::min(5, static_cast<int> (account.transactionHistory.size()));
    for(int i = account.transactionHistory.size() - displayCount; i < account.transactionHistory.size(); ++i){
    std::cout << " - " << account.transactionHistory[i] << std::endl;
    }
    std::cout << "Audit completed Successfully. " << std::endl;
  }
};


int main(){
  std::cout << "=== Financial Account Security System Demo ===" << std::endl;
  // Demonstrate vulnerabilities of the insecure version

  std::cout << "\n1. INSECURE VERSION - vulnerabilities: " << std::endl;
  InSecureAccount insecure;
  insecure.accountNumber = "ACC001";
  insecure.ownerName = "John Doe";
  insecure.balance = 1000.0;

  // Anyone can directly manipulate the balance!
  insecure.balance = 999999.99; // unauthorized
  insecure.balance = -50000.0; // invalid
  std::cout << "Insecure account manipulated - Balnace now: $" << std::fixed << std::setprecision(2) << insecure.balance << std::endl;

  // Demonstrate the secured version
  std::cout << "\n2. SECURE VERSION - Protected Operations: " << std::endl;

  try{
    SecureAccount secure("ACC002", "Jane Smith", "Checking", 1500.0);
    secure.displayAccount();

    // test secure Operations
    std::cout << "\nTesting deposit operations: " << std::endl;
    secure.deposit(500.0);
    secure.deposit(-100.0); // reject
    secure.deposit(60000.0); // reject (over limit)
    std::cout << "\nTesting withdrawal operations: " << std::endl;
    secure.withdraw(1200.0);
    secure.withdraw(5000.0);
    secure.withdraw(50000.0);

  std::cout << "\n3. Emergency compliance action: "<< std::endl;
    emergencyFreeze(secure, "Suspicious activity detected");

  std::cout << "\nTrying operations on frozen account: " << std::endl;
    secure.deposit(100.0);
    ComplianceAuditor::performAudit(secure);

    unfreeze(secure, "no issue found!");


  } catch (const std::exception& e){
  std::cout << "Error creating account: " << e.what() << std::endl;
  }

  return 0;

}
