#include "viterbi.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <format>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
  auto codec = ViterbiCodec();
  auto message = std::vector<std::uint8_t>{0b11111111, 1, 0b1111, 0b11111111, 0b11110000, 0b11111110, 0b11111111, 1}; // 111101010111000010101001101110100101010111111111111111000010010101010110010001100111000010101001101110011
  for (std::uint8_t x : message)
  {
    std::cout << std::format("{:08b}", x) << " ";
  }
  std::cout << "\n"
            << "size: " << message.size() << "\n";
  auto bits = std::array<std::uint8_t, 48>{};
  auto encoded = std::vector<std::uint8_t>();
  codec.Encode(message, encoded);
  auto encoded_string = std::string();
  for (auto i = 0; i < encoded.size(); i++)
  {
    auto buffer = std::string();
    auto x = 0;
    auto limit = 8;
    size_t size = message.size();
    while (x < limit)
    {
      buffer += std::to_string((encoded[i] >> x) & 1);
      x++;
    }
    std::reverse(buffer.begin(), buffer.end());
    encoded_string += buffer;
  }
  std::cout << "code: " << encoded_string << "\n";

  // auto decoded = codec.Decode(encoded_string);
  // std::cout << "decoded: ";
  // std::cout << decoded << "\n";

  return 0;
}