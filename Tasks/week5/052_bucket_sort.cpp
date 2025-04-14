// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <iostream>
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

struct Employee {
  int id;  // employee id (not important for us ...)
  int age; // all people are between 18 <= years <= 70 (evenly distributed)

  // for testing equality
  bool operator==(const Employee &rhs) const {
    return id == rhs.id && age == rhs.age;
  }
  bool operator!=(const Employee &rhs) const { return !(rhs == *this); }
};

// to compare with your implementation we use std::stable_sort
void sort_with_std_stable(vector<Employee> &employees) {
  // stable sort --> order of items in output vector don't change if age is
  // equal, needs O(n) extra space since it's a kind of Mergesort
  stable_sort(begin(employees), end(employees),
              [](const Employee &e1, const Employee &e2) {
                return e1.age < e2.age; // sort by age in increasing order
              });
}

vector<Employee> generate_random_vector(int n) {
  vector<Employee> employees;
  employees.reserve(n);
  for (int i = 0; i < n; ++i) {
    employees.push_back({i, rand() % (71 - 18) + 18}); // not really random!
  }
  return employees;
}

/************** begin assignment **************/
// Your task is to sort a vector of employees by age in ascending order.
// Implement a stable sort. A sorting algorithm is stable if
// whenever there are two records R and S with the same key and with R appearing
// before S in the original array, R will appear before S in the sorted array.
// Actually bucket sort is stable, so you don't need to worry about that ;).
//
// An employee is always between 18 and 70 years (inclusive) old.
// See line 26 to familiarise yourself with the Employee struct.
//
// Try to accomplish the sort in O(n) time. You can use extra space.
//
// It's "quite easy" to parallelize this kind of sort ... If you feel confident,
// try to parallelize your implementation. (optional)
// It's ok if you can't' parallelize your implementation.
// In the course next semester "Algorithm Engineering Lab" we'll learn simple
// parallelization possibilities for C++.

void bucket_sort(vector<Employee> &employees) {
  // 1. create a vector of 53 buckets (0-52) for each age from 18 to 70
  vector<vector<Employee>> buckets(53);

  // 2. iterate over all employees
  for (auto &employee : employees) {
    // 3. put each employee in the corresponding bucket
    buckets[employee.age - 18].push_back(employee);
  }

  employees.clear();

  // 4. iterate over all buckets
  for (auto &bucket : buckets) {
    // 5. append all employees from the bucket to the employees vector
    employees.insert(end(employees), begin(bucket), end(bucket));
  }
}
/*************** end assignment ***************/

int main() {
  // test correctness
  for (int i = 0; i < 1000; ++i) {
    auto employees = generate_random_vector(i);
    auto copy_employess = employees;
    sort_with_std_stable(employees);
    bucket_sort(copy_employess);
    assert(employees == copy_employess);
  }

  TIMERSTART(generate_random_vector)
  int n = 100000000;
  auto employees = generate_random_vector(n);
  auto copy_employess = employees;
  TIMERSTOP(generate_random_vector)

  TIMERSTART(std_sort)
  sort_with_std_stable(employees);
  TIMERSTOP(std_sort)

  TIMERSTART(bucket_sort)
  bucket_sort(copy_employess);
  TIMERSTOP(bucket_sort)
  assert(employees == copy_employess);
}
