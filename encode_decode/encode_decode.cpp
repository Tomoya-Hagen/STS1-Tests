#include <vector>
#include <array>
#include <cstdint>
#include "encode_decode.h"
#include "../viterbi/viterbi.h"
#include "../reed_solomon/include/rs.hpp"
#include "../bit_scrambling/Cpp/bitsnarl.hpp"

namespace sts1cobcsw
{
    void Encode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        auto encoded = std::vector<std::uint8_t>();
        static RS::ReedSolomon<223, 32> rs;
        rs.Encode(src, encoded);
        bitsn::scramble(encoded);
        auto codec = ViterbiCodec();
        codec.Encode(encoded, dst);
    }
    void Decode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        bitsn::un_scramble(src);
        static RS::ReedSolomon<223, 32> rs;
        rs.Decode(src, dst);
    }

}