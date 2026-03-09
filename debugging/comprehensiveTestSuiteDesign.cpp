#define CATCH_CONFIG_MAIN
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <vector>
class FinancialCalculator {
private:
  double interestRate;
  int calculationCount;

public:
  FinancialCalculator(double rate) : calculationCount(0) {
    if (rate < 0) {
      throw std::invalid_argument("Interest rate cannot be negative");
    }
    interestRate = rate;
  }
  // Fixed version of compound interest calculation
  double calculateCompoundInterest(double principal, int years) {
    if (principal < 0) {
      throw std::invalid_argument("Principal cannot be negative");
    }
    if (years < 0) {
      throw std::invalid_argument("Years cannot be negative");
    }
    calculationCount++;
    double result = principal;
    // Fixed: correct loop condition
    for (int i = 0; i < years; i++) {
      result = result * (1 + interestRate);
    }
    return result - principal;
  }

  // Fixed version of portfolio calculation
  double calculatePortfolioValue(const std::vector<double> &investments) {
    if (investments.empty()) {
      return 0.0;
    }
    calculationCount++;
    double total = 0.0;
    // Fixed: correct loop bounds
    for (size_t i = 0; i < investments.size(); i++) {
      if (investments[i] < 0) {
        throw std::invalid_argument("Investment values cannot be negative");
      }
      total += investments[i];
    }
    return total;
  }
  double getInterestRate() const { return interestRate; }
  int getCalculationCount() const { return calculationCount; }
  void resetCalculationCount() { calculationCount = 0; }
};

// Comprehensive Test Cases

TEST_CASE("FinancialCalculator Construction", "[constructor]") {
  SECTION("Valid interest rate") {
    FinancialCalculator calc(0.05);
    REQUIRE(calc.getInterestRate() == 0.05);
    REQUIRE(calc.getCalculationCount() == 0);
  }
  SECTION("Zero interest rate") {
    FinancialCalculator calc(0.0);
    REQUIRE(calc.getInterestRate() == 0);
  }
  SECTION("Negative interest rate throws exception") {
    REQUIRE_THROWS_AS(FinancialCalculator(-0.01), std::invalid_argument);
  }
}

TEST_CASE("Compound Interest Calculations", "[compound_interest]") {
  FinancialCalculator calc(0.05);
  SECTION("Basic compound interest calculation") {
    double interest = calc.calculateCompoundInterest(1000.0, 3);
    REQUIRE(interest == Catch::Approx(157.625).epsilon(0.001));
  }
  SECTION("Zero years returns zero interest") {
    double interest = calc.calculateCompoundInterest(1000.0, 0);
    REQUIRE(interest == 0.0);
  }
  SECTION("One year calculation") {
    double interest = calc.calculateCompoundInterest(0.0, 5);
    REQUIRE(interest == 0.0);
  }
  SECTION("Negative principal throws exception") {
    REQUIRE_THROWS_AS(calc.calculateCompoundInterest(-1000.0, 3),
                      std::invalid_argument);
  }
  SECTION("Negative years throws exception") {
    REQUIRE_THROWS_AS(calc.calculateCompoundInterest(1000.0, -1),
                      std::invalid_argument);
  }
}

TEST_CASE("Portfolio Value Calculations", "[portfolio]") {
  FinancialCalculator calc(0.05);
  SECTION("Standard portfolio calculation") {
    std::vector<double> portfolio = {1500.0, 2300.0, 980.0, 1200.0};
    double total = calc.calculatePortfolioValue(portfolio);
    REQUIRE(total == 5980.0);
  }
  SECTION("Single investment portfolio") {
    std::vector<double> portfolio = {2500.0};
    double total = calc.calculatePortfolioValue(portfolio);
    REQUIRE(total == 2500.0);
  }
  SECTION("Empty portfolio returns zero") {
    std::vector<double> portfolio = {};
    double total = calc.calculatePortfolioValue(portfolio);
    REQUIRE(total == 0.0);
  }
  SECTION("Portfolio with zero values") {
    std::vector<double> portfolio = {1000.0, 0.0, 500.0};
    double total = calc.calculatePortfolioValue(portfolio);
    REQUIRE(total == 1500.0);
  }
  SECTION("Negative investment throws exception") {
    std::vector<double> portfolio = {1000.0, -500.0, 800.0};
    REQUIRE_THROWS_AS(calc.calculatePortfolioValue(portfolio),
                      std::invalid_argument);
  }
}

TEST_CASE("Calculation Count Tracking", "[statistics]") {
  FinancialCalculator calc(0.03);
  SECTION("Count increments with each calculation") {
    REQUIRE(calc.getCalculationCount() == 0);
    calc.calculateCompoundInterest(1000.0, 2);
    REQUIRE(calc.getCalculationCount() == 1);
    std::vector<double> portfolio = {1000.0, 2000.0};
    calc.calculatePortfolioValue(portfolio);
    REQUIRE(calc.getCalculationCount() == 2);
  }
  SECTION("Count can be reset") {
    calc.calculateCompoundInterest(1000.0, 1);
    calc.calculateCompoundInterest(2000.0, 2);
    REQUIRE(calc.getCalculationCount() == 2);
    calc.resetCalculationCount();
    REQUIRE(calc.getCalculationCount() == 0);
  }
}

// Key testing patterns demonstrated:
TEST_CASE("Descriptive test name", "[tag]") {
  SECTION("Specific scenario") {
    // Test setup
    FinancialCalculator calc(0.05);

    // Test execution
    double result = calc.calculateCompoundInterest(1000.0, 3);

    // Test verification with appropriate tolerance
    REQUIRE(result == Catch::Approx(157.625).epsilon(0.001));
  }
}
