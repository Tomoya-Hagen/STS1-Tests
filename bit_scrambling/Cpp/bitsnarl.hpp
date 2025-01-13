#pragma once
#include <array>
#include <valarray>
#include <vector>
#include <cstddef>

namespace bitsn {

static const std::uint8_t GF_table[256] = { //index (i) of an element (with value v) is the value of the v-th element in the inverse matrix
    0,   2,   4,   8,  16,  32,  64, 128,  29,  58, 116, 232, 205, 135,  19,  38,
   76, 152,  45,  90, 180, 117, 234, 201, 143,   3,   6,  12,  24,  48,  96, 192,
  157,  39,  78, 156,  37,  74, 148,  53, 106, 212, 181, 119, 238, 193, 159,  35,
   70, 140,   5,  10,  20,  40,  80, 160,  93, 186, 105, 210, 185, 111, 222, 161,
   95, 190,  97, 194, 153,  47,  94, 188, 101, 202, 137,  15,  30,  60, 120, 240,
  253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163,  91, 182, 113, 226,
  217, 175,  67, 134,  17,  34,  68, 136,  13,  26,  52, 104, 208, 189, 103, 206,
  129,  31,  62, 124, 248, 237, 199, 147,  59, 118, 236, 197, 151,  51, 102, 204,
  133,  23,  46,  92, 184, 109, 218, 169,  79, 158,  33,  66, 132,  21,  42,  84,
  168,  77, 154,  41,  82, 164,  85, 170,  73, 146,  57, 114, 228, 213, 183, 115,
  230, 209, 191,  99, 198, 145,  63, 126, 252, 229, 215, 179, 123, 246, 241, 255,
  227, 219, 171,  75, 150,  49,  98, 196, 149,  55, 110, 220, 165,  87, 174,  65,
  130,  25,  50, 100, 200, 141,   7,  14,  28,  56, 112, 224, 221, 167,  83, 166,
   81, 162,  89, 178, 121, 242, 249, 239, 195, 155,  43,  86, 172,  69, 138,   9,
   18,  36,  72, 144,  61, 122, 244, 245, 247, 243, 251, 235, 203, 139,  11,  22,
   44,  88, 176, 125, 250, 233, 207, 131,  27,  54, 108, 216, 173,  71, 142,   1
};

template <typename T>
struct is_array_of_uint8 {
    enum { value = false };
};

template <>
struct is_array_of_uint8<std::vector<std::uint8_t>> {
    enum { value = true };
};

template <>
struct is_array_of_uint8<std::valarray<std::uint8_t>> {
    enum { value = true };
};

template <std::size_t N>
struct is_array_of_uint8<std::array<std::uint8_t, N>> {
    enum { value = true };
};

template <
    typename T,
    typename std::enable_if<is_array_of_uint8<T>::value, std::nullptr_t>::type = nullptr
>
void scramble(T &bytes) {
    for (size_t i = 0; i < bytes.size(); i++) {
        bytes[i] ^= GF_table[i % 255];
    }
}


template <
    typename T,
    typename std::enable_if<is_array_of_uint8<T>::value, std::nullptr_t>::type = nullptr
>
void un_scramble(T &bytes) {
    scramble(bytes);
}

}
