#pragma once

#include <vector>
#include <cstdint>
#include "../viterbi/viterbi.h"
#include "../reed_solomon/include/rs.hpp"
#include "../bit_scrambling/Cpp/bitsnarl.hpp"


namespace sts1_cobc_sw {
    void Encode(std::span<std::uint8_t> src, std::vector<std::uint8_t>& dst);
    void Decode(std::span<std::uint8_t src, std::vector<std::uint8_t>& dst);

}