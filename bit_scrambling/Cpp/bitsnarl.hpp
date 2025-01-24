#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <valarray>
#include <vector>
#include <cstddef>

namespace bitsn
{

    static const std::uint8_t GF_table_TC[255] = { // for telecommand unscrambling. Polynomial: 0b01011111
        0xff, 0x39, 0x9e, 0x5a, 0x68, 0xe9, 0x06, 0xf5, 0x6c, 0x89, 0x2f, 0xa1, 0x31, 0x5e, 0x08, 0xc0,
        0x52, 0xa8, 0xbb, 0xae, 0x4e, 0xc2, 0xc7, 0xed, 0x66, 0xdc, 0x38, 0xd4, 0xf8, 0x86, 0x50,
        0x3d, 0xfe, 0x73, 0x3c, 0xb4, 0xd1, 0xd2, 0x0d, 0xea, 0xd9, 0x12, 0x5f, 0x42, 0x62, 0xbc,
        0x11, 0x80, 0xa5, 0x51, 0x77, 0x5c, 0x9d, 0x85, 0x8f, 0xda, 0xcd, 0xb8, 0x71, 0xa9, 0xf1,
        0x0c, 0xa0, 0x7b, 0xfc, 0xe6, 0x79, 0x69, 0xa3, 0xa4, 0x1b, 0xd5, 0xb2, 0x24, 0xbe, 0x84,
        0xc5, 0x78, 0x23, 0x01, 0x4a, 0xa2, 0xee, 0xb9, 0x3b, 0x0b, 0x1f, 0xb5, 0x9b, 0x70, 0xe3,
        0x53, 0xe2, 0x19, 0x40, 0xf7, 0xf9, 0xcc, 0xf2, 0xd3, 0x47, 0x48, 0x37, 0xab, 0x64, 0x49,
        0x7d, 0x09, 0x8a, 0xf0, 0x46, 0x02, 0x95, 0x45, 0xdd, 0x72, 0x76, 0x16, 0x3f, 0x6b, 0x36,
        0xe1, 0xc6, 0xa7, 0xc4, 0x32, 0x81, 0xef, 0xf3, 0x99, 0xe5, 0xa6, 0x8e, 0x90, 0x6f, 0x56,
        0xc8, 0x92, 0xfa, 0x13, 0x15, 0xe0, 0x8c, 0x05, 0x2a, 0x8b, 0xba, 0xe4, 0xec, 0x2c, 0x7e,
        0xd6, 0x6d, 0xc3, 0x8d, 0x4f, 0x88, 0x65, 0x03, 0xdf, 0xe7, 0x33, 0xcb, 0x4d, 0x1d, 0x20,
        0xde, 0xad, 0x91, 0x25, 0xf4, 0x26, 0x2b, 0xc1, 0x18, 0x0a, 0x55, 0x17, 0x75, 0xc9, 0xd8,
        0x58, 0xfd, 0xac, 0xdb, 0x87, 0x1a, 0x9f, 0x10, 0xca, 0x07, 0xbf, 0xce, 0x67, 0x96, 0x9a,
        0x3a, 0x41, 0xbd, 0x5b, 0x22, 0x4b, 0xe8, 0x4c, 0x57, 0x82, 0x30, 0x14, 0xaa, 0x2e, 0xeb,
        0x93, 0xb0, 0xb1, 0xfb, 0x59, 0xb7, 0x0e, 0x35, 0x3e, 0x21, 0x94, 0x0f, 0x7f, 0x9c, 0xcf,
        0x2d, 0x34, 0x74, 0x83, 0x7a, 0xb6, 0x44, 0x97, 0xd0, 0x98, 0xaf, 0x04, 0x60, 0x29, 0x54,
        0x5d, 0xd7, 0x27, 0x61, 0x63, 0xf6, 0xb3, 0x6e, 0x1c, 0x6a, 0x7c, 0x43, 0x28, 0x1e};

    static const std::uint8_t GF_table_TM[255] = { // for telemetry scrambling. 0b10101001
        0xff, 0x48, 0x0e, 0xc0, 0x9a, 0x0d, 0x70, 0xbc, 0x8e, 0x2c, 0x93, 0xad, 0xa7, 0xb7, 0x46,
        0xce, 0x5a, 0x97, 0x7d, 0xcc, 0x32, 0xa2, 0xbf, 0x3e, 0x0a, 0x10, 0xf1, 0x88, 0x94, 0xcd,
        0xea, 0xb1, 0xfe, 0x90, 0x1d, 0x81, 0x34, 0x1a, 0xe1, 0x79, 0x1c, 0x59, 0x27, 0x5b, 0x4f,
        0x6e, 0x8d, 0x9c, 0xb5, 0x2e, 0xfb, 0x98, 0x65, 0x45, 0x7e, 0x7c, 0x14, 0x21, 0xe3, 0x11,
        0x29, 0x9b, 0xd5, 0x63, 0xfd, 0x20, 0x3b, 0x02, 0x68, 0x35, 0xc2, 0xf2, 0x38, 0xb2, 0x4e,
        0xb6, 0x9e, 0xdd, 0x1b, 0x39, 0x6a, 0x5d, 0xf7, 0x30, 0xca, 0x8a, 0xfc, 0xf8, 0x28, 0x43,
        0xc6, 0x22, 0x53, 0x37, 0xaa, 0xc7, 0xfa, 0x40, 0x76, 0x04, 0xd0, 0x6b, 0x85, 0xe4, 0x71,
        0x64, 0x9d, 0x6d, 0x3d, 0xba, 0x36, 0x72, 0xd4, 0xbb, 0xee, 0x61, 0x95, 0x15, 0xf9, 0xf0,
        0x50, 0x87, 0x8c, 0x44, 0xa6, 0x6f, 0x55, 0x8f, 0xf4, 0x80, 0xec, 0x09, 0xa0, 0xd7, 0x0b,
        0xc8, 0xe2, 0xc9, 0x3a, 0xda, 0x7b, 0x74, 0x6c, 0xe5, 0xa9, 0x77, 0xdc, 0xc3, 0x2a, 0x2b,
        0xf3, 0xe0, 0xa1, 0x0f, 0x18, 0x89, 0x4c, 0xde, 0xab, 0x1f, 0xe9, 0x01, 0xd8, 0x13, 0x41,
        0xae, 0x17, 0x91, 0xc5, 0x92, 0x75, 0xb4, 0xf6, 0xe8, 0xd9, 0xcb, 0x52, 0xef, 0xb9, 0x86,
        0x54, 0x57, 0xe7, 0xc1, 0x42, 0x1e, 0x31, 0x12, 0x99, 0xbd, 0x56, 0x3f, 0xd2, 0x03, 0xb0,
        0x26, 0x83, 0x5c, 0x2f, 0x23, 0x8b, 0x24, 0xeb, 0x69, 0xed, 0xd1, 0xb3, 0x96, 0xa5, 0xdf,
        0x73, 0x0c, 0xa8, 0xaf, 0xcf, 0x82, 0x84, 0x3c, 0x62, 0x25, 0x33, 0x7a, 0xac, 0x7f, 0xa4,
        0x07, 0x60, 0x4d, 0x06, 0xb8, 0x5e, 0x47, 0x16, 0x49, 0xd6, 0xd3, 0xdb, 0xa3, 0x67, 0x2d,
        0x4b, 0xbe, 0xe6, 0x19, 0x51, 0x5f, 0x9f, 0x05, 0x08, 0x78, 0xc4, 0x4a, 0x66, 0xf5, 0x58};

    template <typename T>
    struct is_array_of_uint8
    {
        enum
        {
            value = false
        };
    };

    template <>
    struct is_array_of_uint8<std::vector<std::uint8_t>>
    {
        enum
        {
            value = true
        };
    };

    template <>
    struct is_array_of_uint8<std::valarray<std::uint8_t>>
    {
        enum
        {
            value = true
        };
    };

    template <std::size_t N>
    struct is_array_of_uint8<std::array<std::uint8_t, N>>
    {
        enum
        {
            value = true
        };
    };

    auto calculate_feedback(std::uint8_t state, std::vector<uint8_t> coefficients) -> std::uint8_t
    {
        assert(coefficients.size() == 8);
        assert(std::find(coefficients.begin(), coefficients.end(), 1) != coefficients.end());
        auto shifter = coefficients.size() - 1;
        auto result = 0;
        auto first_bit_set = false;

        for (int i = 0; i < coefficients.size(); i++)
        {
            if (coefficients[i] == 1)
            {
                if (!first_bit_set)
                {
                    result = (state >> (shifter - i)) & 1;
                    first_bit_set = true;
                }
                else
                {
                    result ^= (state >> (shifter - i)) & 1;
                }
            }
        }
        return result;
    }

    auto calculate_galois_table(int polynomial, int degree) -> std::vector<std::uint8_t>
    {
        auto x = std::pow(2, degree);
        auto table = std::vector<std::uint8_t>(x);
        auto state = 0xFF; // initialy all input bits are 1
        table[0] = state;
        auto coefficients = std::vector<std::uint8_t>(degree);
        auto extractor = std::uint8_t(std::pow(2, degree - 1));
        assert(0b10000000); // for our use case
        auto shifter = degree - 1;

        for (int i = 0; i < degree; i++)
        {
            coefficients[i] = (polynomial & extractor) >> shifter;
            extractor >>= 1;
            shifter--;
        }
        for (int i = 1; i < x; i++)
        {
            std::uint8_t byte = 0;
            for (int j = 0; j < degree; j++)
            {
                auto feedback_bit = calculate_feedback(state, coefficients);
                state = (state >> 1) | (feedback_bit << (degree - 1));
                assert(state >= 0 && state <= 0xFF);
                byte = (byte << 1) | feedback_bit;
            }
            assert(byte >= 0 && byte <= 0xFF);
            table[i] = byte;
        }
        assert((state == 0xFF) && (table[0] == table[table.size() - 1])); // verify periodicity
        table.pop_back();
        return table;
    }

    template <
        typename T,
        typename std::enable_if<is_array_of_uint8<T>::value, std::nullptr_t>::type = nullptr>
    // requires (is_array_of_uint8<T>::value)
    void unscramble_telecommand(T &bytes)
    {
        for (int i = 0; i < bytes.size(); i++)
        {
            bytes[i] ^= GF_table_TC[i % 255];
        }
    }

    template <
        typename T,
        typename std::enable_if<is_array_of_uint8<T>::value, std::nullptr_t>::type = nullptr>
    void scramble_telemetry(T &bytes) //
    {
        for (int i = 0; i < bytes.size(); i++)
        {
            bytes[i] ^= GF_table_TM[i % 255];
        }
    }

}
