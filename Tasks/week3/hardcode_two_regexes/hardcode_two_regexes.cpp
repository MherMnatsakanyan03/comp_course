// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>

using namespace std;

// helper macros for timing
#define TIMERSTART(label)                                                \
  std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
  a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                                      \
  b##label = std::chrono::system_clock::now();                                \
  std::chrono::duration<double> delta##label = b##label - a##label;           \
  std::cout << #label << ": " << std::setprecision(4) << delta##label.count() \
            << " seconds" << std::endl;

// function replaces "search_for" in-place with "replace_with" in string "s"
inline void replace_space_out(std::string &s, const std::string &search_for,
                              const std::string &replace_with)
{
  // not allowed to replace with a longer string
  if (replace_with.size() > search_for.size())
  {
    throw std::runtime_error(
        "size of <replace_with> must be <= size of <search_for>");
  }
  if (search_for.empty())
    return;

  // strstr locates a substring and returns a pointer to the first occurrence
  char *pch = const_cast<char *>(strstr(s.data(), search_for.data()));
  while (pch != nullptr)
  { // repeat till the end is reached
    // copies all characters from replace_with to pch
    strncpy(pch, replace_with.data(), replace_with.size());
    // overwrite invalid characters with spaces
    for (size_t i = 0; i < search_for.size() - replace_with.size(); ++i)
    {
      pch[i + replace_with.size()] = ' ';
    }
    // search for the next occurrence of string search_for
    pch = const_cast<char *>(strstr(pch + search_for.size(), search_for.data()));
  }
}

// counts the number of occurrences of a word in the text
inline size_t count_word_with_c(const string &text, const string &word)
{
  size_t count_c = 0;
  if (word.empty())
    return count_c;
  // strstr locates a substring and returns a pointer to the first occurrence
  const char *pch = strstr(text.data(), word.data());
  while (pch != nullptr)
  { // repeat till the end is reached
    count_c++;
    pch = strstr(pch + word.size(), word.data());
  }
  return count_c;
}

// function replaces "search_for" with "replace_with" in string "s"
// size of "replace_with" is larger than size of "search_for"
// here, a new string is allocated and returned in the end
// fast enough for our purpose, but could be done "in-place" by reallocating the
// original string ;)
inline string replace_with_larger_string(std::string &s,
                                         const std::string &search_for,
                                         const std::string &replace_with)
{
  if (replace_with.size() <= search_for.size())
  {
    throw std::runtime_error("use function <replace_space_out> because size of "
                             "<replace_with> <= size of <search_for>");
  }
  size_t count = count_word_with_c(s, search_for);
  if (count == 0)
    return string(s);
  size_t difference = replace_with.size() - search_for.size();
  std::string new_text(count * difference + s.size(), ' ');

  char *pch = const_cast<char *>(strstr(s.data(), search_for.data()));
  char *pch_old = const_cast<char *>(s.data());
  char *pch_new = const_cast<char *>(new_text.data());
  while (pch != nullptr)
  { // repeat till the end is reached
    auto offset = pch - pch_old;
    strncpy(pch_new, pch_old, offset);
    pch_new += offset;
    pch_old += offset + search_for.size();

    strncpy(pch_new, replace_with.data(), replace_with.size());
    pch_new += replace_with.size();

    // search for the next occurrence of string search_for
    pch =
        const_cast<char *>(strstr(pch + search_for.size(), search_for.data()));
  }
  auto offset = &(*s.end()) - pch_old;
  strncpy(pch_new, pch_old, offset);
  return new_text;
}

void regex_clean(string &text)
{
  { // delete Jackfruit from text
    static const auto pattern = std::regex{"Jackfruit"};
    text = regex_replace(text, pattern, "");
  }
  { // clean apple
    static const auto pattern = std::regex{"Apple|aple|Ap\\sple123"};
    text = regex_replace(text, pattern, "apple");
  }
  { // lemon"
    static const auto pattern = std::regex{"Lemon|limon|le\\smon"};
    text = regex_replace(text, pattern, "lemon");
  }
  { // clean pomegranate
    static const auto pattern =
        std::regex{"Pomegranate|Pome-granate|P0megranate"};
    text = regex_replace(text, pattern, "pomegranate");
  }
  { // "grap " --> "grape "
    static const auto pattern = std::regex{"grap\\s"};
    text = regex_replace(text, pattern, "grape ");
  }
  { // clean grape
    static const auto pattern = std::regex{"Grape|grape5|greengrapes"};
    text = regex_replace(text, pattern, "grape");
  }
  { // clean coconut
    // if after Coconut appear 3 to 6 digits --> remove those digits
    // and make in Coconut the uppercase C to a lowercase c
    static const auto pattern = std::regex{"Coconut\\d{3,6}"};
    text = regex_replace(text, pattern, "coconut");
  }
  { // if 2 or more white spaces reduce them to one, "   " --> " "
    static const auto pattern = std::regex{"\\s{2,}"};
    text = regex_replace(text, pattern, " ");
  }
}

/************** begin assignment **************/
// In this assignment you try to hardcode the last two regex expressions (line
// 131 - 140) from the function "regex_clean". Write code inside the functions
// "clean_coconut" and "clean_spaces". The first six regex expressions of
// function "regex_clean" are already hardcoded in function "fast_clean".

inline void clean_spaces(std::string &s);

// TODO: hardcode coconut regex: "Coconut\\d{3,6}" --> "coconut"
// Hints (advanced):
// - can be done in-place, just put spaces instead of digits, we clean spaces in
//   the end
// - there is a C function "isdigit" to check if a character is a digit
// - search first for "Coconut" and than check if there are 3 to 6 digits after
//   "Coconut"
// - use "strstr" for maximum speed
// - to cast a "const char pointer" to a "non-const char pointer" use const_cast
//
// If you need extra memory, use it! There won't be a tremendous performance
// penalty for doing that ... If you don't feel comfortable working with raw
// C-style pointers than choose an alternative approach and ignore the hints.
// Those hints are really advanced stuff and may harm more than help you, if you
// are unsure what they mean or what to do with them!
inline void clean_coconut(std::string &s) {
    const char *start = s.c_str();
    char *current = const_cast<char *>(start);

    while ((current = strstr(current, "Coconut")) != nullptr) {
        char *digitStart = current + 7; // Move past "Coconut"
        char *digitEnd = digitStart;

        // Check for 3 to 6 digits after "Coconut"
        for (int count = 0; count < 6 && isdigit(*digitEnd); ++count) {
            digitEnd++;
        }

        // Ensure we have exactly 3 to 6 digits
        if (digitEnd - digitStart >= 3) {
            std::fill(digitStart, digitEnd, ' '); // Replace digits with spaces
            current[0] = 'c'; // Replace uppercase 'C' with lowercase 'c'
        }

        current = digitEnd; // Move current pointer past the digits
    }
}

// TODO: hardcode regex to clean two or more spaces into one space: "\\s{2,}" --> " "
// Hints (advanced):
// - can be done in-place, just count the spaces you are "overwriting"
//   and in the end resize the string like
//   "s.resize(s.size() - amount_removed_spaces);"
// - "memmove" allows copy destination and copy source to overlap when copying
//   bytes
// - search first for two spaces "  " and than check if there are more o them
// - use "strstr" for maximum speed
// - to cast a "const char pointer" to a "non-const char pointer" use const_cast
//
// If you need extra memory, use it! There won't be a tremendous performance
// penalty for doing that ... If you don't feel comfortable working with raw
// C-style pointers than choose an alternative approach and ignore the hints.
// Those hints are really advanced stuff and may harm more than help you, if you
// are unsure what they mean or what to do with them!
inline void clean_spaces(std::string &s)
{
  const char *start = s.c_str();
  char *destination = const_cast<char *>(start);
  char *source = destination;
  bool in_space_block = false;
  while (*source)
  {
    // idea: move two pointers, one for reading and one for writing
    // source reads the characters, destination writes them, basically
    // pulling the characters to the front
    if (*source == ' ')
    {
      if (!in_space_block)
      {
        *destination++ = ' ';
        in_space_block = true;
      }
    }
    else
    {
      *destination++ = *source;
      in_space_block = false;
    }
    source++;
  }
  s.resize(destination - start);
}

// we compare the speed of the slow "regex_clean" function with this one, where
// we use hardcoded regex expressions
void fast_clean(string &text)
{
  // "replace_space_out" is used when the replacement string is not larger
  // delete Jackfruit from text
  replace_space_out(text, "Jackfruit", "");
  // Apple|Ap ple123
  replace_space_out(text, "Apple", "apple");
  replace_space_out(text, "Ap ple123", "apple");
  // Lemon|limon|le mon
  replace_space_out(text, "Lemon", "lemon");
  replace_space_out(text, "limon", "lemon");
  replace_space_out(text, "le mon", "lemon");
  //  Pomegranate|Pome-granate|P0megranate
  replace_space_out(text, "Pomegranate", "pomegranate");
  replace_space_out(text, "Pome-granate", "pomegranate");
  replace_space_out(text, "P0megranate", "pomegranate");
  // Grape|grape5|greengrapes
  replace_space_out(text, "Grape", "grape");
  replace_space_out(text, "grape5", "grape");
  replace_space_out(text, "greengrapes", "grape");

  // "replace_with_larger_string" is used when the replacement string is larger
  // grap
  text = replace_with_larger_string(text, "grap ", "grape ");
  // aple
  text = replace_with_larger_string(text, "aple", "apple");

  //// hardcode the two commented out regex expressions
  //// (if you uncomment them, the code becomes correct, but unnecessary slow)
  ////
  //// clean coconut
  //// if after Coconut appear 3 to 6 digits --> remove those digits
  //// and make in Coconut the uppercase C to a lowercase c
  //  {
  //    static const auto pattern = std::regex{"Coconut\\d{3,6}"};
  //    text = regex_replace(text, pattern, "coconut");
  //  }
  //// if 2 or more white spaces reduce them to one, "   " --> " "
  //  {
  //    static const auto pattern = std::regex{"\\s{2,}"};
  //    text = regex_replace(text, pattern, " ");
  //  }

  // TODO: write your hardcoded regex expressions into these two functions
  clean_coconut(text);
  clean_spaces(text);
}
/*************** end assignment ***************/

int main()
{
  ifstream ifs("messy_text.txt");
  if (ifs.fail())
  {
    cerr << "file not found\n";
    return 0;
  }
  string text;
  getline(ifs, text, '\0'); // read the whole file into a string

  string copy_regex = text;
  TIMERSTART(regex_clean)
  regex_clean(copy_regex);
  TIMERSTOP(regex_clean)

  string copy_fast = text;
  TIMERSTART(fast_clean)
  fast_clean(copy_fast);
  TIMERSTOP(fast_clean)

  assert(copy_regex == copy_fast);
}
