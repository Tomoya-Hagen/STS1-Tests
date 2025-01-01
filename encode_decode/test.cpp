#include "test.h"
#include "encode_decode.h"
#include "../cc-viterbi/viterbi.h"

#include <cassert>
#include <cstdint>
#include <format>
#include <iostream>
#include <span>
#include <stdlib.h>
#include <string>
#include <vector>


void test_encode_decode_short() {
    auto src = std::vector<std::uint8_t>();

    for (int i = 0; i < 223; i++) {
        src.push_back(rand() % 255);
    }

    std::vector<std::uint8_t> dst;
    sts1cobcsw::Encode(src, dst);

    auto encoded = std::string();

    for (auto x : dst) {
        encoded += (x - '0');
    }

    auto decoded = codec.DecodeToString(encoded);

    std::vector<std::uint8_t> buffer;

    for (char bit : decoded) {
        buffer.push_back(bit - '0');
    }

    for (int i = 0; i < src.size(); i++) {
        assert(src[i] == buffer[i]);
    }
    std::cout << "Test 'test_encode_decode_short' passed" << "\n";
}

void test_encode_decode_long() {

}

void test_encode_decode_insert_error_success() {

}

void test_encode_decode_insert_error_fail() {

}

int main() {
    test_encode_decode_short();
    test_encode_decode_long();
    test_encode_decode_insert_error_success();
    test_encode_decode_insert_error_fail();
    return 0;
}