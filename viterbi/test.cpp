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
  auto codec = ViterbiCodec();
  auto message = std::vector<std::uint8_t>{0b10000001};
  std::cout << "input:   " << std::format("{:08b}", message[0]) << "\n";
  auto bits = std::array<std::uint8_t, 48>{};

  auto encoded = std::vector<std::uint8_t>();
  codec.Encode(message, encoded);
  auto encoded_string = std::string();
  std::cout << "encoded length: " << encoded.size() << "\n";
  for (auto x : encoded) {
    encoded_string += std::format("{:08b}", x);
  }
  std::cout << encoded_string << "\n";

  auto decoded = codec.Decode(encoded_string);
  std::cout << "decoded: ";
  std::cout << decoded << "\n";

  return 0;
}