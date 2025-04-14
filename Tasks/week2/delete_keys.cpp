// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstddef>
#include <vector>
#include <iostream>

using namespace std;

/************** begin assignment **************/
// Implement a function which takes as input an sorted array and a key, and
// updates the array so that all occurrences of the input key have been removed
// and the remaining elements have been shifted left to fill the emptied
// indices. Return the number of remaining elements. Values beyond the last
// valid element are the same as in the original array.
// (Requirements: O(1) space, O(n) time)

inline size_t delete_keys(vector<int> &v, const int key)
{
  if (v.size() == 0)
  {
    return 0;
  }

  size_t valid_entries = 0;
  size_t i = 0;

  while (i + valid_entries < v.size())
  {
    // i + valid_entries is the position where we need to look at
    // if valid_entries is greater that 0, that means that we dont need
    // to check the current element, because we have alreade moved it behind
    // by | valid_entries | positions
    while ((i + valid_entries < v.size()) && (v[i + valid_entries] == key))
    {
      valid_entries++;
    }

    // as long as we are inbounds, copy the next valid element to the current position
    // where "next valid" means the next index in the theoretically moved field
    if (i + valid_entries < v.size())
    {
      v[i] = v[i + valid_entries];
    }

    i++;
  }

  return v.size() - valid_entries;
}

/*************** end assignment ***************/

int main()
{
  {
    vector<int> v;
    vector<int> vresult;
    int key = 1;
    auto valid_entries = delete_keys(v, key);
    assert(v == vresult);
    assert(valid_entries == 0);
  }
  {
    vector<int> v{1};
    vector<int> vresult{1};
    int key = 2;
    auto valid_entries = delete_keys(v, key);
    assert(v == vresult);
    assert(valid_entries == 1);
  }
  {
    vector<int> v{1};
    vector<int> vresult{1};
    int key = 1;
    auto valid_entries = delete_keys(v, key);
    assert(v == vresult);
    assert(valid_entries == 0);
  }
  {
    vector<int> v{1, 2, 3};
    vector<int> vresult{2, 3, 3};
    int key = 1;
    auto valid_entries = delete_keys(v, key);
    assert(v == vresult);
    assert(valid_entries == 2);
  }
  {
    vector<int> v{1, 2, 2, 3};
    vector<int> vresult{1, 3, 2, 3};
    int key = 2;
    auto valid_entries = delete_keys(v, key);
    assert(v == vresult);
    assert(valid_entries == 2);
  }
  {
    vector<int> v{1, 2, 2, 3, 3};
    vector<int> vresult{1, 3, 3, 3, 3};
    int key = 2;
    auto valid_entries = delete_keys(v, key);
    assert(v == vresult);
    assert(valid_entries == 3);
  }
  {
    vector<int> v{2, 3, 5, 5, 7, 11, 11, 11, 13};
    vector<int> vresult{2, 3, 5, 5, 11, 11, 11, 13, 13};
    int key = 7;
    auto valid_entries = delete_keys(v, key);
    assert(v == vresult);
    assert(valid_entries == 8);
  }
  {
    vector<int> v{1, 2, 3, 4, 3, 5, 6};
    vector<int> vresult{1, 2, 4, 5, 6, 5, 6};
    int key = 3;
    auto valid_entries = delete_keys(v, key);
    assert(v == vresult);
    assert(valid_entries == 5);
  }
  cout << "all tests passed" << endl;
}
