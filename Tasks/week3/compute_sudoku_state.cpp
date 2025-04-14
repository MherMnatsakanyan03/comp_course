// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>

using namespace std;

/************** begin assignment **************/
// Sudoku is a popular logic-based combinatorial number placement puzzle. The
// objective is to fill a 9 x 9 grid with digits subject to the constraint that
// each column, each row, and each of the nine 3 x 3 sub-grids that compose the
// grid contains unique integers.
// In case you need an introduction to Sudoku:
// https://www.learn-sudoku.com/sudoku-rules.html

// We define three different states in which a Sudoku board can be.
enum class state { invalid, valid, solved };
// invalid --> board violates the Sudoku rules
// valid --> board corresponds to the Sudoku rules
// solved --> the puzzle is solved

// Note that the only valid characters are "123456789" and the space ' ' meaning
// an empty field.
// Note that we do not care if a board can actually be solved, we only check if
// it violates the rules, or is valid, or is in a solved state.
// Of course a solved board is also valid, but we only return solved.

// TODO: implement a function that returns the state of a Sudoku board
state compute_sudoku_state(const string &board) {

  // check if the board has the correct size
  if (board.size() != 81) {
    return state::invalid;
  }

  // check if the board contains only valid characters
  for (char c : board) {
    if (c != ' ' && (c < '1' || c > '9')) {
      return state::invalid;
    }
  }

  // check rows
  for (size_t i = 0; i < 9; i++) {
    array<bool, 9> row = {false};
    for (size_t j = 0; j < 9; j++) {
      char c = board[i * 9 + j];
      if (c == ' ') {
        continue;
      }
      if (row[c - '1']) {
        return state::invalid;
      }
      row[c - '1'] = true;
    }
  }

  // check columns
  for (size_t i = 0; i < 9; i++) {
    array<bool, 9> col = {false};
    for (size_t j = 0; j < 9; j++) {
      char c = board[j * 9 + i];
      if (c == ' ') {
        continue;
      }
      if (col[c - '1']) {
        return state::invalid;
      }
      col[c - '1'] = true;
    }
  }

  // check 3x3 sub-grids
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      array<bool, 9> subgrid = {false};
      for (size_t k = 0; k < 3; k++) {
        for (size_t l = 0; l < 3; l++) {
          char c = board[(i * 3 + k) * 9 + j * 3 + l];
          if (c == ' ') {
            continue;
          }
          if (subgrid[c - '1']) {
            return state::invalid;
          }
          subgrid[c - '1'] = true;
        }
      }
    }
  }

  // check if the board is solved
  for (char c : board) {
    if (c == ' ') {
      return state::valid;
    }
  }

  return state::solved;
}
/*************** end assignment ***************/

int main() {
  {
    string board = "274935168"
                   "198726435"
                   "563814729"
                   "356187294"
                   "419562873"
                   "782493516"
                   "821349657"
                   "947651382"
                   "63527894"; // invalid size
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "274935168"
                   "198726435"
                   "563814729"
                   "356187294"
                   "419562873"
                   "782493516"
                   "821349657"
                   "947651382"
                   "6352789412"; // invalid size
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "   9  1 4"
                   " 2  5 7  "
                   "     25 6"
                   "    194 5"
                   " 6574391 "
                   "4 1 26  8"
                   " 18375 4 "
                   " : 4  631" // |:|
                   " 4  6    ";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "586937124"
                   "129654783"
                   "734182596"
                   "372819465"
                   "865743912"
                   "491526308" // |0|
                   "618375249"
                   "257498631"
                   "943261857";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "586937124"
                   "129654783"
                   "734182596"
                   "372819465"
                   "865743912"
                   "4915263a8" // |a|
                   "618375249"
                   "257498631"
                   "943261857";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "145673829"
                   "389215674"
                   "762948135"
                   "296157483"
                   "814369752"
                   "573824961"
                   "957436218"
                   "628591347"
                   "431782596";
    assert(compute_sudoku_state(board) == state::solved);
  }
  {
    string board = "495287163"
                   "821369547"
                   "367145298"
                   "179854632"
                   "634972815"
                   "582613974"
                   "216438759"
                   "953721486"
                   "748596321";
    assert(compute_sudoku_state(board) == state::solved);
  }
  {
    string board = "145673829"
                   "381215674" // 38|1|
                   "762948135"
                   "296157483"
                   "814369752"
                   "573824961"
                   "957436218"
                   "628591347"
                   "431782596";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "145673829"
                   "38 215674"
                   "762948135"
                   "296157483"
                   "814369752"
                   "573824961"
                   "957436 18"
                   "628591347"
                   "431782596";
    assert(compute_sudoku_state(board) == state::valid);
  }
  {
    string board = " 4    897"
                   " 379   52"
                   "8 27  3  "
                   "9  4 7  6"
                   " 8  9  3 "
                   "6 3 18 49"
                   "  83 192 "
                   "26   4  3"
                   " 142596  ";
    assert(compute_sudoku_state(board) == state::valid);
  }
  {
    string board = " 4    897"
                   " 379   52"
                   "8 27  3  "
                   "9  6 7  6" // 9  |6|
                   " 8  9  3 "
                   "6 3 18 49"
                   "  83 192 "
                   "26   4  3"
                   " 142596  ";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = " 4    897"
                   " 379   52"
                   "8 27  3  "
                   "9  4 7  6"
                   " 8  9  3 "
                   "6 3 18 49"
                   "  83 193 " //   83 19|3|
                   "26   4  3"
                   " 142596  ";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "   9  1 4"
                   " 2  5 7  "
                   "     25 6"
                   "    194 5"
                   " 6574391 "
                   "4 1 26  8"
                   " 18375 4 "
                   "   4  631"
                   " 4  6    ";
    assert(compute_sudoku_state(board) == state::valid);
  }
  {
    string board = "2  9  1 4" // |2|
                   " 2  5 7  "
                   "     25 6"
                   "    194 5"
                   " 6574391 "
                   "4 1 26  8"
                   " 18375 4 "
                   "   4  631"
                   " 4  6    ";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "         "
                   "         "
                   "         "
                   "         "
                   "         "
                   "         "
                   "         "
                   "         "
                   "         ";
    assert(compute_sudoku_state(board) == state::valid);
  }
  {
    string board = "1        "
                   " 2       "
                   "  3      "
                   "   4     "
                   "    5    "
                   "     6   "
                   "      7  "
                   "       8 "
                   "        9";
    assert(compute_sudoku_state(board) == state::valid);
  }
  {
    string board = "1        "
                   " 2       "
                   "  3      "
                   "   1     "
                   "    2    "
                   "     3   "
                   "      1  "
                   "       2 "
                   "        3";
    assert(compute_sudoku_state(board) == state::valid);
  }
  {
    string board = "1        "
                   " 2       "
                   "  3      "
                   "   1     "
                   "    2    "
                   "     3   "
                   "      1  "
                   "      32 "
                   "        3";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "1        "
                   " 2       "
                   "  3      "
                   "   1    7"
                   "    2 7  "
                   "     3   "
                   "      1  "
                   "       2 "
                   "        3";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "1        "
                   " 2       "
                   "  3      "
                   "   1     "
                   "    2    "
                   "     3   "
                   "  4   1  "
                   " 4     2 "
                   "        3";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "1        "
                   " 2 1     "
                   "  3      "
                   "   1     "
                   "    2    "
                   "     3   "
                   "      1  "
                   "       2 "
                   "        3";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "1        "
                   " 2       "
                   "  3      "
                   "   1 6   "
                   "   62    "
                   "     3   "
                   "      1  "
                   "       2 "
                   "        3";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  {
    string board = "1        "
                   " 2       "
                   "  3      "
                   " 5 1     "
                   "5   2    "
                   "     3   "
                   "      1  "
                   "       2 "
                   "        3";
    assert(compute_sudoku_state(board) == state::invalid);
  }
  cout << "all tests passed" << endl;
}
