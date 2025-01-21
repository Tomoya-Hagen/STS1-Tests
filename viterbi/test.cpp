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
  auto message = std::vector<std::uint8_t>{0b11111111, 1, 0b1111, 0b11111111}; // encoded: 111101010111000010101 (for 0xFF), 111101010001111011011 (for 0b11111101)
  for (std::uint8_t x : message) {
    std::cout << std::format("{:08b}", x) << " ";
  }
  std::cout << "\n" << "size: " << message.size() << "\n";
  auto bits = std::array<std::uint8_t, 48>{};
  auto encoded = std::vector<std::uint8_t>();
  codec.Encode(message, encoded);
  auto encoded_string = std::string();
  auto counter = 0;
  for (uint8_t i = 0; i < encoded.size(); i++) {
    auto buffer = std::string();
    int x = 0;
    uint8_t limit;
    if (i == encoded.size() - 1) {
      if (i % 2 == 0) {
        limit = 5;
      } else {
        limit = 1;
      }
    } else {
      limit = 8;
    }
    while (x < limit) {
      buffer += std::to_string((encoded[i] >> x) & 1);
      x++;
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