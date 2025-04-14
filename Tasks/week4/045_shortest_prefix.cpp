// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../helper.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>

#define ASSERT_EQUAL(actual, expected) Helper::assertEqual<string>(actual, expected, __FILE__, __LINE__)

using namespace std;

// this is a simple implementation of a trie
class Trie
{
private:
  struct Trie_Node
  {
    // indicates whether the string corresponding to the path from the root is a string
    bool is_string = false;
    unordered_map<char, unique_ptr<Trie_Node>> nodes;
  };

  unique_ptr<Trie_Node> root = unique_ptr<Trie_Node>(new Trie_Node());

public:
  // insert a string into the trie
  bool insert(const string &s)
  {
    auto *p = root.get();
    for (char c : s)
    {
      if (p->nodes.find(c) == p->nodes.cend())
      {
        p->nodes[c] = unique_ptr<Trie_Node>(new Trie_Node());
      }
      p = p->nodes[c].get(); // get pointer of the next node in path
    }
    // s already existed in this trie
    if (p->is_string)
    {
      return false;
    }
    else
    {                      // p->is_string == false.
      p->is_string = true; // inserts s into this trie
      return true;
    }
  }

  // TODO: implement this function (see below)
  string shortest_prefix(const string &s);
};

/************** begin assignment **************/
// Words (strings) have prefixes. For example, the prefixes of "banana" are "b",
// "ba", "ban", "bana", "banan", "banana". This problem is concerned with
// finding the shortest prefix of a string (the query string) that is not in a
// set of strings (the dictionary strings). If all prefixes of the string are
// present, return the empty string. For example:
// - The query is "cat", the dictionary is {"dog", "be", "cut"}: return "ca"
//   since the prefixes of "cat" are "c", "ca", and "cat", and "c" is a prefix
//   of a word in the dictionary, but "ca" is not.
// - The query is "cat", the dictionary is {"dog", "be", "cut", "car"}: return
//   "cat" "c" and "ca" are prefixes of words in the dictionary, but "cat" is
//   not.
// - Look at the test cases in the main function for more examples.
//
// To solve this assignment we use a trie. Study the code between lines 14 and
// 46 to become familiar with the implementation of the trie we use to solve
// this task. The function "shortest_prefix" in line 45 is not yet implemented.
// Implement the function "shortest_prefix" below.
// The function returns the shortest prefix of the query string s
// which is not a prefix of any string in the trie. Return the empty
// string if all prefixes of the query are prefixes of some string in the
// trie. For example, if the query string is "cat", and the trie contains
// {"dog", "be", "cut", "car", "catsnip", "category"}, your program should
// return "", since all prefixes of "cat" are prefixes of words in the
// trie.
//
// hint:
// - Finding a shortest prefix of s that is not a prefix of any string in
//   the trie is simply a matter of finding the first node m on the
//   search path from the root that does not have a child corresponding to the
//   next character in s.
// - You need no recursion, no stack. Look at the lines 29 - 34 to get an idea
//   of how to move along a path in a trie.

string Trie::shortest_prefix(const string &s)
{
  auto *p = root.get(); // get pointer of the root node

  for (size_t i = 0; i < s.size(); ++i)
  {
    char c = s[i];
    if (p->nodes.find(c) == p->nodes.cend())
    {
      return s.substr(0, i + 1);
    }
    p = p->nodes[c].get(); // get pointer of the next node in path
  }

  return "";
}

/*************** end assignment ***************/

int main()
{
  {
    Trie trie;
    // fill trie with words
    for (const string s : {"dog", "be", "cut"})
    {
      trie.insert(s);
    }
    ASSERT_EQUAL(trie.shortest_prefix("cat"), "ca");
    ASSERT_EQUAL(trie.shortest_prefix("dog"), "");
    ASSERT_EQUAL(trie.shortest_prefix("bet"), "bet");
    ASSERT_EQUAL(trie.shortest_prefix("cow"), "co");
    // there is no word starting with "p" in the trie, so the
    // shortest prefix in "pig" is "p"
    ASSERT_EQUAL(trie.shortest_prefix("pig"), "p");
    ASSERT_EQUAL(trie.shortest_prefix("cutting"), "cutt");
    ASSERT_EQUAL(trie.shortest_prefix("cu"), ""); // "cu" is a prefix of "cut"
    ASSERT_EQUAL(trie.shortest_prefix("z"), "z");
    // insert a new string into the trie
    trie.insert("car");
    ASSERT_EQUAL(trie.shortest_prefix("cat"), "cat");
    ASSERT_EQUAL(trie.shortest_prefix(""), "");
  }
  {
    Trie trie;
    // fill trie with words
    for (const string s : {"romane", "romanus", "romulus", "rubens", "ruber",
                           "rubicon", "rubicundus"})
    {
      trie.insert(s);
    }
    ASSERT_EQUAL(trie.shortest_prefix("romane"), "");
    ASSERT_EQUAL(trie.shortest_prefix("romanes"), "romanes");
    ASSERT_EQUAL(trie.shortest_prefix("romines"), "romi");
    ASSERT_EQUAL(trie.shortest_prefix("rubons"), "rubo");
    ASSERT_EQUAL(trie.shortest_prefix("xyz"), "x");
    ASSERT_EQUAL(trie.shortest_prefix("rubicundicus"), "rubicundi");
  }
  cout << "all tests passed" << endl;
}
