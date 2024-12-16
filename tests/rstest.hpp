/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#ifndef RSTEST_H
#define RSTEST_H
#include "testsuite.hpp"
#include <rs.hpp>

class RStest : public TestSuite
{
public:
    static Report run_tests();

    struct Custom { // one object costs 8 Bytes (after a and b, there happens a padding of 2 Bytes, since sizeof(float) is 4 Bytes)
        uint8_t a;
        uint8_t b;
        float c;

    public:
        Custom() : a(0), b(0), c(0) {};
        Custom(uint8_t a, uint8_t b, float c) : a(a), b(b), c(c) {};
    };

    static bool test_encode(std::string &name);
    static bool test_decode(std::string &name);
    static bool test_stress(std::string &name);
    static bool test_encode_decode();
    static bool test_single_error();
    static bool test_half_parity_corrupted();
    static bool test_encode_decode_2();
    static bool stress_test(size_t message_length, int num_tests, int max_errors);
    static bool test_really_short_data();
    static bool test_encode_uint8();
    static bool test_encode_uint8_large();
    static bool test_short_data_using_buffer_for_long_data();
    static bool test_encode_char_200();
    static bool test_encode_char_250();
    static bool test_float();
    static bool test_custom_data_type();
};

#endif // RSTEST_H
