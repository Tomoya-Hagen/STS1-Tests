#include "test.h"
#include "encode_decode.h"
#include "../viterbi/viterbi.h"

#include <cassert>
#include <cstdint>
#include <format>
#include <iostream>
#include <span>
#include <stdlib.h>
#include <string>
#include <vector>

void test_encode_decode_short()
{
    auto src = std::vector<std::uint8_t>();

    std::cout << "input: ";
    for (int i = 0; i < 223; i++)
    {
        src.push_back(rand() % 50);
        std::cout << std::to_string(src[i]) << " ";
    }
    std::cout << "\n";

    auto dst = std::vector<std::uint8_t>();
    sts1cobcsw::Encode(src, dst);
    int control_size = dst.size(); // the size of the total code word. used to test the string of the code word.
    auto encoded = std::string();

    for (int i = 0; i < dst.size(); i++)
    {
        auto x = dst[i];
        auto byte = std::string();
        int limit = 8;
        while (limit > 0)
        {
            byte += (x & 1 ? '1' : '0');
            x >>= 1;
            limit--;
        }

        // last byte
        if (i == dst.size() - 1)
        {
            int index = 0;
            int j = 0;
            while (j < byte.size())
            { // find the first 1 to cut off the bits before it. E.g. 00000001 => index = 7
                if (byte[j] == '1')
                {
                    index = j;
                }
                j++;
            }
            int lim = 8 - index;
            while (lim > 0)
            {
                encoded += byte[index--];
                lim--;
            }
        }

        std::reverse(byte.begin(), byte.end());
        encoded += byte;
    }

    auto decoded = codec.DecodeToString(encoded);
    decoded = decoded.substr(0, decoded.size() - 3); // Cut off 3 last bits since the src.size() will always be odd
    auto buffer = std::vector<std::uint8_t>();

    auto byte = 0;
    for (int i = 0; i < decoded.size(); i++)
    {
        byte = (byte << 1) | (decoded[i] - '0');
        if (i != 0 && i % 7 == 0)
        {
            assert(byte >= 0 && byte <= 0b11111111);
            buffer.push_back(byte);
            byte = 0;
        }
    }

    if (byte != 0)
    {
        buffer.push_back(byte);
    }

    std::cout << "after cc decoding, size: " << buffer.size() << " code word:";
    for (auto x : buffer)
    {
        std::cout << std::to_string(x) << " ";
    }
    std::cout << "\n";

    auto destination = std::vector<std::uint8_t>();
    sts1cobcsw::Decode(buffer, destination);
    std::cout << "src: " << src.size() << "\n";
    std::cout << "destination: " << destination.size() << "\n";
    assert(src.size() == destination.size());
    for (auto x : destination)
    {
        std::cout << std::to_string(x) << " ";
    }
    // for (int i = 0; i < src.size(); i++) {
    //     assert(src[i] == destination[i]);
    // }
    // std::cout << "Test 'test_encode_decode_short' passed" << "\n";
}

void test_encode_decode_long()
{
    auto src = std::vector<std::uint8_t>();

    for (int i = 0; i < 1000; i++)
    {
        src.push_back(rand() % 255);
    }

    std::vector<std::uint8_t> dst;
    sts1cobcsw::Encode(src, dst);

    auto encoded = std::string();

    for (auto x : dst)
    {
        encoded += (x - '0');
    }

    auto decoded = encoded; // = codec.Decode(encoded);

    std::vector<std::uint8_t> buffer;

    for (char bit : decoded)
    {
        buffer.push_back(bit - '0');
    }

    auto destination = std::vector<std::uint8_t>();
    sts1cobcsw::Decode(buffer, destination);

    assert(src.size() == destination.size());
    for (int i = 0; i < src.size(); i++)
    {
        assert(src[i] == destination[i]);
    }
    std::cout << "Test 'test_encode_decode_long' passed" << "\n";
}

void test_encode_decode_insert_error_success()
{
    auto src = std::vector<std::uint8_t>();

    for (int i = 0; i < 500; i++)
    {
        src.push_back(rand() % 255);
    }

    std::vector<std::uint8_t> dst;
    sts1cobcsw::Encode(src, dst);

    auto encoded = std::string();

    for (auto x : dst)
    {
        encoded += (x - '0');
    }

    int errors = 8;
    assert(errors < encoded.size());
    int pos = encoded.size() / 2;
    for (int i = 0; i < errors; i++)
    {
        if (encoded[pos + i] == '0')
        {
            encoded[pos + i] = '1';
        }
        else
        {
            encoded[pos + i] = '0';
        }
    }
    auto decoded = encoded; //  = codec.Decode(encoded);

    auto buffer = std::vector<std::uint8_t>();

    for (char bit : decoded)
    {
        buffer.push_back(bit - '0');
    }

    auto destination = std::vector<std::uint8_t>();
    sts1cobcsw::Decode(buffer, destination);

    assert(src.size() == destination.size());
    for (int i = 0; i < src.size(); i++)
    {
        assert(src[i] == destination[i]);
    }
    std::cout << "Test 'test_encode_decode_insert_error_success' passed" << "\n";
}

void test_encode_decode_insert_error_fail()
{
}

void test_preamble_and_sync_marker()
{
    auto buffer = std::vector<std::uint8_t>();
    buffer.resize(255);
    std::fill(buffer.begin(), buffer.end(), 1);
    sts1cobcsw::AppendPreambleAndSyncMarker(buffer);
    assert(buffer.size() == 263);
    auto sync_marker = std::array<uint16_t, 4>{0x3C, 0x67, 0x49, 0x52};
    for (int i = 0; i < buffer.size(); i++)
    {
        auto x = buffer[i];
        if (i < 4)
        {
            assert(x == 0x33);
        }
        if (i >= 4 && i < 8) {
            assert(x == sync_marker[i - 4]);
        }
        std::cout << std::to_string(x) << " ";
    }
    std::cout << "\n";
    std::cout << "Test 'test_preamble_and_sync_marker' passed" << "\n";
}

int main()
{
    // test_encode_decode_short();
    // test_encode_decode_long();
    // test_encode_decode_insert_error_success();
    // test_encode_decode_insert_error_fail();
    test_preamble_and_sync_marker();
    return 0;
}