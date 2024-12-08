// Viterbi Codec.
//
// Author: Min Xu <xukmin@gmail.com>
// Date: 01/30/2015

#ifndef VITERBI_H_
#define VITERBI_H_

#include <ostream>
#include <string>
#include <utility>
#include <vector>

// This class implements both a Viterbi Decoder and a Convolutional Encoder.
class ViterbiCodec
{
public:
  // Note about Polynomial Descriptor of a Convolutional Encoder / Decoder.
  // A generator polymonial is built as follows: Build a binary number
  // representation by placing a 1 in each spot where a connection line from
  // the shift register feeds into the adder, and a zero elsewhere. There are 2
  // ways to arrange the bits:
  // 1. msb-current
  //    The MSB of the polynomial corresponds to the current input, while the
  //    LSB corresponds to the oldest input that still remains in the shift
  //    register.
  //    This representation is used by MATLAB. See
  //    http://radio.feld.cvut.cz/matlab/toolbox/comm/tutor124.html
  // 2. lsb-current
  //    The LSB of the polynomial corresponds to the current input, while the
  //    MSB corresponds to the oldest input that still remains in the shift
  //    register.
  //    This representation is used by the Spiral Viterbi Decoder Software
  //    Generator. See http://www.spiral.net/software/viterbi.html
  // We use 2.
  ViterbiCodec();

  std::string Encode(const std::string &bits) const;

  std::string Decode(const std::string &bits) const;

  int constraint() const { return 7; }

  const std::array<int, 2> &polynomials() const { return polynomials_; }

private:
  // Suppose
  //
  //     Trellis trellis;
  //
  // Then trellis[i][s] is the state in the (i - 1)th iteration which leads to
  // the current state s in the ith iteration.
  // It is used for traceback.
  typedef std::vector<std::array<int, 64>> Trellis;

  int num_parity_bits() const;

  void InitializeOutputs();

  int NextState(int current_state, int input) const;

  std::string Output(int current_state, int input) const;

  int BranchMetric(const std::string &bits,
                   int source_state,
                   int target_state) const;

  // Given num_parity_bits() received bits, compute and returns path
  // metric and its corresponding previous state.
  std::pair<int, int> PathMetric(const std::string &bits,
                                 const std::array<int, 64> &prev_path_metrics,
                                 int state) const;

  // Given num_parity_bits() received bits, update path metrics of all states
  // in the current iteration, and append new traceback vector to trellis.
  void UpdatePathMetrics(const std::string &bits,
                         std::array<int, 64> *path_metrics,
                         Trellis *trellis) const;

  const int constraint_ = 7;
  const std::array<int, 2> polynomials_ = {0b1111001, 0b1011011};

  // The output table.
  // The index is current input bit combined with previous inputs in the shift
  // register. The value is the output parity bits in string format for
  // convenience, e.g. "10". For example, suppose the shift register contains
  // 0b10 (= 2), and the current input is 0b1 (= 1), then the index is 0b110 (=
  // 6).
  std::array<std::string, 1 << 7> outputs_;

  const std::array<bool, 4> puncturing_pattern_ = {1, 1, 0, 1};
};

std::ostream &operator<<(std::ostream &os, const ViterbiCodec &codec);


#endif // VITERBI_H_