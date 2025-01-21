// Implementation of ViterbiCodec.
//
// Author: Min Xu <xukmin@gmail.com>
// Date: 01/30/2015

#include "viterbi.h"

#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <limits>
#include <span>
#include <format>
#include <string>
#include <utility>
#include <vector>
#include <cstdint>

namespace
{

  int HammingDistance(const std::string &x, const std::string &y)
  {
    assert(x.size() == y.size());
    int distance = 0;
    for (int i = 0; i < x.size(); i++)
    {
      distance += x[i] != y[i];
    }
    return distance;
  }

} // namespace

std::ostream &operator<<(std::ostream &os, const ViterbiCodec &codec)
{
  os << "ViterbiCodec(" << codec.constraint() << ", {";
  const std::array<std::uint8_t, 2> &polynomials = codec.polynomials();
  assert(!polynomials.empty());
  os << polynomials.front();
  for (int i = 1; i < polynomials.size(); i++)
  {
    os << ", " << polynomials[i];
  }
  return os << "})";
}

int ReverseBits(int num_bits, int input)
{
  assert(input < (1 << num_bits));
  int output = 0;
  while (num_bits-- > 0)
  {
    output = (output << 1) + (input & 1);
    input >>= 1;
  }
  return output;
}

ViterbiCodec::ViterbiCodec()
{
  assert(!polynomials_.empty());
  for (int i = 0; i < polynomials_.size(); i++)
  {
    assert(polynomials_[i] > 0);
    assert(polynomials_[i] < (1 << constraint_));
  }
  InitializeOutputs();
}

int ViterbiCodec::num_parity_bits() const
{
  return polynomials_.size();
}

int ViterbiCodec::NextState(int current_state, int input) const
{
  return (current_state >> 1) | (input << (constraint_ - 2));
}

std::uint8_t ViterbiCodec::Output(int current_state, int input) const
{
  return outputs_.at(current_state | (input << (constraint_ - 1)));
}

void ViterbiCodec::Encode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst) const
{
  auto state = 0;
  auto bytes = 0;
  auto i = 0;

  for (; i < src.size(); i++)
  {
    auto counter = 0;
    for (int j = CHAR_BIT - 1; j > 0; j = j - 2) // per iteration 12 bits.
    {
      auto bit1 = (src[i] >> j) & 1;
      auto bit2 = (src[i] >> (j - 1)) & 1;
      assert(bit1 == 0 || bit1 == 1);
      assert(bit2 == 0 || bit2 == 1);
      auto output1 = Output(state, bit1);
      state = NextState(state, bit1);
      auto output2 = Output(state, bit2);
      state = NextState(state, bit2);
      std::uint8_t output = (output1 << 1) | (output2 & 1);
      assert(output >= 0 && output <= 0b111);
      bytes = (bytes << 3) | output;
    }

    if (i % 2 == 1) // when i is odd, the total bits are 24, thus 3 bytes.
    {
      assert(bytes >= 0 && bytes <= 0xFFFFFF);
      int mask = 0xFF0000;
      int shift = 16;
      for (int k = 0; k < 3; k++)
      {
        dst.push_back((bytes & mask) >> shift);
        mask >>= 8;
        shift -= 8;
      }
      bytes = 0;
    }
  }

  for (int j = constraint_ - 1; j > 0; j = j - 2) {
    assert(constraint_ == 7);
    auto bit1 = 0;
    auto bit2 = 0;
    auto output1 = Output(state, bit1);
    state = NextState(state, bit1);
    auto output2 = Output(state, bit2);
    state = NextState(state, bit2);
    std::uint8_t output = (output1 << 1) | (output2 & 1);
    assert(output >= 0 && output <= 0b111);
    bytes = (bytes << 3) | output;
  }
  std::cout << "bits: " << std::to_string(bytes) << "\n";
  // bis hier mal richtig

  if (i % 2 == 1) { // 21 bits to process e.g.: 111101010001111011011
    assert(bytes >= 0 && bytes <= 0x1FFFFF);
    dst.push_back((bytes >> 13) & 0xFF); // 11110101
    dst.push_back((bytes >> 5) & 0xFF); //  00011110
    dst.push_back(bytes & 0x1F);  // 11011
  } else { // 9 bits to process
    assert(bytes >= 0 && bytes <= 0x1FF);
    dst.push_back((bytes >> 1) & 0xFF);
    dst.push_back(bytes & 1);
  }
}

void ViterbiCodec::InitializeOutputs()
{
  for (int i = 0; i < outputs_.size(); i++)
  {
    outputs_[i] = 0;
    for (int j = 0; j < num_parity_bits(); j++)
    {
      uint8_t polynomial = polynomials_[j];
      uint8_t input = i;
      uint8_t output = 0;
      for (int k = 0; k < constraint_; k++)
      {
        output ^= (input & 1) & (polynomial & 1);
        polynomial >>= 1;
        input >>= 1;
      }
      outputs_[i] = (outputs_[i] << 1) | output;
    }
  }
}

int ViterbiCodec::BranchMetric(const std::string &bits,
                               int source_state,
                               int target_state) const
{
  assert(bits.size() == num_parity_bits());
  assert((target_state & ((1 << (constraint_ - 2)) - 1)) == source_state >> 1);
  const auto output =
      Output(source_state, target_state >> (constraint_ - 2));

  return HammingDistance(bits, std::format("{:02b}", output));
}

std::pair<int, int> ViterbiCodec::PathMetric(
    const std::string &bits,
    const std::vector<int> &prev_path_metrics,
    int state) const
{
  int s = (state & ((1 << (constraint_ - 2)) - 1)) << 1;
  int source_state1 = s | 0;
  int source_state2 = s | 1;

  int pm1 = prev_path_metrics[source_state1];
  if (pm1 < std::numeric_limits<int>::max())
  {
    pm1 += BranchMetric(bits, source_state1, state);
  }
  int pm2 = prev_path_metrics[source_state2];
  if (pm2 < std::numeric_limits<int>::max())
  {
    pm2 += BranchMetric(bits, source_state2, state);
  }

  if (pm1 <= pm2)
  {
    return std::make_pair(pm1, source_state1);
  }
  else
  {
    return std::make_pair(pm2, source_state2);
  }
}

void ViterbiCodec::UpdatePathMetrics(const std::string &bits,
                                     std::vector<int> *path_metrics,
                                     Trellis *trellis) const
{
  std::vector<int> new_path_metrics(path_metrics->size());
  std::vector<int> new_trellis_column(1 << (constraint_ - 1));
  for (int i = 0; i < path_metrics->size(); i++)
  {
    std::pair<int, int> p = PathMetric(bits, *path_metrics, i);
    new_path_metrics[i] = p.first;
    new_trellis_column[i] = p.second;
  }

  *path_metrics = new_path_metrics;
  trellis->push_back(new_trellis_column);
}

std::string ViterbiCodec::Decode(const std::string &bits) const
{
  // Compute path metrics and generate trellis.
  Trellis trellis;
  std::vector<int> path_metrics(1 << (constraint_ - 1),
                                std::numeric_limits<int>::max());
  path_metrics.front() = 0;
  for (int i = 0; i < bits.size(); i += num_parity_bits())
  {
    std::string current_bits(bits, i, num_parity_bits());
    // If some bits are missing, fill with trailing zeros.
    // This is not ideal but it is the best we can do.
    if (current_bits.size() < num_parity_bits())
    {
      current_bits.append(
          std::string(num_parity_bits() - current_bits.size(), '0'));
    }
    UpdatePathMetrics(current_bits, &path_metrics, &trellis);
  }

  // Traceback.
  std::string decoded;
  int state = std::min_element(path_metrics.begin(), path_metrics.end()) -
              path_metrics.begin();
  for (int i = trellis.size() - 1; i >= 0; i--)
  {
    decoded += state >> (constraint_ - 2) ? "1" : "0";
    state = trellis[i][state];
  }
  std::reverse(decoded.begin(), decoded.end());

  // Remove (constraint_ - 1) flushing bits.
  return decoded.substr(0, decoded.size() - constraint_ + 1);
}
