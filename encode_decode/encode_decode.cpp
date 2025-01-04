#include "encode_decode.h"
#include "../viterbi/viterbi.h"
#include "../reed_solomon/include/rs.hpp"
#include "../bit_scrambling/Cpp/bitsnarl.hpp"

#include <cstdint>
#include <iostream>
#include <span>
#include <vector>

namespace sts1cobcsw
{
    static RS::ReedSolomon<223, 32> rs;
    static constexpr int block_size = 223;
    static constexpr int ecc_length = 32;

    void Encode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        assert(!src.empty());
        const int input_size = src.size();
        int total_blocks = (input_size + block_size - 1) / block_size;
        int rs_length = block_size + ecc_length;

        auto encoded = std::vector<uint8_t>(total_blocks * rs_length, 0); // this still ignores the filling zeros!

        for (int i = 0; i < total_blocks; i++)
        {
            int start_idx = i * block_size;
            int current_block_size = std::min(block_size, input_size - start_idx); // possibly an edge case handling for the last block
            uint8_t block[block_size] = {0};
            memcpy(block, src.data() + start_idx, current_block_size);
            rs.Encode(block, encoded.data() + i * rs_length);
        }

        std::cout << "code: after RS-encode: " << encoded.size() << "\n";
        bitsn::scramble(encoded);
        std::cout << "code: after scrambling: " << encoded.size() << "\n";

        auto codec = ViterbiCodec();
        codec.Encode(encoded, dst);
        std::cout << "dst: " << dst.size() << "\n";
    }
    void Decode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        assert(!src.empty());
        auto buffer = std::vector<std::uint8_t>(src.begin(), src.end()); // no array possible as src.size() known at runtime.
        bitsn::un_scramble(buffer);

        const int input_size = buffer.size();
        int total_blocks = (input_size + block_size - 1) / block_size;
        dst.resize(total_blocks * block_size);
        for (int i = 0; i < total_blocks; i++)
        {
            const uint8_t *current_block = buffer.data() + i * (block_size + ecc_length);
            uint8_t decoded_block[block_size] = {0};
            rs.Decode(current_block, decoded_block);
            int start_idx = i * block_size;
            int current_block_size = std::min(block_size, static_cast<int>(dst.size()) - start_idx);
            memcpy(dst.data() + start_idx, decoded_block, current_block_size);
        }
    }
}
