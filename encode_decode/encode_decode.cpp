#include "encode_decode.h"
#include "../viterbi/viterbi.h"
#include "../reed_solomon/include/rs.hpp"
#include "../bit_scrambling/Cpp/bitsnarl.hpp"

#include <algorithm>
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
            uint8_t block[block_size] = {0};
            // memcpy(block, src.data() + start_idx, block_size);
            std::copy(src.data() + start_idx, src.data() + start_idx + block_size, block + start_idx);
            rs.Encode(block, encoded.data() + i * rs_length);
        }

        std::cout << "code: after RS-encode: " << encoded.size() << "\n";
        bitsn::scramble_telemetry(encoded);
        std::cout << "code: after scrambling: " << encoded.size() << "\n";

        AppendPreambleAndSyncMarker(encoded);

        auto codec = ViterbiCodec();
        codec.Encode(encoded, dst);
        std::cout << "dst: " << dst.size() << "\n";
    }

    void AppendPreambleAndSyncMarker(std::vector<std::uint8_t> &encoded)
    {
        auto preamble_sync_marker_length = 8;
        encoded.resize(encoded.size() + preamble_sync_marker_length); // 4 Bytes-Preamble and 4 Bytes of Sync Marker
        std::rotate(encoded.rbegin(), encoded.rbegin() + preamble_sync_marker_length, encoded.rend()); // right-shifts 8 entries forward
        auto sync_marker = std::array<uint8_t, 4>{0b00011010, 0b11001111, 0b11111100, 0b00011101};
        for (int i = 0; i < preamble_sync_marker_length; i++) {
            if (i < 4) {
                encoded[i] = 0x33; // the preamble
            } else {
                encoded[i] = sync_marker[i - 4];
            }
        }
    }

    void Decode(std::span<std::uint8_t> src, std::vector<std::uint8_t> &dst)
    {
        assert(!src.empty() && src.size() == 255);
        auto buffer = std::vector<std::uint8_t>(src.begin(), src.end());
        bitsn::unscramble_telecommand(buffer);

        const int rs_length = block_size + parity_symbols;
        const int input_size = buffer.size();
        // assert(input_size % rs_length == 0);
        int total_blocks = input_size / rs_length;
        dst.resize(total_blocks * block_size);
        for (int i = 0; i < total_blocks; i++)
        {
            const uint8_t *current_block = buffer.data() + i * rs_length;
            uint8_t decoded_block[block_size] = {0};
            rs.Decode(current_block, decoded_block);
            int start_idx = i * block_size;
            // memcpy(dst.data() + start_idx, decoded_block, block_size);
            std::copy(decoded_block, decoded_block + block_size, dst.data() + start_idx);
        }
    }
}
