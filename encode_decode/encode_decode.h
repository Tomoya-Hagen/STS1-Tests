#pragma once

#include "../bit_scrambling/Cpp/bitsnarl.hpp"
#include "../reed_solomon/rs.hpp"
#include "../viterbi/viterbi.h"

#include <cstdint>
#include <span>
#include <vector>

namespace sts1cobcsw
{
    void Encode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst);
    void ConvertBases(uint8_t *data, bool dual_to_basis, size_t size);
    void AppendPreambleAndSyncMarker(std::vector<std::uint8_t> &encoded);
    void Decode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst);
}
