// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../../helper.hpp"
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// helper macros for timings
#define TIMERSTART(label)                                                \
  std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
  a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                                      \
  b##label = std::chrono::system_clock::now();                                \
  std::chrono::duration<double> delta##label = b##label - a##label;           \
  std::cout << #label << ": " << std::setprecision(4) << delta##label.count() \
            << " seconds" << std::endl;

using namespace std;

struct Input
{
  string text;               // only words and spaces
  vector<size_t> word_sizes; // contains for each word in the text the amount of characters
  size_t longest_word;       // amount of characters in the longest word
};

struct Result
{
  size_t line_width;   // line width that produces the longest river
  size_t river_length; //  length of the longest river

  // Overload operator<< for Result
  friend std::ostream &operator<<(std::ostream &os, const Result &result)
  {
    os << "[line_width: " << result.line_width << ", river_length: " << result.river_length << "]";
    return os;
  }
  // for testing equality of results (ignore this)
  bool operator==(const Result &rhs) const
  {
    return line_width == rhs.line_width && river_length == rhs.river_length;
  }

  bool operator!=(const Result &rhs) const
  {
    return !(*this == rhs);
  }
};

#define ASSERT_EQUAL(actual, expected) Helper::assertEqual<Result>(actual, expected, __FILE__, __LINE__)

// one could easily tune the io speed, but io is not critical here,
// so we don't care about its speed
Input get_input(const string &path)
{
  ifstream ifs(path);
  if (ifs.fail())
  {
    throw runtime_error("cannot open file " + path + "\n");
  }
  size_t words;
  ifs >> words;              // read amount of words
  vector<size_t> word_sizes; // contains the size of each word
  string word;
  string text;
  size_t longest_word = 0;
  while (ifs >> word)
  {
    text += word + " ";
    word_sizes.push_back(word.size());
    longest_word = max(longest_word, word.size());
  }
  text.resize(text.size() - 1);
  return Input{move(text), move(word_sizes), longest_word};
}

Result get_result(const string &path)
{
  size_t line_width;
  size_t river_length;
  ifstream ifs(path);
  if (ifs.fail())
  {
    throw runtime_error("cannot open file " + path + "\n");
  }
  ifs >> line_width;
  ifs >> river_length;
  return Result{line_width, river_length};
}

/************** begin assignment **************/

// Read the detailed description of the problem in "exercise_go_with_the_flow.pdf.
// This is an actual problem from the "icpc world finals 2018".
// So some of the best programmers tried to solve this under time pressure.
// If you need some hints, read: hints_for_solution.pdf
//
// If you are interested in seeing more icpc problems that were used in the
// world finals, see here:
// https://icpc.baylor.edu/worldfinals/problems
//
// In a real competition you would also have to write the boilerplate code
// to parse the input.
// You don't have to write the boilerplate code here to parse the input.
// Here, you try to accomplish the actual task of finding the line width
// where the input text contains the longest possible river.
// If more than one line width yields this maximum,
// take the shortest line width.
// In the original task there is a time limit of 12 seconds.
// You don't have a time limit.
// But be aware that naive solutions will run "forever".

Result find_longest_river(Input &input)
{
  // You have the following information to design your algorithm.
  // You don't have to use everything, use only what you need.

  // the actual text, text is cleaned and contains only words and spaces
  // const string &text = input.text;
  // amount of characters in the longest word in the text
  const size_t longest_word = input.longest_word;
  // contains for each word in the text the amount of characters (same order of words as in the text)
  const vector<size_t> &word_sizes = input.word_sizes;
  // the amount of all words in the text
  const size_t words_count = input.word_sizes.size();

  // When I solved the problem, the following thing was important,
  // which is not clearly mentioned in the task description:
  // --> a new line never starts with a space
  //     (if you would have a space after a line break, ignore the space and continue with the next word)

  // should contain in the end the "minimal" optimal line width
  size_t line_width = longest_word; // line width can't be shorter than the longest word
  // should contain in the end the longest possible river
  size_t river_length = 0;
  size_t num_lines = 1;

  size_t min_line_width = longest_word;

  size_t current_line_width = min_line_width;
  // Idea: Try all possible line widths and calculate the longest river for each line width
  while (river_length < num_lines)
  {
    vector<uint16_t> prev_line(current_line_width, 0);
    vector<uint16_t> current_line(current_line_width, 0);
    // Principle: take prev_line and current_line,
    size_t char_idx = -1;
    size_t word_idx = 0;
    size_t longest_river = 0;
    num_lines = 1;
    size_t length_until_next_line = SIZE_MAX;

    // Fill the lines with words, respecting line breaks
    while (word_idx < words_count - 1)
    {
      if (char_idx + word_sizes[word_idx] < current_line_width)
      {
        char_idx += word_sizes[word_idx] + 1;
        if (char_idx + word_sizes[word_idx + 1] < current_line_width)
        {
          uint16_t above_left = prev_line[char_idx - 1];
          uint16_t above = prev_line[char_idx];
          uint16_t above_right = prev_line[char_idx + 1];

          current_line[char_idx] = max(above, max(above_left, above_right)) + 1;

          longest_river = max(longest_river, static_cast<size_t>(current_line[char_idx]));
        }
        else
        {
          length_until_next_line = min(length_until_next_line, char_idx + word_sizes[word_idx + 1] + 1);
        }

        word_idx++;
      }
      else
      {
        num_lines++;
        swap(prev_line, current_line);
        fill(current_line.begin(), current_line.end(), 0);
        char_idx = -1;
      }
    }

    if (longest_river > river_length)
    {
      river_length = longest_river;
      line_width = current_line_width;
    }

    current_line_width = length_until_next_line;
  }

  return Result{line_width, river_length};
}

/*************** end assignment ***************/

int main()
{
  // set data_dir to folder with test data
  string data_dir = "./F-gowithflow/";
  // test texts in task description
  {
    string text = "The Yangtze is the third longest "
                  "river in Asia and the longest in "
                  "the world to flow "
                  "entirely in one country";
    vector<size_t> word_sizes{3, 7, 2, 3, 5, 7, 5, 2, 4, 3, 3,
                              7, 2, 3, 5, 2, 4, 8, 2, 3, 7};
    Input input{move(text), move(word_sizes), 8};
    Result result = find_longest_river(input);
    Result correct_result{15, 5};
    cout << "Done With first test" << endl;
    ASSERT_EQUAL(result, correct_result);
  }
  {
    string text = "When two or more rivers meet at "
                  "a confluence other than the sea "
                  "the resulting merged river takes "
                  "the name of one of those rivers";
    vector<size_t> word_sizes{4, 3, 2, 4, 6, 4, 2, 1, 10, 5, 4, 3, 3,
                              3, 9, 6, 5, 5, 3, 4, 2, 3, 2, 5, 6};
    Input input{move(text), move(word_sizes), 10};
    Result result = find_longest_river(input);
    Result correct_result{21, 6};
    cout << "Done With first test" << endl;
    ASSERT_EQUAL(result, correct_result);
  }

  TIMERSTART(total_time_on_test_data);
  // test now the texts provided by the organisers of the competition
  {
    string secret = "secret-";
    for (int i = 1000; i < 1030; ++i)
    {
      string in = secret + to_string(i).substr(1, 4) + ".in";
      string ans = secret + to_string(i).substr(1, 4) + ".ans";
      Input input = get_input(data_dir + in);
      Result correct_result = get_result(data_dir + ans);
      Result result = find_longest_river(input);
      ASSERT_EQUAL(result, correct_result);
      cout << in.data() << " passed\n";
    }
  }
  {
    string secret = "secret-";
    for (int i = 1031; i <= 1047; ++i)
    {
      string in = secret + to_string(i).substr(1, 4) + "-per.in";
      string ans = secret + to_string(i).substr(1, 4) + "-per.ans";
      Input input = get_input(data_dir + in);
      Result correct_result = get_result(data_dir + ans);
      Result result = find_longest_river(input);
      ASSERT_EQUAL(result, correct_result);
      cout << in.data() << " passed\n";
    }
  }
  {
    string secret = "secret-";
    for (int i = 1048; i <= 1062; ++i)
    {
      string in = secret + to_string(i).substr(1, 4) + "-dk.in";
      string ans = secret + to_string(i).substr(1, 4) + "-dk.ans";
      Input input = get_input(data_dir + in);
      Result correct_result = get_result(data_dir + ans);
      Result result = find_longest_river(input);
      ASSERT_EQUAL(result, correct_result);
      cout << in.data() << " passed\n";
    }
  }
  {
    Input input = get_input(data_dir + "secret-099-small.in");
    Result correct_result = get_result(data_dir + "secret-099-small.ans");
    Result result = find_longest_river(input);
    ASSERT_EQUAL(result, correct_result);
    cout << "secret-099-small.in passed\n";
  }
  TIMERSTOP(total_time_on_test_data);

  return 0;
}
