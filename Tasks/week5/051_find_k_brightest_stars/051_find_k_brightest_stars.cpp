// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

/************** begin assignment **************/
// Find k (= 8) brightest stars in a stream. To measure the brightness we use
// the visual magnitude (Vmag). The smaller Vmag the brighter the star appears
// to us. If you want to know more about Vmag read this:
// https://de.wikipedia.org/wiki/Scheinbare_Helligkeit
// We work with the file stars.txt. Look at the structure of the file, because
// you will parse it line by line to extract information about the stars.

struct Star
{
  string name; // name of the star
  double vmag; // stars brightness (the smaller vmag the brighter the star)
};

// You get the input file stream "ifs" to the "stars.txt" file and a parameter
// "k". "k" is the amount of the brightest stars we wan't to extract from the
// stream. Try to extract the "k" brightest stars in O(n * log(k)) time with
// O(log n) extra space.
vector<Star> find_k_brightest_stars(ifstream &ifs, const size_t k)
{
  // skipt the first 17 lines
  for (size_t i = 0; i < 17; ++i)
  {
    string line;
    getline(ifs, line);
  }

  // "result" contains in the end the k brightest stars in sorted order
  // (brightest first)
  vector<Star> result(k);

  auto comparator = [](const Star &a, const Star &b)
  {
    return a.vmag < b.vmag;
  };
  // use a min heap
  priority_queue<Star, vector<Star>, decltype(comparator)> min_heap(comparator);

  string line;
  // read as long as the line doesn't start with the string "#"
  while (getline(ifs, line) && line[0] != '#')
  {
    // first string trimmed until 17th char is name of the star
    // from char 64 is vmag
    uint endName = line.find_last_not_of(" ", 17);
    string name = line.substr(0, endName + 1);
    double vmag = stod(line.substr(64, 5));

    Star star{name, vmag};

    // if min heap is not full yet, push star to min heap
    if (min_heap.size() < k)
    {
      min_heap.push(star);
    }
    else
    {
      // if min heap is full, compare the current star with the smallest star
      // in the min heap. If the current star is brighter, replace the smallest
      // star in the min heap with the current star
      if (vmag < min_heap.top().vmag)
      {
        min_heap.pop();
        min_heap.push(star);
      }
    }
  }

  // copy min heap to result
  for (size_t i = k; i > 0; --i)
  {
    result[i - 1] = min_heap.top();
    min_heap.pop();
  }

  return result;
}
/*************** end assignment ***************/

int main()
{
  string path = "051_stars.txt";
  ifstream ifs{path}; // open file as a stream
  if (ifs.fail())
  {
    cerr << "cannot find: " << path << endl;
    return 1;
  }
  size_t k = 8; // k brightest stars
  vector<Star> brightest_stars = find_k_brightest_stars(ifs, k);

  vector<Star> correct_stars{{"Sirius", -1.45}, {"Canopus", -0.62}, {"Arcturus", -0.05}, {"Rigil Kentaurus", -0.01}, {"Vega", 0.03}, {"Capella", 0.08}, {"Rigel", 0.18}, {"Procyon", 0.4}};

  // test correctness of solution
  assert(brightest_stars.size() == correct_stars.size());
  const double epsilon = 1.0e-12; // almost 0
  for (size_t i = 0; i < brightest_stars.size(); ++i)
  {
    assert(abs(brightest_stars[i].vmag - correct_stars[i].vmag) < epsilon);
    assert(brightest_stars[i].name == correct_stars[i].name);
  }

  // output brightest stars
  for (const auto &star : brightest_stars)
  {
    cout << star.name << endl;
    cout << star.vmag << endl;
    cout << endl;
  }
}
