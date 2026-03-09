#include <iostream>
#include <string>
#include <vector>

class FinancialCalculator {
private:
  double interestRate;
  int calculationCount;

public:
  FinancialCalculator(double rate) : interestRate(rate), calculationCount(0) {}
  // Bug: Compound interest calculation has logic error
  double calculateCompoundInterest(double principal, int years) {
    calculationCount++;
    double result = principal;
    // Intentional bug: off-by-one error in loop
    for (int i = 0; i <= years; i++) {
      result = result * (1 + interestRate);
    }
    return result - principal; // Return only the interest earned
  }
  // Bug: Memory access issue with portfolio calculations
  double calculatePortfolioValue(const std::vector<double> &investments) {
    calculationCount++;
    double total = 0.0;
    // Intentional bug: potential out-of-bounds access
    for (size_t i = 0; i < investments.size(); i++) {
      total += investments[i];
    }
    return total;
  }
  void displayStats() const {
    std::cout << "Calculations performed: " << calculationCount << std::endl;
    std::cout << "Interest rate: " << interestRate * 100 << "%" << std::endl;
  }
};
int main() {
  FinancialCalculator calculator(0.05); // 5% interest rate
  // Test compound interest calculation
  std::cout << "Testing compound interest calculation:" << std::endl;
  double interest = calculator.calculateCompoundInterest(1000.0, 3);
  std::cout << "Interest earned: $" << interest << std::endl;
  // Test portfolio calculation
  std::cout << "\nTesting portfolio calculation:" << std::endl;
  std::vector<double> portfolio = {1500.0, 2300.0, 980.0, 1200.0};
  double totalValue = calculator.calculatePortfolioValue(portfolio);
  std::cout << "Total portfolio value: $" << totalValue << std::endl;
  calculator.displayStats();
  return 0;
}
