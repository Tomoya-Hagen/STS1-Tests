#include <array>
#include <cassert>
#include "BLAKE2s.h"
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string.h>

void printHex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    std::cout << std::dec << std::endl;
}

void hash_empty_input()
{
    printf("test empty input\n");
    uint8_t hash[32]; // uint8_t is 1 Byte
    BLAKE2s blake2s;
    uint8_t key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                   0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                   0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
    blake2s.resetHMAC(key, 32);
    blake2s.update("", 0);
    blake2s.finalizeHMAC(key, 32, hash, 32);
    for (int i = 0; i < 32; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void hash_single_block_input()
{
    printf("test single block input\n");
    BLAKE2s blake2s;
    std::string data = "This frog has been stripped of the owner. It is now too hot too";
    uint8_t hash[32]; // truncated to 64 bits
    uint8_t key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                   0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                   0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
    blake2s.resetHMAC(key, 32);
    blake2s.update(data.c_str(), data.size());
    blake2s.finalizeHMAC(key, 32, hash, 32);
    for (int i = 0; i < 8; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void hash_double_block_input()
{
    printf("test double block input\n");
    BLAKE2s blake2s;
    std::string data = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012";
    std::string key = "oida!";
    uint8_t hash[32];
    blake2s.resetHMAC(key.c_str(), key.size());
    blake2s.update(data.c_str(), data.size());
    blake2s.finalizeHMAC(key.c_str(), key.size(), hash, 32);
    for (int i = 0; i < 8; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void hash_multiple_block_input()
{
    printf("test multiple block input\n");
    BLAKE2s blake2s;
    uint8_t data[256] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};
    uint8_t hash[32];
    uint8_t key[12] = {1, 98, 99, 90, 9, 89, 77, 200, 199, 99, 9, 159};
    blake2s.resetHMAC(key, 12);
    blake2s.update(data, 256);
    blake2s.finalizeHMAC(key, 12, hash, 32);
    for (int i = 0; i < 8; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void hash_partial_block_input()
{
    printf("test partial block input\n");
    BLAKE2s blake2s;
    std::array<uint8_t, 4> data = { 'E', 'x', 'a', 'm'};
    std::array<char, 10> key = {'a', 'e', '&', '#', '3', 'G', 'K', '!', ')', ']'};
    uint8_t hash[32];
    blake2s.resetHMAC(key.data(), key.size()); // "data()" returns a pointer to the first element, include the array header
    blake2s.update(data.data(), data.size());
    blake2s.finalizeHMAC(key.data(), key.size(), hash, 32);
    for (int i = 0; i < 8; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void hash_block_smaller_than_one_block()
{
    printf("test block smaller than a block size\n");
    BLAKE2s blake2s;
    uint8_t data[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    uint8_t hash[32];
    blake2s.reset(32);
    blake2s.update(data, 32);
    blake2s.finalize(hash, 32);
    for (int i = 0; i < 32; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

void test_large_data()
{
    printf("test large data\n");
    uint8_t largeData[10 * 1024 * 1024]; // 10 MB of data
    uint8_t hash[32];
    BLAKE2s blake2s;
    memset(largeData, 0xAB, sizeof(largeData));
    blake2s.reset(32);
    blake2s.update(largeData, sizeof(largeData));
    blake2s.finalize(hash, 32);
}

void test_hmac()
{
    printf("test hmac\n");
    uint8_t key[32] = { 5, 3, 1, 2, 6, 4, 8, 2, 10, 19, 2, 7, 2, 5, 3, 1  };  // 32-byte key
    uint8_t message[] = { 2, 5, 8, 12, 9, 10, 45 };  // Message of any length
    uint8_t result[32];

    BLAKE2s blake2s;

    blake2s.resetHMAC(key, sizeof(key));
    blake2s.update(message, sizeof(message));
    blake2s.finalizeHMAC(key, sizeof(key), result, sizeof(result));

    printHex(result, 32);
}



void testBasicSignatureGeneration() {
    std::cout << "Test Case 1: Basic Signature Generation" << std::endl;

    const std::string data = "Uplink data example message";
    const std::string key = "testkey123";

    BLAKE2s blake2s;
    blake2s.resetHMAC(key.c_str(), key.length());
    blake2s.update(data.c_str(), data.length());

    uint8_t hash[32];
    blake2s.finalizeHMAC(key.c_str(), key.length(), hash, sizeof(hash));

    std::cout << "Generated HMAC Signature: ";
    printHex(hash, sizeof(hash));
}

int main()
{
    // hash_empty_input(); // checked
    // test_hmac(); // checked
    // hash_single_block_input(); // checked
    // hash_double_block_input(); // checked
    // hash_partial_block_input(); // checked
    // hash_multiple_block_input(); // checked
    hash_block_smaller_than_one_block();
    // testBasicSignatureGeneration(); // checked
}