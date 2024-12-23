#include "viterbi.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <format>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  auto codec = ViterbiCodec(7, {0b1111001, 0b1011011});
  for (auto x : codec.outputs_) {
    std::cout << std::format("{:02b}", x) << " ";
  }
  std::cout << "\n";

  auto message = std::vector<std::uint8_t>{1};
  auto bits = std::array<std::uint8_t, 48>{};

  auto encoded = codec.Encode(message);
  auto encoded_string = std::string();
  for (auto x : encoded) {
    encoded_string += std::format("{:02b}", x);
  }
  std::cout << encoded_string << "\n";

  auto decoded = codec.Decode(encoded_string);
  std::cout << decoded << "\n";

  return 0;
}