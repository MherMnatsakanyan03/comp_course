// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// helper macros for timing
#define TIMERSTART(label)                                                      \
  std::chrono::time_point<std::chrono::system_clock> a##label, b##label;       \
  a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                                       \
  b##label = std::chrono::system_clock::now();                                 \
  std::chrono::duration<double> delta##label = b##label - a##label;            \
  std::cout << #label << ": " << delta##label.count() << " seconds"            \
            << std::endl;

// macro simulates a COEX module
#define COEX(a, b)                                                             \
  if (a > b) {                                                                 \
    int c = a;                                                                 \
    a = b;                                                                     \
    b = c;                                                                     \
  }

/************** begin assignment **************/
// Implement Green's sorting network to sort 16 integers.
// See slide "Interesting Facts About Sorting Networks" (slide 11) of the
// lecture or https://www.hanshq.net/knuth-cheque.html.
// Use the COEX Macro (line 28) that represents a single compare-and-exchange
// module of a sorting network.

// sorting network to sort 16 integers
inline void sort_16_int(int *arr) {
  // first layer
  for(int i = 0; i < 16; i += 2) {
    COEX(arr[i], arr[i+1]);
  }

  // second layer
  for(int i = 0; i < 16; i += 4) {
    COEX(arr[i], arr[i+2]);
    COEX(arr[i+1], arr[i+3]);
  }

  // third layer
  for(int i = 0; i < 16; i += 8) {
    COEX(arr[i], arr[i+4]);
    COEX(arr[i+1], arr[i+5]);
    COEX(arr[i+2], arr[i+6]);
    COEX(arr[i+3], arr[i+7]);
  }

  // fourth layer
  for(int i = 0; i < 8; i++) {
    COEX(arr[i], arr[i+8]);
  }

  // fifth layer - from here, pattern could not be seen // 28 COEX
  COEX(arr[5], arr[10]);

  COEX(arr[6], arr[9]);

  COEX(arr[3], arr[12]);

  COEX(arr[7], arr[11]);
  COEX(arr[13], arr[14]);

  COEX(arr[1], arr[2]);
  COEX(arr[4], arr[8]);

  COEX(arr[1], arr[4]);
  COEX(arr[7], arr[13]);

  COEX(arr[2], arr[8]);
  COEX(arr[11], arr[14]);

  COEX(arr[2], arr[4]);
  COEX(arr[5], arr[6]);
  COEX(arr[9], arr[10]);
  COEX(arr[11], arr[13]);

  COEX(arr[3], arr[8]);

  COEX(arr[7], arr[12]);

  COEX(arr[6], arr[8]);

  COEX(arr[3], arr[5]);
  COEX(arr[7], arr[9]);
  COEX(arr[10], arr[12]);

  COEX(arr[3], arr[4]);
  COEX(arr[5], arr[6]);
  COEX(arr[7], arr[8]);
  COEX(arr[9], arr[10]);
  COEX(arr[11], arr[12]);
  
  COEX(arr[6], arr[7]);
  COEX(arr[8], arr[9]);
}
/*************** end assignment ***************/

int main() {
  // test correctness with zero-one principle
  // http://www.euroinformatica.ro/documentation/programming/!!!Algorithms_CORMEN!!!/DDU0170.html
  {
    vector<int> a(16);
    vector<int> b(16);
    for (uint32_t i = 0; i <= UINT16_MAX; ++i) {
      uint32_t num = i;
      for (int j = 0; j < 16; ++j) {
        a[j] = num & 1u;
        num >>= 1u;
      }
      b = a;
      sort_16_int(a.data());
      sort(begin(b), end(b));
      assert(a == b);
    }
  }
  int n = 16;

  // create a vector with random ints
  vector<int> v1(n);
  auto rand_int = bind(uniform_int_distribution<int>{INT32_MIN, INT32_MAX},
                       default_random_engine{std::random_device{}()});
  for (int i = 0; i < n; ++i) {
    v1[i] = rand_int();
  }
  auto v2 = v1;
  auto v_copy = v1;

  int repetitions = 100000000;
  TIMERSTART(std_sort)
  for (int i = 0; i < repetitions; ++i) {
    v1 = v_copy;
    sort(begin(v1), end(v1));
  }
  TIMERSTOP(std_sort)

  TIMERSTART(greens_sorting_network)
  for (int i = 0; i < repetitions; ++i) {
    v2 = v_copy; // copy time is here significantly, 1/5 of the total runtime!
    sort_16_int(v2.data());
  }
  TIMERSTOP(greens_sorting_network)

  assert(v1 == v2);
}
