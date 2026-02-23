#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace rm_a0 {

class BigInt {
public:
  BigInt(){ digits_.push_back(0);}

  // Constructs from a non-negative decimal string.
  explicit BigInt(const std::string &s){
        for (int i = s.size() - 1; i >= 0; i--) {
            digits_.push_back(s[i] - '0');
        }
  }

  friend BigInt operator+(const BigInt &a, const BigInt &b){
    BigInt result;
    result.digits_.clear();
    int carry = 0;
    size_t max_size = std::max(a.digits_.size(), b.digits_.size());
    for (size_t i = 0; i < max_size || carry; i++) {
        int digit_a = i < a.digits_.size() ? a.digits_[i] : 0;
        int digit_b = i < b.digits_.size() ? b.digits_[i] : 0;
        int sum = digit_a + digit_b + carry;
        result.digits_.push_back(sum % 10);
        carry = sum / 10;
    }
    return result;
  }
  friend std::ostream &operator<<(std::ostream &os, const BigInt &x){
        for (int i = x.digits_.size() - 1; i >= 0; i--) {
            os << x.digits_[i];
        }
        return os;
  }

private:
  // Little-endian digits, each 0..9.
  std::vector<int> digits_;
};


// Reads two lines A and B, returns (A+B)+"\n".
// If parsing fails, ok=false and returns empty string.
std::string SolveBigIntAdd(const std::string &input, bool &ok);

} // namespace rm_a0
