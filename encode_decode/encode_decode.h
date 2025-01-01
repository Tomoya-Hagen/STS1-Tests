#pragma once

#include "../bit_scrambling/Cpp/bitsnarl.hpp"
#include "../reed_solomon/include/rs.hpp"
#include "../viterbi/viterbi.h"

#include <cstdint>
#include <span>
#include <vector>

namespace sts1cobcsw
{
    void Encode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst);
    void Decode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst);
}
