#include "encode_decode.h"
#include "../viterbi/viterbi.h"
#include "../reed_solomon/include/rs.hpp"
#include "../bit_scrambling/Cpp/bitsnarl.hpp"

#include <cstdint>
#include <span>
#include <vector>

namespace sts1cobcsw
{
    static RS::ReedSolomon<223, 32> rs;

    void Encode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        auto encoded = std::vector<std::uint8_t>();
        rs.Encode(src, encoded);
        bitsn::scramble(encoded);
        auto codec = ViterbiCodec();
        codec.Encode(encoded, dst);
    }
    void Decode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        bitsn::un_scramble(src);
        rs.Decode(src, dst);
    }

}