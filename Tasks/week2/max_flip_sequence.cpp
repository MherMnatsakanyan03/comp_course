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

// obfuscated code to test your solution (ignore this)
uint32_t obfuscated(uint32_t o_abab67357a45d416943cc4921e785b98)
{
    if (o_abab67357a45d416943cc4921e785b98 == UINT32_MAX)
    {
        return (0x0000000000000040u + 0x0000000000000220u + 0x0000000000000820u -
                0x0000000000000A60u);
    }
    uint32_t o_0eb3e51cabf50b3d1a61f2d180a35aae =
        (0x0000000000000000u + 0x0000000000000200u + 0x0000000000000800u -
         0x0000000000000A00u);
    uint32_t o_f5042e0723caca6ff1c0cace5d4569f4 =
        (0x0000000000000000u + 0x0000000000000200u + 0x0000000000000800u -
         0x0000000000000A00u);
    uint32_t o_c37bf56dd800ab9798b3f82e74a0f6aa =
        (0x0000000000000000u + 0x0000000000000200u + 0x0000000000000800u -
         0x0000000000000A00u);
    while (o_abab67357a45d416943cc4921e785b98)
    {
        uint32_t o_be7578e6b3e06edeb987145645ba05ea =
            o_abab67357a45d416943cc4921e785b98 &
            (0x0000000000000002u + 0x0000000000000201u + 0x0000000000000801u -
             0x0000000000000A03u);
        o_abab67357a45d416943cc4921e785b98 >>=
            (0x0000000000000002u + 0x0000000000000201u + 0x0000000000000801u -
             0x0000000000000A03u);
        if (o_be7578e6b3e06edeb987145645ba05ea)
        {
            o_f5042e0723caca6ff1c0cace5d4569f4++;
        }
        else if ((o_abab67357a45d416943cc4921e785b98 &
                  (0x0000000000000002u + 0x0000000000000201u +
                   0x0000000000000801u - 0x0000000000000A03u)) ==
                 (0x0000000000000000u + 0x0000000000000200u +
                  0x0000000000000800u - 0x0000000000000A00u))
        {
            o_c37bf56dd800ab9798b3f82e74a0f6aa =
                (0x0000000000000000u + 0x0000000000000200u + 0x0000000000000800u -
                 0x0000000000000A00u);
            o_f5042e0723caca6ff1c0cace5d4569f4 =
                (0x0000000000000000u + 0x0000000000000200u + 0x0000000000000800u -
                 0x0000000000000A00u);
        }
        else
        {
            o_c37bf56dd800ab9798b3f82e74a0f6aa = o_f5042e0723caca6ff1c0cace5d4569f4;
            o_f5042e0723caca6ff1c0cace5d4569f4 =
                (0x0000000000000000u + 0x0000000000000200u + 0x0000000000000800u -
                 0x0000000000000A00u);
        }
        o_0eb3e51cabf50b3d1a61f2d180a35aae =
            std::max(o_0eb3e51cabf50b3d1a61f2d180a35aae,
                     o_f5042e0723caca6ff1c0cace5d4569f4 +
                         o_c37bf56dd800ab9798b3f82e74a0f6aa);
    }
    return o_0eb3e51cabf50b3d1a61f2d180a35aae +
           (0x0000000000000002u + 0x0000000000000201u + 0x0000000000000801u -
            0x0000000000000A03u);
}

/************** begin assignment **************/
// You have an integer and you can flip exactly one bit from a 0 to a 1. Write
// code to find the length of the longest sequence of 1s you could create.
// Actually, that's a hard one!
// No complexity requirements, as long as you solve it somehow.
// If you fail that's ok. You should at least try.
// Optimal: O(b) time, O(1) space, b is the number of bits in the integer.

// EXAMPLE
// Input: 1775 (or: 11011101111)
// Output: 8

// Hints:
// - Start with a brute force solution. Can you try all possibilities?
// - Flipping a O to a 1 can merge two sequences of 1s --- but only if the two
//   sequences are separated by only one 0.
// - Each sequence can be lengthened by merging it with an adjacent sequence
//   (if any) or just flipping the immediate neighboring zero. You just need to
//   find the best choice.

uint32_t max_flip_sequence_size(uint32_t n)
{

    // if n is all 1s, return 32
    if (n == UINT32_MAX)
    {
        return 32;
    }

    // initialize variables
    uint32_t max_length = 0;

    // iterate through the bits of n
    for (uint32_t i = 0; i < 32 - max_length; i++)
    {
        uint32_t current_length = 0;
        bool flipped = false;

        // for the following bits
        for (uint32_t j = i; j < 32; j++)
        {
            // if the bit is 1, increment the current length
            if ((n & (1 << j)) != 0)
            {
                current_length++;
            }
            // if the bit is 0, flip it and increment the current length
            else if (!flipped)
            {
                flipped = true;
                current_length++;
            }
            // if the bit is 0 and we have already flipped a bit, break
            else
            {
                break;
            }
        }

        // update the max length
        max_length = max(max_length, current_length);
    }

    // return the max length
    return max_length;
}
/*************** end assignment ***************/

int main()
{
    // test correctness
    assert(max_flip_sequence_size(UINT32_MAX) == 32);
    assert(max_flip_sequence_size(0) == 1);
    assert(max_flip_sequence_size(1) == 2);
    assert(max_flip_sequence_size(2) == 2);
    assert(max_flip_sequence_size(UINT32_MAX - 1) == 32);
    assert(max_flip_sequence_size(UINT32_MAX << 2u) == 31);
    int n = 100000;
    auto rand_uint32 = bind(uniform_int_distribution<uint32_t>{0, UINT32_MAX},
                            default_random_engine{std::random_device{}()});
    for (int i = 0; i < n; ++i)
    {
        // generate a random unsigned integer
        uint32_t num = rand_uint32();
        // test if your solution is correct
        assert(max_flip_sequence_size(num) == obfuscated(num));
    }
    cout << "all tests passed" << endl;
}
