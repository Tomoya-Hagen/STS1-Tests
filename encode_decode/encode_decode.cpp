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
        int input_size = src.size();
        int block_size = 223;
        int ecc_length = 32;
        const int total_blocks = (input_size + block_size - 1) / block_size;
        uint8_t encoded[total_blocks][block_size + ecc_length];

        for (int i = 0; i < total_blocks; i++)
        {
            int start_idx = i * block_size;
            int current_block_size = std::min(block_size, total_message_length - start_idx);
            uint8_t block[block_size] = {0};
            memcpy(block, message + start_idx, current_block_size);

            rs.Encode(block, encoded[i]);
        }

        uint8_t code[total_blocks * (block_size + ecc_length)] = {0}; // ignores the filling zeroes!
        for (int i = 0; i < total_blocks; i++)
        {
            memcpy(code + (i * (block_size + ecc_length)), encoded[i], block_size + ecc_length);
            // bitsn::scramble(encoded[i]); // probably the order of process wrong, since encoded block-wise. Maybe a new array
            // of length of the whole encoded RS-Code.
        }
        bitsn::scramble(code);

        auto codec = ViterbiCodec();
        codec.Encode(code, dst);
    }
    void Decode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        auto buffer = std::vector<std::uint8_t>(src.begin(), src.end());
        bitsn::un_scramble(buffer);
        rs.Decode(buffer.data(), dst.data());
    }

}