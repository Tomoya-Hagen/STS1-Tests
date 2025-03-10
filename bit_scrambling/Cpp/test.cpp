#include <iostream>
#include <cstddef>
#include <cstring>
#include <format>
#include "bitsnarl.hpp"

#define N 4000
void test_scramble_identity()
{
    std::array<std::uint8_t, N> bytes_orig, bytes_copy;
    for (int i = 0; i < N; i++)
    {
        bytes_orig[i] = rand() % 255;
    }
    std::memcpy(bytes_copy.begin(), bytes_orig.begin(), N);
    bitsn::unscramble_telecommand(bytes_orig);
    // bitsn::scramble_telemetry(bytes_orig);
    bitsn::unscramble_telecommand(bytes_orig);
    for (int i = 0; i < N; i++)
    {
        if (bytes_orig[i] != bytes_copy[i])
        {
            printf("diff at %i-th: %d\n", i, bytes_copy[i]);
        }
    }
}

void test_scramble_error_spread()
{
    double same_rate = 0.0;
    double avg_same_rate = 0.0;

    std::array<std::uint8_t, N> bytes_orig, bytes_scrambled;
    for (int i = 0; i < N; i++)
    {
        bytes_orig[i] = rand() % 255;
    }
    memcpy(bytes_scrambled.begin(), bytes_orig.begin(), N);
    bitsn::unscramble_telecommand(bytes_scrambled);
    for (int p = 0; p < N; p++)
    { // error at p
        std::array<uint8_t, N> bytes_dirty;
        memcpy(bytes_dirty.begin(), bytes_scrambled.begin(), N);
        bytes_dirty[p] += 7; // inject some error
        // bitsn::scramble_telemetry(bytes_dirty);
        bitsn::unscramble_telecommand(bytes_dirty);
        int same_count = 0;
        for (int i = 0; i < N; i++)
        {
            if (bytes_dirty[i] == bytes_scrambled[i])
            {
                same_count++;
            }
        }
        same_rate = (double)same_count / (double)N;
        avg_same_rate += same_rate;
    }
    double error = avg_same_rate / (double)N;
    printf("Probability of getting same bytes after bit error: %f\n", error);
    printf("Probability of getting same bytes at random: 1/256=%f\n", 1.0 / 256.0);
}

void test_scramble_and_unscramble()
{
    std::valarray<std::uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8};

    std::cout << "Original" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;

    bitsn::unscramble_telecommand(data);
    std::cout << "Scrambled" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;

    // bitsn::scramble_telemetry(data);
    bitsn::unscramble_telecommand(data);
    std::cout << "Un-Scrambled" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;
}

void test_scramble_and_unscramble2()
{
    std::valarray<std::uint8_t> data = {4, 1, 5, 12, 2, 5, 19, 80};

    std::cout << "Original" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;

    bitsn::unscramble_telecommand(data);
    std::cout << "Scrambled" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;

    // bitsn::scramble_telemetry(data);
    bitsn::unscramble_telecommand(data);
    std::cout << "Un-Scrambled" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;
}

void test_scramble_and_unscramble_string()
{
    std::string data = "This is a test";
    std::array<std::uint8_t, 16> bytes;

    for (int i = 0; i < 16; i++)
    {
        bytes[i] = data[i];
    }

    std::cout << "Original text: " << data << std::endl;

    std::cout << "Original bytes: ";
    for (int i = 0; i < 16; i++)
    {
        std::cout << (int)bytes[i] << " ";
    }
    std::cout << std::endl;

    bitsn::unscramble_telecommand(bytes);

    std::cout << "Scrambled bytes: ";
    for (int i = 0; i < 16; i++)
    {
        std::cout << (int)bytes[i] << " ";
    }
    std::cout << std::endl;

    std::string scrambled_data(bytes.begin(), bytes.end());
    std::cout << "Scrambled text: " << scrambled_data << std::endl;

    // bitsn::scramble_telemetry(bytes);
    bitsn::unscramble_telecommand(bytes);

    std::string unscrambled_data(bytes.begin(), bytes.end());

    std::cout << "Un-scrambled bytes: " << unscrambled_data << std::endl;
}

void test_scramble_and_unscramble_with_errors()
{
    std::valarray<std::uint8_t> data = {200, 100, 50, 25, 12, 6, 3, 1};

    std::cout << "Original" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;

    bitsn::scramble_telemetry(data);
    std::cout << "Scrambled" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;

    bitsn::scramble_telemetry(data);
    std::cout << "Un-Scrambled" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;

    bitsn::scramble_telemetry(data);
    data[0] += 1;
    data[5] += 1;

    bitsn::scramble_telemetry(data);
    std::cout << "Un-Scrambled with errors" << std::endl;
    for (auto &e : data)
    {
        std::cout << (int)e << " ";
    }
    std::cout << std::endl;
}

void test_galois_field()
{
    auto galois_field = bitsn::calculate_galois_table(0b01011111, 8); // 0b10101001 for TM // 0b01011111 for TC
    std::cout << "size: " << galois_field.size() << "\n";
    for (int i = 0; i < galois_field.size(); i++)
    {
        std::cout << "0x" << std::format("{:02x}", galois_field[i]);
        if (i != galois_field.size() - 1) {
            std::cout << ", ";
        }
        if (i % 15 == 0 && i == galois_field.size() - 1) {
            std::cout << "\n";
        }
    }
}

int main()
{
    test_galois_field();
    // test_scramble_identity();
    // test_scramble_error_spread();
    // test_scramble_and_unscramble();
    // test_scramble_and_unscramble2();
    // test_scramble_and_unscramble_with_errors();
    // test_scramble_and_unscramble_string();

}
