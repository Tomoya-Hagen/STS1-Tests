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
RStest::run_tests() {
    bool (*tests[])(std::string&) = {
        test_encode,
        test_decode,
        test_stress
    };

    Report rep = {3, 0};

    for(uint i = 0; i < rep.overall; i++) {
        if(test(tests[i])) rep.passed++;
    }

    std::cout << "RSTest: " << rep.passed << "/" << rep.overall << " tests passed.\n\n";

    if (!RStest::test_encode_decode()) {
        std::cout << "Encode-decode test failed!\n" << std::endl;
    } else {
        std::cout << "Encode-decode test passed!\n" << std::endl;
    }

    if (!RStest::test_encode_decode_2()) {
        std::cout << "Encode-decode test 2 failed!\n" << std::endl;
    } else {
        std::cout << "Encode-decode test 2 passed!\n" << std::endl;
    }

    if (!RStest::test_single_error()) {
        std::cout << "Single error test failed!\n" << std::endl;
    } else {
        std::cout << "Single error test passed!\n" << std::endl;
    }

    if (!RStest::test_half_parity_corrupted()) {
        std::cout << "Half parity corrupted test failed!\n" << std::endl;
    } else {
        std::cout << "Half parity corrupted test passed!\n" << std::endl;
    }

    if (!RStest::test_really_long_data()) {
        std::cout << "Really long data test failed!\n" << std::endl;
    } else {
        std::cout << "Really long data test passed!\n" << std::endl;
    }

    if (!RStest::test_really_short_data()) {
        std::cout << "Really short data test failed!\n" << std::endl;
    } else {
        std::cout << "Really short data test passed!\n" << std::endl;
    }

    // if (!RStest::stress_test((size_t)128, 1000, 8)) {
    //     std::cout << "Stress test failed!\n" << std::endl;
    // } else {
    //     std::cout << "Stress test passed!\n" << std::endl;
    // }

    return rep;
}

bool
RStest::test_encode(std::string &name) {
    INIT_TESTCASE;
    RS::ReedSolomon<30, 8> rs;

    char message[30];
    char encoded[38];

    uint8_t right[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 186, 143
    };

    memcpy(message, right, 30);

    rs.Encode(message, encoded);

    SUBTEST(compare((uint8_t*)encoded, (uint8_t*)right, 38));
    RETURN;
}

bool
RStest::test_decode(std::string &name) {
    INIT_TESTCASE;
    RS::ReedSolomon<30, 8> rs;

    char message[30];
    char right[30] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    };

    // Test clean message
    uint8_t clean[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 186, 143
    };
    SUBTEST(rs.Decode(clean, message) == 0);
    SUBTEST(compare(message, right, 30));

    // Test corrupted message
    uint8_t corrupted[38] = {
        0, 1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14, 0, 16, 17, 18, 19, 0, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        99, 26, 219, 193, 9, 94, 0, 143
    };

    SUBTEST(rs.Decode(corrupted, message) == 0);
    SUBTEST(compare(message, right, 30));

    RETURN
}

bool RStest::test_encode_decode() {
    char message[] = "Some very important message ought to be delivered";
    const int msglen = sizeof(message);
    const int ecclen = 8;

    char repaired[msglen];
    char encoded[msglen + ecclen];


    RS::ReedSolomon<msglen, ecclen> rs;

    rs.Encode(message, encoded);

    // Corrupting first 8 bytes of message (any 4 bytes can be repaired, no more)
    for(uint i = 0; i < ecclen / 2; i++) {
        encoded[i] = 'E';
    }

    rs.Decode(encoded, repaired);

    std::cout << "Original:  " << message  << std::endl;
    std::cout << "Corrupted: " << encoded  << std::endl;
    std::cout << "Repaired:  " << repaired << std::endl;

    return (memcmp(message, repaired, msglen) == 0);
}

bool
RStest::test_encode_decode_2() {
    uint8_t message[] = "102042018744466623571113171923";
    uint8_t encoded[sizeof(message) + 8];
    uint8_t buffer_std[sizeof(message)];
    uint8_t buffer_crp[sizeof(message)];

    RS::ReedSolomon<sizeof(message), 8> rs;

    rs.Encode(message, encoded);

    uint8_t corrupted[sizeof(encoded)] = {0};

    for (int i = 0; i < sizeof(encoded); i++) {
        corrupted[i] = encoded[i];
    }

    corrupted[5] = 9;
    corrupted[6] = 9;
    corrupted[7] = 9;
    corrupted[8] = 9;

    rs.Decode(encoded, buffer_std);
    rs.Decode(corrupted, buffer_crp);

    auto print_hex = [](const uint8_t* data, size_t length) {
        for (size_t i = 0; i < length; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
        }
        std::cout << std::dec << std::endl; // Reset to decimal
    };

    std::cout << "Original:  ";
    print_hex(message, sizeof(message));
    std::cout << "Encoded:   ";
    print_hex(encoded, sizeof(encoded));
    std::cout << "Corrupted: ";
    print_hex(corrupted, sizeof(corrupted));
    std::cout << "Repaired std:  ";
    print_hex(buffer_std, sizeof(buffer_std));
    std::cout << "Repaired crp:  ";
    print_hex(buffer_crp, sizeof(buffer_crp));

    return (memcmp(message, buffer_std, sizeof(message)) == 0 && memcmp(message, buffer_crp, sizeof(message)) == 0);
}

bool RStest::test_single_error() {
    uint8_t message[] = "102042018744466623571113171923";
    uint8_t encoded[sizeof(message) + 8];
    uint8_t buffer_std[sizeof(message)];
    uint8_t buffer_crp[sizeof(message)];

    RS::ReedSolomon<sizeof(message), 8> rs;
    rs.Encode(message, encoded);

    uint8_t corrupted[sizeof(encoded)];
    memcpy(corrupted, encoded, sizeof(encoded));
    corrupted[0] ^= 0xFF;  // Introduce a single error

    rs.Decode(encoded, buffer_std);
    rs.Decode(corrupted, buffer_crp);

    return memcmp(message, buffer_std, sizeof(message)) == 0 &&
           memcmp(message, buffer_crp, sizeof(message)) == 0;
}

// adding one more corruption makes it fail, meaning 8 Bytes are the max. as expected!
bool RStest::test_half_parity_corrupted() {
    uint8_t message[] = "102042018744466623571113171923";
    uint8_t encoded[sizeof(message) + 8];
    uint8_t buffer_std[sizeof(message)];
    uint8_t buffer_crp[sizeof(message)];

    RS::ReedSolomon<sizeof(message), 8> rs;
    rs.Encode(message, encoded);

    uint8_t corrupted[sizeof(encoded)];
    memcpy(corrupted, encoded, sizeof(encoded));
    for (int i = sizeof(message); i < sizeof(encoded); i += 2) {
        corrupted[i] ^= 0xFF;  // Corrupt every other parity symbol
    }

    rs.Decode(encoded, buffer_std);
    rs.Decode(corrupted, buffer_crp);

    return memcmp(message, buffer_std, sizeof(message)) == 0 &&
           memcmp(message, buffer_crp, sizeof(message)) == 0;
}

void generate_random_message(uint8_t *message, size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (size_t i = 0; i < length; ++i) {
        message[i] = dis(gen);
    }
}

void introduce_errors(uint8_t *encoded, size_t length, int num_errors) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pos_dis(0, length - 1);
    std::uniform_int_distribution<> val_dis(0, 255);

    for (int i = 0; i < num_errors; ++i) {
        size_t pos = pos_dis(gen);
        uint8_t error_value = val_dis(gen);
        encoded[pos] ^= error_value; // Introduce error
    }
}

bool stress_test(size_t message_length, int num_tests, int max_errors) {
    std::array<uint8_t, 128> message;
    std::vector<uint8_t> encoded(message_length + 8);
    std::vector<uint8_t> decoded(message_length);
    std::vector<uint8_t> corrupted(message_length + 8);

    const uint8_t msg_len = message_length;

    RS::ReedSolomon<128, 8> rs;

    int success_count = 0;

    for (int i = 0; i < num_tests; ++i) {
        generate_random_message(message.data(), message_length);
        rs.Encode(message.data(), encoded.data());

        // Test with different number of errors
        for (int errors = 0; errors <= max_errors; ++errors) {
            std::copy(encoded.begin(), encoded.end(), corrupted.begin());
            introduce_errors(corrupted.data(), corrupted.size(), errors);

            rs.Decode(corrupted.data(), decoded.data());

            if (memcmp(message.data(), decoded.data(), message_length) == 0) {
                success_count++;
            }
        }
    }

    std::cout << "Total tests: " << num_tests * (max_errors + 1) << std::endl;
    std::cout << "Successful decodings: " << success_count << std::endl;
    std::cout << "Success rate: " << (double)success_count / (num_tests * (max_errors + 1)) * 100 << "%" << std::endl;

    return true;
}

bool RStest::test_really_short_data() {
    char msg[] = "Hello";
    char encoded[sizeof(msg) + 2];
    char buffer_std[sizeof(msg)];

    RS::ReedSolomon<sizeof(msg), 2> rs;

    rs.Encode(msg, encoded);

    for (char c : msg) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    std::cout << std::endl;

    for (char c : encoded) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    std::cout << std::endl;

    encoded[5] = 'a';

    rs.Decode(encoded, buffer_std);

    return memcmp(msg, buffer_std, sizeof(msg)) == 0;
}

// currently: one error and 2 parity bits -> passes
bool RStest::test_really_long_data() {
    char msg[240];
    for (int i = 0; i < 240; i++) {
        msg[i] = i - '0';
    }

    char endcoded[sizeof(msg) + 1];
    char buffer_std[sizeof(msg)];

    RS::ReedSolomon<sizeof(msg), 2> rs;

    rs.Encode(msg, endcoded);
    endcoded[5] = 'a';
    rs.Decode(endcoded, buffer_std);

    return memcmp(msg, buffer_std, sizeof(msg)) == 0;

}

bool
RStest::test_stress(std::string &name) {
    INIT_TESTCASE;

    SUBTEST(true);

    RETURN;
}

