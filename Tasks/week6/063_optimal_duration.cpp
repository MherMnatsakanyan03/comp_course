// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>

using namespace std;

/************** begin assignment **************/
// We consider the problem of assigning tasks to workers. Each worker must be
// assigned exactly two tasks. Each task takes a fixed amount of time. Tasks are
// independent, that is, there are no constraints of the form "Task 4 cannot
// start before Task 3 is completed." Any task can be assigned to any worker. We
// want to assign tasks to workers so as to minimize how long it takes before
// all tasks are completed. For example, if there are 6 tasks whose durations
// are 5, 2, 1, 6, 4, 4 hours, then an optimum assignment is to give the first
// two tasks (that is, the tasks with duration 5 and 2) to one worker, the next
// two (1 and 6) to another worker, and the last two tasks (4 and 4) to the last
// worker. For this assignment, all tasks will finish after
// max(5 + 2, 1 + 6, 4 + 4) = 8 hours. Design an algorithm that takes as input a
// set of tasks and returns the time after all tasks will finish in an optimum
// assignment. In our example the answer would be 8.
//
// hints:
//      - Simply enumerating all possible sets of pairs of tasks is not
//      feasible, there are too many of them --> n!/2^(n/2), where n is the
//      number of tasks
//      - Sorting the tasks by its durations first, helps you to design an
//      optimal greedy strategy.
//      - With which task would you pair the task with the longest duration?
//      - With which task would you pair the task with the second longest
//      duration? And so on ...

// vector "tasks" contains for each task the duration
// (there is an even number of tasks,
// a single task duration is between 0 and 10000)
unsigned int optimal_duration(const vector<unsigned int> &tasks) {
  unsigned int total_duration = 0;

  // sort tasks by duration
  vector<unsigned int> sorted_tasks = tasks;
  sort(sorted_tasks.begin(), sorted_tasks.end());

  // assign tasks to workers
  for (size_t i = 0; i < sorted_tasks.size() / 2; i++) {
    total_duration = max(total_duration, sorted_tasks[i] + sorted_tasks[sorted_tasks.size() - i - 1]);
  }
  
  return total_duration;
}

/*************** end assignment ***************/

int main() {
  assert(optimal_duration({}) == 0); // no tasks, duration is 0
  assert(optimal_duration({1, 3}) == 4);
  assert(optimal_duration({8, 1, 10, 9}) == 17);
  assert(optimal_duration({5, 2, 1, 6, 4, 4}) == 8);
  assert(optimal_duration({1523, 5338, 2914, 1546, 7274, 3263, 1240,
                           4284, 9461, 8142, 6582, 3714, 5527, 9641,
                           2886, 5401, 9727, 4254, 9874, 478}) == 11164);
  assert(optimal_duration(
             {1909, 5799, 6360, 5445, 9563, 3214, 4260, 558,  7246, 3888, 3000,
              3516, 7321, 7538, 1307, 9093, 4005, 3017, 3455, 5253, 2302, 7400,
              9437, 401,  3237, 4624, 2421, 6913, 416,  9824, 8809, 6431, 6576,
              2411, 9519, 1989, 7771, 9022, 5368, 5802, 6197, 7018, 1101, 2697,
              6546, 2920, 9358, 294,  4757, 1643, 1435, 3959, 6999, 7935, 63,
              704,  1193, 8332, 9484, 9926, 1256, 2248, 4466, 1691, 2974, 6503,
              1617, 1277, 872,  7219, 7627, 5636, 7587, 7219, 1962, 3340, 4141,
              1067, 9914, 108,  6327, 5395, 4015, 5559, 2692, 1280, 4230, 2134,
              394,  2076, 6338, 8912, 5171, 9028, 2957, 5112}) == 10123);
  cout << "all tests passed" << endl;
}
