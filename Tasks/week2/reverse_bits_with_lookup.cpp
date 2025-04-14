// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <random>
#include <iostream>

using namespace std;

// Lookup table to get a reversed byte. For example you want to know the
// reversed byte of "2" (00000010), you find it out by executing lookup[2]
// --> 64 (01000000)
const uint32_t lookup[256] = {
    0,  128, 64, 192, 32, 160, 96,  224, 16, 144, 80, 208, 48, 176, 112, 240,
    8,  136, 72, 200, 40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248,
    4,  132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 52, 180, 116, 244,
    12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220, 60, 188, 124, 252,
    2,  130, 66, 194, 34, 162, 98,  226, 18, 146, 82, 210, 50, 178, 114, 242,
    10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250,
    6,  134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246,
    14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254,
    1,  129, 65, 193, 33, 161, 97,  225, 17, 145, 81, 209, 49, 177, 113, 241,
    9,  137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
    5,  133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245,
    13, 141, 77, 205, 45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253,
    3,  131, 67, 195, 35, 163, 99,  227, 19, 147, 83, 211, 51, 179, 115, 243,
    11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219, 59, 187, 123, 251,
    7,  135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
    15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255};

/************** begin assignment **************/
// Write a program that takes a 32-bit unsigned integer and returns the 32-bit
// unsigned integer consisting of the bits of the input number in reverse order.
// For example, if the input is alternating 1s and Os, i.e., (1010 ... 10), the
// output should be alternating Os and 1s, i.e., (0101 ... 01).
// Use a lookup table to reverse individual bytes of the unsigned integer.

// A simple solution to reverse the bits in a 32-bit unsigned integer (no lookup table)
inline uint32_t reverse_simple(uint32_t num) {
  uint32_t result = 0;
  for (uint32_t offset = 31; num; num >>= 1u) {
    // extract least significant bit and left shift by offset
    // update result
    result |= (num & 1u) << offset--;
  }
  return result;
}

// TODO: Use the lookup table at line 18 to reverse the bits of an unsigned 32-bit integer.
inline uint32_t reverse_with_lookup(uint32_t num) {
  uint32_t result = 0;

  // for each byte
  for (uint32_t i = 0; i < 4; i++) {
    // 1. shift the byte to the rightmost position
    // 2. mask the byte with 0xff to get the last 8 bits
    // 3. use the lookup table to get the reversed byte
    // 4. shift the reversed byte to the correct position
    result |= lookup[(num >> (i * 8)) & 0xff] << (24 - i * 8);
  }
  
  return result;
}

/*************** end assignment ***************/

int main() {
  int n = 1000;
  auto rand_uint32 = bind(uniform_int_distribution<uint32_t>{0, UINT32_MAX},
                          default_random_engine{std::random_device{}()});
  // test correctness
  for (int i = 0; i < n; ++i) {
    // generate a random unsigned integer
    uint32_t num = rand_uint32();
    // test if lookup version produces the correct results
    assert(reverse_simple(num) == reverse_with_lookup(num));
  }
  cout << "all tests passed" << endl;
}
