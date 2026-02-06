class BankAccount{
private:
  int balance;
public:
  BankAccount(int bal){
    balance = 0;
    addBalance(bal);
  }

  int getBalance() const{
    return balance;
  }

  void addBalance(int bal){
    if(bal < 0) {
      std::cout << "cannot add negative balance " << std::endl; 
    }
    balance += bal;
  } 
}
