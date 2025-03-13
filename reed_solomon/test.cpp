#include "rs.hpp"
#include <cstddef>
#include <iostream>
#include <cstdint>
#include <cassert>

void test_ccsds_short_message() {
    RS::ReedSolomon<223, 32> rs;
    uint8_t data[223] = {};  
    std::cout << "size is: " << sizeof(data) << "\n";
    for (int i = 0; i < sizeof(data); i++) {
        // data[i] = rand() % 255;
        data[i] = i;
    }

    uint8_t buffer[255] = {};

    rs.Encode(data, buffer);

    int random = 500;
    for (int i = 0; i < 32; i++) {
        data[(i + random) % 255] = 0;
    }

    uint8_t decoded[223] = {};

    rs.Decode(buffer, decoded);

    for (int i = 0; i < sizeof(decoded); i++) {
        assert(data[i] == decoded[i]);
    }
    std::cout << "Test successful!" << "\n";
}
int main() {
    test_ccsds_short_message();
    return 0;
}

