// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../helper.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>

#define ASSERT_EQUAL(actual, expected) Helper::assertEqual<size_t>(actual, expected, __FILE__, __LINE__)

using namespace std;

/************** begin assignment **************/
// The input is an array of n unsigned integers, where A[i] denotes the maximum
// you can advance from index i.
// Write a function to compute the minimum number
// of steps needed to advance beyond the last index of an array. If it's
// impossible to go beyond the last index of an array, your function should
// return 0.
// Optimal: O(n) time, O(1) space

// EXAMPLES
// Input: < 4, 1, 2, 3, 0, 2, 0 >
// Output: 3

// Input: < 4, 1, 2, 3, 0, 1, 0 >
// Output: 0

size_t min_steps_beyond_last_index(const vector<size_t> &A)
{
  size_t n = A.size();

  if (n == 0 || A[0] == 0)
    return 0;

  size_t steps = 1;

  size_t i = 0;
  while ( i < n)
  {
    size_t current_possible_steps = A[i];
    size_t remaining_steps = n - i;

    if (current_possible_steps >= remaining_steps)
      break;

    size_t current_best_jump = 0;
    size_t current_best_jump_index = 0;
    for (size_t j = i; j <= current_possible_steps + i; j++)
    {
      size_t current_jump = j + A[j];
      if (current_jump > current_best_jump)
      {
        current_best_jump = current_jump;
        current_best_jump_index = j;
      }
    }

    size_t old_i = i;
    i = current_best_jump_index;

    if (i == old_i)
      return 0;

    steps++;
  }

  return steps;
}
/*************** end assignment ***************/

int main()
{
  {
    vector<size_t> v;
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 0);
  }
  {
    vector<size_t> v = {1};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 1);
  }
  {
    vector<size_t> v = {0};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 0);
  }
  {
    vector<size_t> v = {3, 3, 1, 0, 2, 0, 1};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 4);
  }
  {
    vector<size_t> v = {3, 3, 1, 0, 2, 0, 0};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 0);
  }
  {
    vector<size_t> v = {3, 2, 0, 0, 2, 0, 1};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 0);
  }
  {
    vector<size_t> v = {2, 4, 1, 1, 0, 2, 3};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 3);
  }
  {
    vector<size_t> v = {2, 1, 1, 3, 0, 2, 0};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 4);
  }
  {
    vector<size_t> v = {4, 1, 2, 3, 0, 1, 0};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 0);
  }
  {
    vector<size_t> v = {4, 1, 2, 3, 0, 2, 0};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 3);
  }
  {
    vector<size_t> v = {1, 1, 5, 1, 1, 1, 1, 4, 1, 1, 1};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 4);
  }
  {
    vector<size_t> v = {3, 0, 0, 4, 1, 1, 1, 4, 1, 1, 1};
    ASSERT_EQUAL(min_steps_beyond_last_index(v), 3);
  }
  cout << "all tests passed" << endl;
}
