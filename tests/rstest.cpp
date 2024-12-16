/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#include "rstest.hpp"
#include <iomanip>
#include <rs.hpp>
#include <vector>
#include <random>

Report
RStest::run_tests()
{
    // bool (*tests[])(std::string&) = {
    //     test_encode,
    //     test_decode,
    //     test_stress
    // };

    Report rep = {3, 0};

    // for(uint i = 0; i < rep.overall; i++) {
    //     if(test(tests[i])) rep.passed++;
    // }

    // std::cout << "RSTest: " << rep.passed << "/" << rep.overall << " tests passed.\n\n";

    if (!RStest::test_encode_decode())
    {
        std::cout << "Encode-decode test failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Encode-decode test passed!\n"
                  << std::endl;
    }

    if (!RStest::test_encode_uint8())
    {
        std::cout << "Encode-decode uint8 test failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Encode-decode uint8 test passed!\n"
                  << std::endl;
    }

    if (!RStest::test_encode_uint8_large())
    {
        std::cout << "Encode-decode uint8 test large failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Encode-decode uint8 test large passed!\n"
                  << std::endl;
    }

    if (!RStest::test_short_data_using_buffer_for_long_data())
    {
        std::cout << "Short data using buffer for long data test failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Short data using buffer for long data test passed!\n"
                  << std::endl;
    }

    if (!RStest::test_encode_char_200())
    {
        std::cout << "Encode-decode test char with 200 Bytes failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Encode-decode test char with 200 Bytes passed!\n"
                  << std::endl;
    }

    if (!RStest::test_encode_char_250())
    {
        std::cout << "Encode-decode test char with 250 Bytes failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Encode-decode test char with 250 Bytes passed!\n"
                  << std::endl;
    }

    if (!RStest::test_float())
    {
        std::cout << "Encode-decode test float failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Encode-decode test float passed!\n"
                  << std::endl;
    }

    if (!RStest::test_single_error())
    {
        std::cout << "Single error test failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Single error test passed!\n"
                  << std::endl;
    }

    if (!RStest::test_half_parity_corrupted())
    {
        std::cout << "Half parity corrupted test failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Half parity corrupted test passed!\n"
                  << std::endl;
    }

    if (!RStest::test_really_short_data())
    {
        std::cout << "Really short data test failed!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Really short data test passed!\n"
                  << std::endl;
    }

    if (!RStest::test_custom_data_type()) {
        std::cout << "Custom data type test failed!\n"
                  << std::endl;
    } else {
        std::cout << "Custom data type test passed!\n"
                  << std::endl;
    }

    return rep;
}

bool RStest::test_encode(std::string &name)
{
    INIT_TESTCASE;
    RS::ReedSolomon<223, 32> rs;

    char message[223];
    char encoded[255];

    uint8_t right[255] = {0};

    for (int i = 0; i < 200; i++)
    {
        right[i] = i;
    }

    memcpy(message, right, 200);

    rs.Encode(message, encoded);

    for (int i = 0; i < 255; i++)
    {
        std::cout << encoded[i]; // why no output here??
    }
    std::cout << std::endl;

    SUBTEST(compare((uint8_t *)encoded, (uint8_t *)right, 200));

    rs.Decode(encoded, right);

    RETURN;
}

bool RStest::test_decode(std::string &name)
{
    INIT_TESTCASE;
    RS::ReedSolomon<223, 32> rs;

    char message[30];
    char right[30] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

    // Test clean message
    uint8_t clean[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 186, 143};
    SUBTEST(rs.Decode(clean, message) == 0);
    SUBTEST(compare(message, right, 30));

    // Test corrupted message
    uint8_t corrupted[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14, 0, 16, 17, 18, 19, 0, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 0, 143};

    SUBTEST(rs.Decode(corrupted, message) == 0);
    SUBTEST(compare(message, right, 30));

    RETURN
}

bool RStest::test_encode_uint8()
{
    std::cout << "test_encode_uint8" << std::endl;
    RS::ReedSolomon<223, 32> rs;

    uint8_t message[223] = {0};
    uint8_t encoded[255] = {0};

    uint8_t right[200] = {0};

    // Fill right with values from 0 to 199
    for (int i = 0; i < 200; i++)
    {
        right[i] = i % 10;
    }

    memcpy(message, right, 200);

    // Encode the message
    rs.Encode(message, encoded);

    // Print the encoded message
    std::cout << "Encoded message: ";
    for (int i = 0; i < 255; i++)
    {
        std::cout << static_cast<int>(encoded[i]) << " ";
    }
    std::cout << " end of encoded msg." << std::endl;

    memcmp(encoded, right, 200) != 0 ? std::cout << "Test failed" << std::endl : std::cout << "Test passed" << std::endl;

    encoded[5] = 10;
    encoded[6] = 10;
    encoded[7] = 11;
    encoded[8] = 11;

    uint8_t decoded[255] = {0};

    // Decode the message
    rs.Decode(encoded, decoded);

    return memcmp(decoded, right, 200) == 0;
}

bool RStest::test_encode_uint8_large()
{
    std::cout << "test_encode_uint8 with a large message" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    // Create a longer message
    const int total_message_length = 500;
    uint8_t message[total_message_length];
    for (int i = 0; i < total_message_length; i++)
    {
        message[i] = dis(gen);
    }

    const int block_size = 223;
    const int ecc_length = 32;
    const int total_blocks = (total_message_length + block_size - 1) / block_size; // "block_size - 1" is to handle the case where the message length is not a multiple of block_size,
    // to ensure that the remainder data is included in the last block. "-1" so that there are not too many blocks in case msg length a multiple of block size or smaller.
    uint8_t encoded[total_blocks][block_size + ecc_length];
    uint8_t decoded[total_message_length] = {0};

    RS::ReedSolomon<block_size, ecc_length> rs;
    for (int i = 0; i < total_blocks; i++)
    {
        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, total_message_length - start_idx);
        uint8_t block[block_size] = {0}; // Initialize to zero
        memcpy(block, message + start_idx, current_block_size);

        rs.Encode(block, encoded[i]);
    }

    // Corrupt some bytes in the encoded message
    for (int i = 0; i < ecc_length; i++)
    {
        if (i < ecc_length / 2)
        {
            encoded[0][i] ^= 1; // Huge data burst
        }
        else
        {
            encoded[total_blocks - 1][i] ^= 1;
        }
    }

    // Decode each block
    bool success = true;
    for (int i = 0; i < total_blocks; i++)
    {
        uint8_t block[block_size + ecc_length];
        memcpy(block, encoded[i], block_size + ecc_length);

        uint8_t decoded_block[block_size] = {0};
        rs.Decode(block, decoded_block);

        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, total_message_length - start_idx);
        memcpy(decoded + start_idx, decoded_block, current_block_size);

        if (memcmp(decoded + start_idx, message + start_idx, current_block_size) != 0)
        {
            success = false;
        }
    }

    return success;
}

bool RStest::test_short_data_using_buffer_for_long_data()
{
    std::cout << "test_short_data_using_buffer_for_long_data" << std::endl;
    const int block_size = 223;
    const int ecc_length = 32;
    const int message_length = 60;
    uint8_t message[message_length];
    const int total_blocks = (message_length + block_size - 1) / block_size;
    uint8_t encoded[total_blocks][block_size + ecc_length];
    uint8_t decoded[message_length] = {0};
    RS::ReedSolomon<block_size, ecc_length> rs;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < message_length; i++)
    {
        if (dis(gen) != 0)
        {
            message[i] = dis(gen);
        }
        else
        {
            message[i] = 1;
        }
    }

    for (int i = 0; i < total_blocks; i++)
    {
        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length - start_idx);
        uint8_t block[block_size] = {0};
        memcpy(block, message + start_idx, current_block_size);
        rs.Encode(block, encoded[i]);
    }

    std::cout << "Encoded message: ";
    for (int i = 0; i < total_blocks; i++)
    {
        for (int j = 0; j < block_size + ecc_length; j++)
        {
            std::cout << static_cast<int>(encoded[i][j]) << " ";
        }
    }
    std::cout << std::endl;

    encoded[0][0] = 0;
    encoded[0][1] = 0;
    encoded[0][2] = 0;
    encoded[0][3] = 0;
    encoded[0][4] = 0;
    encoded[0][5] = 0;
    encoded[0][6] = 0;
    encoded[0][7] = 0;
    encoded[0][8] = 0;
    encoded[0][9] = 0;
    encoded[0][10] = 0;
    encoded[0][11] = 0;
    encoded[0][12] = 0;
    encoded[0][13] = 0;
    encoded[0][14] = 0;
    encoded[0][15] = 0;

    bool success = true;

    for (int i = 0; i < total_blocks; i++)
    {
        uint8_t block[block_size + ecc_length];
        memcpy(block, encoded[i], block_size + ecc_length);

        uint8_t decoded_block[block_size] = {0};
        rs.Decode(block, decoded_block);

        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length - start_idx);
        memcpy(decoded + start_idx, decoded_block, current_block_size);

        if (memcmp(decoded + start_idx, message + start_idx, current_block_size) != 0)
        {
            success = false;
        }
    }

    return success;
}

bool RStest::test_encode_decode()
{
    char message[] = "Some very important message ought to be delivered";
    const int msglen = 50;
    const int ecclen = 32;

    char repaired[255];
    char encoded[255];

    RS::ReedSolomon<223, ecclen> rs;

    rs.Encode(message, encoded);

    // Corrupting first 8 bytes of message
    for (uint i = 0; i < ecclen / 4; i++)
    {
        encoded[i] = 'E';
    }

    rs.Decode(encoded, repaired);

    std::cout << "Original:  " << message << std::endl;
    std::cout << "Corrupted: " << encoded << std::endl;
    std::cout << "Repaired:  " << repaired << std::endl;

    return (memcmp(message, repaired, msglen) == 0);
}

bool RStest::test_encode_char_200()
{
    std::cout << "Test encode char 200 Bytes" << std::endl;
    const int block_size = 223;
    const int ecc_length = 32;
    const int message_length = 200;
    char message[message_length];
    const int total_blocks = (message_length + block_size - 1) / block_size;
    char encoded[total_blocks][block_size + ecc_length] = {0};
    char decoded[message_length] = {0};
    RS::ReedSolomon<block_size, ecc_length> rs;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < message_length; i++)
    {
        message[i] = static_cast<char>(dis(gen));
    }

    for (int i = 0; i < total_blocks; i++)
    {
        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length - start_idx);
        char block[block_size] = {0};
        memcpy(block, message + start_idx, current_block_size);
        rs.Encode(block, encoded[i]);
    }

    // Corrupt some bytes in the encoded message
    for (int i = 0; i < 16; i++)
    {
        encoded[0][i] = 0;
    }

    std::cout << std::endl;

    bool success = true;

    for (int i = 0; i < total_blocks; i++)
    {
        char block[block_size + ecc_length];
        memcpy(block, encoded[i], block_size + ecc_length);

        char decoded_block[block_size] = {0};
        rs.Decode(block, decoded_block);

        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length - start_idx);
        memcpy(decoded + start_idx, decoded_block, current_block_size);

        if (memcmp(decoded + start_idx, message + start_idx, current_block_size) != 0)
        {
            success = false;
        }
    }

    std::cout << "Decoded message: ";
    for (int i = 0; i < message_length; i++)
    {
        std::cout << static_cast<int>(decoded[i]);
    }
    std::cout << std::endl;

    return success;
}

bool RStest::test_encode_char_250()
{
    const int block_size = 223;
    const int ecc_length = 32;
    const int message_length = 250;
    char message[message_length];
    const int total_blocks = (message_length + block_size - 1) / block_size;
    char encoded[total_blocks][block_size + ecc_length] = {0};
    char decoded[message_length] = {0};
    RS::ReedSolomon<block_size, ecc_length> rs;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < message_length; i++)
    {
        message[i] = static_cast<char>(dis(gen));
    }

    for (int i = 0; i < total_blocks; i++)
    {
        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length - start_idx);
        char block[block_size] = {0};
        memcpy(block, message + start_idx, current_block_size); // std::copy
        rs.Encode(block, encoded[i]);
    }

    std::cout << std::endl;

    // Corrupt some bytes in the encoded message
    for (int i = 0; i < 25; i++)
    { // If 17 Bytes or more are corrupted in one block -> FAIL!
        if (i < 16)
        {
            encoded[0][i] = 0;
        }
        else
        {
            encoded[1][i - 16] = 0;
        }
    }

    std::cout << std::endl;

    bool success = true;

    for (int i = 0; i < total_blocks; i++)
    {
        char block[block_size + ecc_length];
        memcpy(block, encoded[i], block_size + ecc_length);

        char decoded_block[block_size] = {0};
        rs.Decode(block, decoded_block);

        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length - start_idx);
        memcpy(decoded + start_idx, decoded_block, current_block_size);

        if (memcmp(decoded + start_idx, message + start_idx, current_block_size) != 0)
        {
            success = false;
        }
    }

    return success;
}

bool RStest::test_float()
{
    std::cout << "test_float" << std::endl;

    const int block_size = 223;
    const int ecc_length = 32;
    const int message_length = 224 / 4;
    const int total_blocks = (message_length * 4 + block_size - 1) / block_size;

    float message[message_length];
    uint8_t encoded[total_blocks][block_size + ecc_length] = {0};
    uint8_t decoded[message_length * 4] = {0};

    RS::ReedSolomon<block_size, ecc_length> rs;

    // Fill the float array with some random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-100.0, 100.0);

    // Convert float array to a byte array for encoding
    uint8_t byte_message[message_length * sizeof(float)];
    for (int i = 0; i < message_length; i++)
    {
        message[i] = dis(gen);
        memcpy(byte_message + i * sizeof(float), &message[i], sizeof(float)); // Serialize to byte array
    }

    std::cout << "Original message: " << std::endl;
    for (int i = 0; i < message_length; i++)
    {
        std::cout << message[i];
    }
    std::cout << std::endl;

    // Step 1: Encode the message using Reed-Solomon
    for (int i = 0; i < total_blocks; i++)
    {
        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length * 4 - start_idx);
        uint8_t block[block_size] = {0};
        memcpy(block, byte_message + start_idx, current_block_size); // Copy byte data for encoding
        rs.Encode(block, encoded[i]);
    }

    for (int i = 0; i < 16; i++)
    {
        encoded[0][i] ^= 1;
    }

    bool success = true;
    for (int i = 0; i < total_blocks; i++)
    {
        uint8_t block[block_size + ecc_length];
        memcpy(block, encoded[i], block_size + ecc_length);

        uint8_t decoded_block[block_size] = {0};
        rs.Decode(block, decoded_block);

        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length * 4 - start_idx);
        memcpy(decoded + start_idx, decoded_block, current_block_size);

        if (memcmp(decoded + start_idx, byte_message + start_idx, current_block_size) != 0)
        {
            success = false;
        }
    }

    float decoded_floats[message_length] = {0};
    std::cout << "Decoded message: " << std::endl;
    for (int i = 0; i < message_length; i++)
    {
        memcpy(&decoded_floats[i], decoded + i * sizeof(float), sizeof(float));
        std::cout << decoded_floats[i];
    }
    std::cout << std::endl;

    return success;
}

bool RStest::test_custom_data_type()
{
    std::cout << "test custom data type with 1024 Bytes" << std::endl;

    const int message_length = 128;
    Custom custom_data[128];
    custom_data[0] = Custom(1, 2, 0.5);
    for (int i = 1; i < 128; i++)
    {
        custom_data[i] = Custom(i, i + 1, i / i + 1);
    }

    const int block_size = 223;
    const int ecc_length = 32;
    const int total_blocks = (message_length * 8 + block_size - 1) / block_size;
    uint8_t encoded[total_blocks][block_size + ecc_length] = {0};
    uint8_t decoded[message_length * sizeof(Custom)] = {0};

    RS::ReedSolomon<block_size, ecc_length> rs;

    uint8_t byte_message[message_length * sizeof(Custom)];
    for (int i = 0; i < message_length; i++)
    {
        memcpy(byte_message + i * sizeof(Custom), &custom_data[i], sizeof(Custom)); // Serialize to byte array
    }

    for (int i = 0; i < total_blocks; i++)
    {
        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length * 8 - start_idx);
        uint8_t block[block_size] = {0};
        memcpy(block, byte_message + start_idx, current_block_size); // Copy byte data for encoding
        rs.Encode(block, encoded[i]);
    }

    for (int i = 0; i < total_blocks; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            encoded[i][j] ^= 1;
        }
    }

    bool success = true;
    for (int i = 0; i < total_blocks; i++)
    {
        uint8_t block[block_size + ecc_length];
        memcpy(block, encoded[i], block_size + ecc_length);

        uint8_t decoded_block[block_size] = {0};
        rs.Decode(block, decoded_block);

        int start_idx = i * block_size;
        int current_block_size = std::min(block_size, message_length * 8 - start_idx);
        memcpy(decoded + start_idx, decoded_block, current_block_size);

        if (memcmp(decoded + start_idx, byte_message + start_idx, current_block_size) != 0)
        {
            success = false;
        }
    }

    return success;
}

bool RStest::test_single_error()
{
    uint8_t message[] = "102042018744466623571113171923";
    uint8_t encoded[223 + 32];
    uint8_t buffer_std[223];
    uint8_t buffer_crp[223];

    RS::ReedSolomon<223, 32> rs;
    rs.Encode(message, encoded);

    uint8_t corrupted[sizeof(encoded)];
    memcpy(corrupted, encoded, sizeof(encoded));
    corrupted[0] ^= 0xFF; // Introduce a single error

    rs.Decode(encoded, buffer_std);
    rs.Decode(corrupted, buffer_crp);

    return memcmp(message, buffer_std, sizeof(message)) == 0 &&
           memcmp(message, buffer_crp, sizeof(message)) == 0;
}

// adding one more corruption makes it fail, meaning 8 Bytes are the max. as expected!
bool RStest::test_half_parity_corrupted()
{
    uint8_t message[] = "102042018744466623571113171923";
    uint8_t encoded[223 + 32];
    uint8_t buffer_std[223];
    uint8_t buffer_crp[223];

    RS::ReedSolomon<223, 32> rs;
    rs.Encode(message, encoded);

    uint8_t corrupted[sizeof(encoded)];
    memcpy(corrupted, encoded, sizeof(encoded));
    for (int i = sizeof(message); i < sizeof(encoded); i += 2)
    {
        corrupted[i] ^= 0xFF; // Corrupt every other parity symbol
    }

    rs.Decode(encoded, buffer_std);
    rs.Decode(corrupted, buffer_crp);

    return memcmp(message, buffer_std, sizeof(message)) == 0 &&
           memcmp(message, buffer_crp, sizeof(message)) == 0;
}

void generate_random_message(uint8_t *message, size_t length)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (size_t i = 0; i < length; ++i)
    {
        message[i] = dis(gen);
    }
}

bool RStest::test_really_short_data()
{
    char msg[] = "Hello";
    char encoded[255];
    char buffer_std[223];

    RS::ReedSolomon<223, 32> rs;

    rs.Encode(msg, encoded);

    for (int i = 0; i < 5; i++)
    {
        std::cout << encoded[i];
    }
    std::cout << std::endl;

    encoded[0] = 'a';
    encoded[1] = 'a';
    encoded[2] = 'a';
    encoded[3] = 'a';
    encoded[4] = 'a';

    for (int i = 0; i < 5; i++)
    {
        std::cout << encoded[i];
    }
    std::cout << std::endl;

    rs.Decode(encoded, buffer_std);

    return memcmp(msg, buffer_std, sizeof(msg)) == 0;
}

bool RStest::test_stress(std::string &name)
{
    INIT_TESTCASE;

    SUBTEST(true);

    RETURN;
}
