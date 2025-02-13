#pragma once

#include "../viterbi/viterbi.h"

auto codec = ViterbiCodec();

void test_encode_decode_short();

void test_encode_decode_long();

void test_encode_decode_insert_error_success();

void test_encode_decode_insert_error_fail();

void test_basis_conversion();
