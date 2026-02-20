#include <iostream>
#include "../include/rm_a0/a0_01_temperature.hpp"

int main() {
  double celsius = 0.0;
  if (!(std::cin >> celsius)) {
    return 0;
  }
  std::cout << rm_a0::FormatFahrenheit(rm_a0::CelsiusToFahrenheit(celsius));
  return 0;
}
/*
double CelsiusToFahrenheit(double celsius) {
    return celsius * 9.0 / 5.0 + 32.0;
}

std::string FormatFahrenheit(double fahrenheit) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%.2f\n", fahrenheit);
    return std::string(buffer);
}
*/