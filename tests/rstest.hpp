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

    static bool test_encode(std::string& name);
    static bool test_decode(std::string& name);
    static bool test_stress(std::string& name);
    static bool test_encode_decode();
    static bool test_single_error();
    static bool test_half_parity_corrupted();
    static bool test_encode_decode_2();
    static bool stress_test(size_t message_length, int num_tests, int max_errors);
    static bool test_really_short_data();
    static bool test_really_long_data();
};

#endif // RSTEST_H
