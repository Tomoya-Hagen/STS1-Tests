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
  auto message = std::vector<std::uint8_t>{0b11111111};
  for (std::uint8_t x : message) {
    std::cout << std::format("{:08b}", x) << " ";
  }
  std::cout << "\n" << "size: " << message.size() << "\n";
  auto bits = std::array<std::uint8_t, 48>{};
  auto encoded = std::vector<std::uint8_t>();
  codec.Encode(message, encoded);
  auto encoded_string = std::string();
  auto counter = 0;
  for (auto x : encoded) {
    auto buffer = std::string();
    while (x) {
      buffer += std::to_string(x & 1);
      x >>= 1;
    }
    std::reverse(buffer.begin(), buffer.end());
    encoded_string += buffer;
    counter++;
  }
  std::cout << encoded_string << "\n" << "size: " << counter << "\n";

  // auto decoded = codec.Decode(encoded_string);
  // std::cout << "decoded: ";
  // std::cout << decoded << "\n";

  return 0;
}