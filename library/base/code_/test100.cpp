#include <math.h>
#include <cstdint>
#include <iostream>
#include "dainty_base_traits.h"

using namespace dainty::base::traits;

struct t_float {
  float         value;
  std::uint32_t bits;
  bool          sign;
  std::uint32_t exponent;
  std::uint32_t mantissa;
};

struct t_double {
  double        value;
  std::uint64_t bits;
  bool          sign;
  std::uint64_t exponent;
  std::uint64_t mantissa;
};

#define FLOAT_SIZEOF           (int)(sizeof(float)*8)
#define FLOAT_SIGN(value)      ((value & (1U << (FLOAT_SIZEOF - 1))) == true)
#define FLOAT_EXPONENT(value)  ((0xff & (value >> 23)))
#define FLOAT_MANTISSA(value)  ((0x7fffff & value))

#define DOUBLE_SIZEOF          (int)(sizeof(double)*8)
#define DOUBLE_SIGN(value)     ((value & (1UL << (DOUBLE_SIZEOF - 1))) == true)
#define DOUBLE_EXPONENT(value) (0x7ffUL & (value >> 52))
#define DOUBLE_MANTISSA(value) (0xfffffffffffffUL & value)

template<typename T, t_if<t_is_same<float, T>> = YES>
constexpr
t_float representation(T value) noexcept {
  std::uint32_t bits = *((std::uint32_t*)(&value));
  return { value,
           bits,
           FLOAT_SIGN(bits),
           FLOAT_EXPONENT(bits),
           FLOAT_MANTISSA(bits) };
}

bool check_value(t_float value) noexcept {
  float tmp = 0;
  if (!value.mantissa && !value.exponent)
    tmp = value.sign ? 0 : -0;
  else
    tmp = ((1 + (value.mantissa * pow(2, -23))) * pow(2, (int)value.exponent - 127));

  if (tmp != value.value) {
    std::cout << "float: " << value.value << " != " << tmp << std::endl;
    return false;
  }
  return true;
}

template<typename T, t_if<t_is_same<double, T>> = YES>
constexpr
t_double representation(T value) noexcept {
  std::uint64_t bits = *((std::uint64_t*)(&value));
  return { value,
           bits,
           DOUBLE_SIGN(bits),
           DOUBLE_EXPONENT(bits),
           DOUBLE_MANTISSA(bits) };
}

bool check_value(t_double value) noexcept {
  double tmp = 0;
  if (!value.mantissa && !value.exponent)
    tmp = value.sign ? 0 : -0;
  else
    tmp = ((1 + (value.mantissa * pow(2, -52))) * pow(2, (int)value.exponent - 1023));

  if (tmp != value.value) {
    std::cout << "double: " << value.value << " != " << tmp << std::endl;
    return false;
  }
  return true;
}

t_float print_bits(const t_float& f) noexcept {
  std::cout << "float: " << f.value << " bits: ";
  for (int i = 0; i < FLOAT_SIZEOF; ++i) {
    bool on = (1 << (FLOAT_SIZEOF - 1 - i)) & f.bits;
    std::cout << (on ? "1" : "0");
  }

  std::cout << ", expo: ";
  for (int i = 0; i < 8; ++i) {
    bool on = (1 << (8 - 1 - i)) & f.exponent;
    std::cout << (on ? "1" : "0");
  }

  std::cout << ", significand: ";
  for (int i = 0; i < 23; ++i) {
    bool on = (1 << (23 - 1 - i)) & f.mantissa;
    std::cout << (on ? "1" : "0");
  }

  std::cout << std::endl;
  return f;
}

t_double print_bits(const t_double& d) noexcept {
  std::cout << "double: " << d.value << " bits: ";
  for (int i = 0; i < DOUBLE_SIZEOF; ++i) {
    bool on = (1UL << (DOUBLE_SIZEOF - 1 - i)) & d.bits;
    std::cout << (on ? "1" : "0");
  }

  std::cout << ", expo: ";
  for (int i = 0; i < 11; ++i) {
    bool on = (1UL << (11 - 1 - i)) & d.exponent;
    std::cout << (on ? "1" : "0");
  }

  std::cout << ", significand: ";
  for (int i = 0; i < 52; ++i) {
    bool on = (1UL << (52 - 1 - i)) & d.mantissa;
    std::cout << (on ? "1" : "0");
  }

  std::cout << std::endl;
  return d;
}

int main() {

  check_value(print_bits(representation(1.1f)));
  check_value(print_bits(representation(0.11f)));
  check_value(print_bits(representation(0.011f)));
  check_value(print_bits(representation(0.0011f)));
  check_value(print_bits(representation(-0.0f)));
  check_value(print_bits(representation(0.0f)));
  check_value(print_bits(representation(1.5e38f)));
  check_value(print_bits(representation(1.5e38f + 1.1f)));

  check_value(print_bits(representation(1.1)));
  check_value(print_bits(representation(0.11)));
  check_value(print_bits(representation(0.011)));
  check_value(print_bits(representation(0.0011)));
  check_value(print_bits(representation(-0.0)));
  check_value(print_bits(representation(0.0)));
  check_value(print_bits(representation(1.5e38)));
  check_value(print_bits(representation(1.5e38 + 1.1)));

  return 0;
}
