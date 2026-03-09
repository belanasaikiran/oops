#include <catch2/catch_test_macros.hpp>
int totalSum(std::vector<int> nums) {
  int sum = 0;
  for (int i = 0; i <= nums.size(); i++) {
    sum += nums[i];
  }
  return sum;
}
TEST_CASE("Array Sum", "[SUM OF ALL ELEMENTS]") {
  SECTION("CHECK SUM") {
    std::vector<int> numbers = {10, 20, 30};
    REQUIRE(totalSum(numbers) == 60);
  }
  SECTION("EMPTY ARRAY") {
      REQUIRE(totalSum({}) == 0);
  }
}


// Buggy Code: Accessing elements out of bounds.

// 7456
// int totalSum(std::vector<int> nums) {
//   int sum = 0;
//   for (int i = 0; i <= nums.size(); i++) {
//     sum += nums[i];
//   }
//   return sum;
// }
// To debug, I would add a breakpoint at line 3 (for loop) and check what is happening with array out of bounds and I would inspect the error at runtime.

// The below is the corrected version along with test cases using Catch2 framework

// 9101112131415161718
// #include <catch2/catch_test_macros.hpp>
// int totalSum(std::vector<int> nums) {
//   int sum = 0;
//   for (int i = 0; i <= nums.size(); i++) {
//     sum += nums[i];
//   }
//   return sum;
// }
// TEST_CASE("Array Sum", "[SUM OF ALL ELEMENTS]") {
//   SECTION("CHECK SUM") {

// Testable code ensures that add new code doesn't break the existing dependencies. It is best add test cases for the logic you write to ensure any new logic additions  doesn't brick the code.
