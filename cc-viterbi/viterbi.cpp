// Implementation of ViterbiCodec.
//
// Author: Min Xu <xukmin@gmail.com>
// Date: 01/30/2015

#include "viterbi.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <format>
#include <limits>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace
{

  int HammingDistance(const uint8_t &x, const uint8_t &y)
  {
    int distance = 0;
    int x_copy = x;
    int y_copy = y;
    while (x_copy && y_copy)
    {
      distance += (x_copy & 1) != (y_copy & 1);
      x_copy >>= 1;
      y_copy >>= 1;
    }
    return distance;
  }

} // namespace

std::ostream &operator<<(std::ostream &os, const ViterbiCodec &codec)
{
  os << "ViterbiCodec(" << codec.constraint() << ", {";
  const std::array<int, 2> &polynomials = codec.polynomials();
  assert(!polynomials.empty());
  os << polynomials.front();
  for (int i = 1; i < polynomials.size(); i++)
  {
    os << ", " << polynomials[i];
  }
  return os << "})";
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

uint8_t ViterbiCodec::Output(int current_state, int input) const
{
  return outputs_.at(current_state | (input << (constraint_ - 1)));
}

std::vector<uint8_t> ViterbiCodec::Encode(std::span<uint8_t> src) const
{
  std::cout << "input: ";
  for (auto byte : src)
  {
    std::cout << std::format("{:08b} ", byte);
  }
  std::cout << std::endl;
  std::vector<uint8_t> dst;
  dst.reserve(src.size() + constraint_ - 1);
  int state = 0;
  // Encode the message bits.
  for (int i = 0; i < src.size(); i++)
  {
    int input = src[i];

    dst.push_back(Output(state, input));
    state = NextState(state, input);
  }
  std::cout << "code: ";
  for (int i = 0; i < dst.size(); i++)
  {
    std::cout << dst[i];
  }
  std::cout << std::endl;

  // Encode (constaint_ - 1) flushing bits.
  for (int i = 0; i < constraint_ - 1; i++)
  {
    dst.push_back(Output(state, 0));
    state = NextState(state, 0);
  }
  std::cout << "bits after flushing: ";
  for (int i = 0; i < dst.size(); i++)
  {
    std::cout << dst[i];
  }

  int puncturing_index = 0;
  std::vector<uint8_t> punctured_dst;
  punctured_dst.reserve(dst.size());

  for (int i = 0; i < dst.size(); i++)
  {
    if (puncturing_pattern_[puncturing_index % 4])
    {
      punctured_dst.push_back(dst[i]);
    }
    puncturing_index++;
  }
  // std::cout << "after puncturing: " << dst << std::endl;
  return dst;
}

void ViterbiCodec::InitializeOutputs()
{
  for (int i = 0; i < outputs_.size(); i++)
  {
    for (int j = 0; j < num_parity_bits(); j++)
    {
      // Reverse polynomial bits to make the convolution code simpler.
      // int polynomial = ReverseBits(constraint_, polynomials_[j]);
      int polynomial = polynomials_[j];
      int input = i;
      int output = 0;
      for (int k = 0; k < constraint_; k++)
      {
        output ^= (input & 1) & (polynomial & 1);
        polynomial >>= 1;
        input >>= 1;
      }
      outputs_[i] += output ? 1 : 0;
    }
  }
}

int ViterbiCodec::BranchMetric(const std::string &bits,
                               int source_state,
                               int target_state) const
{
  assert(bits.size() == num_parity_bits());
  // This checks the validity of the state transition. The new state should be consistent
  // with the shift register behavior of the convolutional encoder. Essentially, this means
  // that the lower bits of the target_state should match the upper bits of the source_state
  // after being shifted right by one position.
  assert((target_state & ((1 << (constraint_ - 2)) - 1)) == source_state >> 1);
  const uint8_t output =
      Output(source_state, target_state >> (constraint_ - 2));

  int bit_sequence = 0;
  for (int i = 0; i < bits.size(); i++)
  {
    bit_sequence <<= 1;
    bit_sequence |= (bits[i] - '0');
  }

  return HammingDistance(bit_sequence, output); // how well does the expected output from that transition match the actual received bits?
}

std::pair<int, int> ViterbiCodec::PathMetric(
    const std::string &bits,
    const std::array<int, 64> &prev_path_metrics,
    int state) const
{
  int s = (state & ((1 << (constraint_ - 2)) - 1)) << 1; // applies the mask to the state to keep only the lower bits. constraint_ - 2 bits remain unchanged when a new bit comes in.
  int source_state1 = s | 0;
  int source_state2 = s | 1; // these source states represent the 2 possible prev. states, before the right-most bit was pushed out of the register.

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
                                     std::array<int, 64> *path_metrics,
                                     Trellis *trellis) const
{
  std::array<int, 64> new_path_metrics;
  std::array<int, 64> new_trellis_column;
  for (int i = 0; i < path_metrics->size(); i++)
  {
    std::pair<int, int> p = PathMetric(bits, *path_metrics, i);
    new_path_metrics[i] = p.first;
    new_trellis_column[i] = p.second;
  }

  // *path_metrics = new_path_metrics;
  for (int i = 0; i < path_metrics->size(); i++)
  {
    (*path_metrics)[i] = new_path_metrics[i];
  }
  trellis->push_back(new_trellis_column);
}

std::string ViterbiCodec::Decode(const std::string &bits) const
{
  int puncture_index = 0;
  std::string reconstructed_bits;
  int pattern[] = {0, 1, 0};
  int pattern_index = 0;

  for (int i = 0; i < bits.size(); i++)
  {
    if (puncturing_pattern_[puncture_index % 4])
    {
      reconstructed_bits += bits[i]; // Use the bit from the input
    }
    else
    {
      reconstructed_bits += pattern[pattern_index++ % 3];
      i--;
    }
    puncture_index++;
  }
  // Compute path metrics and generate trellis.
  Trellis trellis;

  std::array<int, 64> path_metrics; // path_matrics[i] represents the cost to reach the state i, thus the length is the number of possible states.
  std::fill(path_metrics.begin(), path_metrics.end(), std::numeric_limits<int>::max());
  path_metrics.front() = 0;

  for (int i = 0; i < reconstructed_bits.size(); i += num_parity_bits())
  {
    std::string current_bits(reconstructed_bits, i, num_parity_bits());
    // If some bits are missing, fill with trailing zeros.
    // This is not ideal but it is the best we can do.
    if (current_bits.size() < num_parity_bits())
    {
      current_bits.append(std::string(num_parity_bits() - current_bits.size(), '0'));
    }

    UpdatePathMetrics(current_bits, &path_metrics, &trellis);
  }

  // Traceback.
  std::string decoded;
  int state = std::min_element(path_metrics.begin(), path_metrics.end()) -
              path_metrics.begin();
  std::cout << "trellis size: " << trellis.size() << std::endl;
  for (int i = trellis.size() - 1; i >= 0; i--)
  {
    decoded += state >> (constraint_ - 2) ? "1" : "0";
    state = trellis[i][state];
  }
  std::reverse(decoded.begin(), decoded.end());

  std::cout << "decoded: " << decoded << std::endl;

  // Remove (constraint_ - 1) flushing bits.
  return decoded.substr(0, decoded.size() - constraint_ + 1);
}
