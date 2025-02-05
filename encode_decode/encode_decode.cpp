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
    static constexpr int parity_symbols = 32;

    void Encode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        int rs_length = block_size + parity_symbols;
        const int input_size = src.size();
        assert(!src.empty() && input_size % block_size == 0);
        int total_blocks = input_size / block_size;

        auto encoded = std::vector<uint8_t>(total_blocks * rs_length, 0);

        for (int i = 0; i < total_blocks; i++)
        {
            int start_idx = i * block_size;
            int current_block_size = std::min(block_size, input_size - start_idx);
            uint8_t block[block_size] = {0};
            memcpy(block, src.data() + start_idx, current_block_size);
            rs.Encode(block, encoded.data() + i * rs_length);
        }

        std::cout << "code: after RS-encode: " << encoded.size() << "\n";
        bitsn::scramble_telemetry(encoded);
        std::cout << "code: after scrambling: " << encoded.size() << "\n";

        auto codec = ViterbiCodec();
        codec.Encode(encoded, dst);
        std::cout << "dst: " << dst.size() << "\n";
    }

    void Decode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst) // src.size() == 255 * x, x element Z?
    {
        // convolutional decoding is executed by the ground station.
        assert(!src.empty());
        auto buffer = std::vector<std::uint8_t>(src.begin(), src.end());
        bitsn::unscramble_telecommand(buffer);

        const int input_size = buffer.size();
        // assert(input_size % block_size == 0);
        int total_blocks = (input_size + block_size - 1) / block_size;
        dst.resize(total_blocks * block_size);
        for (int i = 0; i < total_blocks; i++)
        {
            const uint8_t *current_block = buffer.data() + i * (block_size + parity_symbols);
            uint8_t decoded_block[block_size] = {0};
            rs.Decode(current_block, decoded_block);
            int start_idx = i * block_size;
            // int current_block_size = std::min(block_size, static_cast<int>(dst.size()) - start_idx); // unnecessary, if the block_size is ensured to be 223 symbols long
            memcpy(dst.data() + start_idx, decoded_block, block_size);
        }
    }
}
