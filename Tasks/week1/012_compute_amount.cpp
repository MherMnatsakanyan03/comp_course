// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <chrono>
#include <iostream>

using namespace std;

// helper macros for timing
#define TIMERSTART(label)                                                  \
    std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
    a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                              \
    b##label = std::chrono::system_clock::now();                      \
    std::chrono::duration<double> delta##label = b##label - a##label; \
    std::cout << #label << ": " << delta##label.count() << " seconds" \
              << std::endl;

/************** begin assignment **************/
// compute the amount of all positive integer solutions to the equation
// a^3 + b^3 = c^3 + d^3 where a, b, c, and d are integers between 1 and n
// (1 <= n <= 500)

// an obvious brute force solution will just have four nested for loops
// this algorithm iterates through all possible values of a, b, c, and d
// and checks if that combination happens to work --> runtime is O(n^4)
int compute_amount_brute_force(const int n)
{
    int amount = 0;
    for (int a = 1; a <= n; ++a)
    {
        for (int b = 1; b <= n; ++b)
        {
            for (int c = 1; c <= n; ++c)
            {
                for (int d = 1; d <= n; ++d)
                {
                    if (a * a * a + b * b * b == c * c * c + d * d * d)
                    {
                        ++amount;
                    }
                }
            }
        }
    }
    return amount;
}

#include <map>
#include <unordered_map>
// try to improve the time complexity
// (if you can't, try at least to improve the execution time)
int compute_amount(const int n)
{
    int amount = 0;

    // create a map of all possible values of a^3 + b^3
    std::unordered_map<int, int> cube_sum_map;
    for (int a = 1; a <= n; ++a)
    {
        for (int b = 1; b <= n; ++b)
        {
            int cube_sum = a * a * a + b * b * b;
            // Idea: if a^3 + b^3 = c^3 + d^3, then we can just add the number of times
            // we have seen this sum before to the amount, then we only need to multiply
            // the number of times we have seen this sum with itself to get all possible
            // combinations of a, b, c, and d that satisfy the equation (since the amout
            // is equal to that of c^3 + d^3)
            cube_sum_map[cube_sum]++;
        }
    }

    // no need to calculate the cube sum again, so just return sum of squares
    for (auto &cube_sum : cube_sum_map)
    {
        amount += cube_sum.second * cube_sum.second;
    }

    return amount;
}
/*************** end assignment ***************/

int main()
{
    // test correctness of implementation
    for (int n = 1; n < 100; ++n)
    {
        assert(compute_amount_brute_force(n) == compute_amount(n));
    }

    // compare execution times for n = 100
    int n = 200;
    cout << "n = " << n << endl;
    TIMERSTART(brute_force)
    int result_bf = compute_amount_brute_force(n);
    TIMERSTOP(brute_force)

    TIMERSTART(improved)
    int result = compute_amount(n);
    TIMERSTOP(improved)

    assert(result_bf == result);
}
