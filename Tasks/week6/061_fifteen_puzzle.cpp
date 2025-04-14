// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

// helper macros for timings
#define TIMERSTART(label)                                                      \
  std::chrono::time_point<std::chrono::system_clock> a##label, b##label;       \
  a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                                       \
  b##label = std::chrono::system_clock::now();                                 \
  std::chrono::duration<double> delta##label = b##label - a##label;            \
  std::cout << #label << ": " << std::setprecision(4) << delta##label.count()  \
            << " seconds" << std::endl;

using namespace std;

class Game {
private:
  // frame needs 128 bit (8 bit * 16), we could also press the information into
  // a 64 bit number since our values are between 0 and 15 --> require 4 bit
  // instead of 8 bit (actually that would be an interesting idea, but we don't
  // do that in order to make the code more simple / readable)
  array<uint8_t, 16> frame;

  // vector "moves" contains previous moves so far
  vector<char> moves;

  // Legal moves are 'R', 'L', 'U', and 'D' for right, left, up, and down.
  // array "valid_moves" contains valid moves for each index in the frame
  const array<string, 16> valid_moves{
      "RD",  "RLD",  "RLD",  "LD",  "RUD", "RLUD", "RLUD", "LUD",
      "RUD", "RLUD", "RLUD", "LUD", "RU",  "RLU",  "RLU",  "LU"};

  // Translate char direction to internal move in the frame.
  // Of course we could avoid translating a direction to an
  // internal representation by using the numbers as valid moves directly.
  static int to_internal(char direction) {
    switch (direction) {
    case 'R':
      return 1;
    case 'L':
      return -1;
    case 'D':
      return 4;
    case 'U':
      return -4;
    default:
      throw std::runtime_error("Legal moves are 'R', 'L', 'U', and 'D'!");
    }
  }

  // index of zero --> (empty) tile
  size_t index_missing_tile = 0;

public:
  // output game to the console (you can cout a game object to see its content)
  friend ostream &operator<<(ostream &os, const Game &game) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        os << int(game.frame[i * 4 + j]) << "\t";
      }
      os << endl;
    }
    os << "moves: ";
    for (char c : game.moves) {
      os << c;
    }
    os << endl;
    os << "moves count: " << game.moves.size() << endl;
    os << "final state: " << boolalpha << game.is_in_final_state() << endl;
    return os;
  }

  // get the frame of all tiles
  const array<uint8_t, 16> &get_frame() const { return frame; }

  // get all moves you've done so far
  const vector<char> &get_moves_so_far() const { return moves; }

  // returns valid moves of the current game state as a string
  string get_valid_moves() const { return valid_moves[index_missing_tile]; }

  // computes the manhattan distance of the current frame (how far are we from
  // the final state)
  int get_manhattan_distance() const {
    int distance = 0;
    for (int i = 0; i < 16; ++i) {
      if (frame[i] != 0) {
        int col_diff = abs((frame[i] - 1) % 4 - i % 4);
        int row_diff = abs((frame[i] - 1) / 4 - i / 4);
        distance += col_diff + row_diff;
      }
    }
    return distance;
  }

  // Makes a move in a given direction. First call get_valid_moves to obtain
  // valid move directions of the current game state.
  void move(char direction) {
    // Input validation for your own safety!
    if (strchr(valid_moves[index_missing_tile].c_str(), direction)) {
      moves.push_back(direction);
      int i = to_internal(direction);
      swap(frame[index_missing_tile], frame[index_missing_tile + i]);
      index_missing_tile += i;
    } else {
      throw std::runtime_error("Invalid move! Index of missing tile: " +
                               to_string(index_missing_tile) +
                               ", Invalid Move: " + direction);
    }
  }

  // undo last move
  void undo_move() {
    if (!moves.empty()) {
      char last_direction = moves.back();
      moves.pop_back(); // remove move from moves
      int i = to_internal(last_direction);
      swap(frame[index_missing_tile], frame[index_missing_tile - i]);
      index_missing_tile -= i;
    }
  }

  // we can hash bitsets, but not arrays, so we cheat here and return a bitset
  // we could also press the information into a 64 bit number with some bit
  // shifting ... (but we're lazy and cast to a bitset)
  const bitset<128> &key() { return (bitset<128> &)frame; }

  // returns true if frame is in the final state, else false
  bool is_in_final_state() const {
    static constexpr array<uint8_t, 16> final_state{
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
    return frame == final_state;
  }

  // constructs a game given a valid frame
  explicit Game(const array<uint8_t, 16> &frame) : frame(frame) {
    array<uint8_t, 16> histogram{0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0};
    // test if all values are below 16
    for (unsigned char i : frame) {
      if (frame[i] >= 16) {
        throw std::runtime_error("Only values below 16 are allowed!");
      }
      if (frame[i] == 0) {
        // initialize zero tile
        index_missing_tile = i;
      }
      histogram[frame[i]]++;
    }
    // test if all values unique
    for (unsigned char i : histogram) {
      if (histogram[i] != 1) {
        throw std::runtime_error("Values in the frame must be all unique!");
      }
    }
  }
};

/************** begin assignment **************/
// The 15-puzzle is a very popular game: you have certainly seen it even if you
// don't know it by that name. It is constructed with 15 sliding tiles, each
// with a different number from 1 to 15, with all tiles packed into a 4 by 4
// frame with one tile missing. The object of the puzzle is to arrange the tiles
// so that they are ordered as below:
//
//  +---+---+---+---+
//  | 1 | 2 | 3 | 4 |
//  +---+---+---+---+
//  | 5 | 6 | 7 | 8 |
//  +---+---+---+---+
//  | 9 | 10| 11| 12|
//  +---+---+---+---+
//  | 13| 14| 15|   |
//  +---+---+---+---+
//    final state
//
// The only legal operation is to exchange the missing tile with one of the 2,
// 3, or 4 tiles it shares an edge with. Consider the following sequence of
// moves:
//
//  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+
//  | 2 | 12| 11| 14|  | 2 | 12| 11| 14|  | 2 | 12| 11| 14|  | 2 | 12| 11| 14|
//  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+
//  | 6 | 15| 10| 5 |  | 6 | 15| 10| 5 |  | 6 | 15|   | 5 |  | 6 |   | 15| 5 |
//  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+
//  | 3 |   | 9 | 13|  | 3 | 9 |   | 13|  | 3 | 9 | 10| 13|  | 3 | 9 | 10| 13|
//  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+
//  | 8 | 7 | 1 | 4 |  | 8 | 7 | 1 | 4 |  | 8 | 7 | 1 | 4 |  | 8 | 7 | 1 | 4 |
//  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+  +---+---+---+---+
//   A random puzzle   The missing tile   The missing tile    The missing tile
//      position        moves right (R)   moves upwards (U)    moves left (L)
//
// We denote moves by the neighbor of the missing tile is swapped with it. Legal
// values are 'R', 'L', 'U', and 'D' for right, left, up, and down, based on the
// movements of the hole. Given an initial configuration of a 15-puzzle you must
// determine a sequence of steps that take you to the final state. Each solvable
// 15-puzzle input requires at most 80 steps to be solved. Here, you can assume
// that the input puzzle always has a solution.
//
// Your task is simply to find a valid solution for a given puzzle that always
// needs less than 200 moves. (As soon as you realize with a heuristic that a
// move would lead to a solution that has more than 200 moves, prune the move
// away.)
//
// A solution is basically a sequence of movements until reaching the final
// state of the puzzle. (R,D,L,L,U, ....)
// Google the "15 puzzle" if you are still unsure what it's about.

// Your friend has already written the class representing the 15-puzzle game.
// Look at the class defined in line 34, you will use its public interface.
// Your friend also tried to adapt the backtracking approach to solve the 15
// puzzle.
//
// Study the code below.
//
// It all works so far, but the selection of
// candidate moves is the part where your friend needs your help.
// Go to function "construct_candidates" and implement the missing parts.
//
// (This assignment is more about to learn to work with unfamiliar code. --> In
// real Projects it's an essential skill.)

// test if found the solution
bool is_solution(const Game &game) { return game.is_in_final_state(); }

// when we find a solution we write the moves to the output vector "out_moves"
// and set the finished flag to true
void process_solution(const Game &game, vector<char> &out_moves,
                      bool &finished) {
  out_moves = game.get_moves_so_far(); // write moves sequence to output vector
  finished = true; // we are done, because we have found a valid solution (stop
                   // searching)
}

// This is the function where your friend needs your help.
string construct_candidates(Game &game,
                            unordered_set<bitset<128>> &visited_game_states) {
  // we want first consider moves that are closer to the final solution
  // so we use a struct that assigns each candidate a distance
  struct Candidate_Info {
    char candidate;
    int distance;
  };

  // for a given game state, give me the valid moves, for example "RLD"
  string candidates = game.get_valid_moves();
  // we save promising moves into the new_candidates vector
  vector<Candidate_Info> new_candidates;
  // iterate over all possible valid moves
  for (char c : candidates) {
    // we simulate a move to evaluate the position it leads to
    game.move(c);
    int distance = 0;

    // TODO: write code here
    // everything you need to write happens in this for loop
    // (but if you want, you can adapt the code in other places as well)
    // if you do nothing and run the program, you get a stack overflow caused
    // by recursion depth
    // Hint:
    // - exclude moves that lead to a previously visited game state
    //   (when you push a move to "new_candidates" put the hash of the frame
    //   to the set "visited_game_states", so next time you can check if you've
    //   already processed the game state)
    // - you need a distance measure to recognize if the move
    //   may help you to solve the problem (for example manhattan distance,
    //   you can also make your own measure)
    //   (in our case a small distance is good --> we sort moves based on
    //   distance below)
    // - now the most important part: you need to prune moves away that would
    //   lead to a solution that has more than 200 moves (for example the
    //   manhattan distance can be used as a lower bound to compute the amount
    //   of moves needed until we finish --> add the lower bound to the moves
    //   count you already have to decide if a move should be pruned away)
    // - actually you could finish this assignment in less than 10 lines of code
    //   if you use the functionality provided by the Game class
    //   (but you can also write your own code --> I would appreciate it, if you
    //   develop and implement own creative ideas.)
    // - especially useful functions in the Game class are:
    //   * game.key() --> for hashing the frame
    //   * game.get_manhattan_distance() --> basic distance measure until finish
    //   * game.get_moves_so_far().size() --> moves count we already have

    // exclude moves that lead to a previously visited game state
    if (visited_game_states.find(game.key()) != visited_game_states.end()) {
      game.undo_move(); // undo the move
      continue;
    }
    
    // add the hash of the frame to the set of visited game states
    visited_game_states.insert(game.key());

    // compute the manhattan distance of the current frame
    distance = game.get_manhattan_distance();

    // prune moves away that would lead to a solution that has more than 200 moves
    if (distance + game.get_moves_so_far().size() > 200) {
      game.undo_move(); // undo the move
      continue;
    }

    game.undo_move();
    new_candidates.push_back({c, distance});
  }

  // sort new candidates by its distance to the final solution,
  // less distant candidates come first
  sort(begin(new_candidates), end(new_candidates),
       [](const Candidate_Info &a, const Candidate_Info &b) {
         return a.distance <= b.distance;
       });
  // delete old candidates
  candidates.clear();
  // write new candidates to old ones
  for (auto candidate : new_candidates) {
    candidates += candidate.candidate;
  }
  return candidates;
}

// backtracking adapted for the 15-puzzle
void backtrack(Game &game, unordered_set<bitset<128>> &visited_game_states,
               bool &finished, vector<char> &out_moves) {
  if (is_solution(game)) { // if we found a solution, process it
    process_solution(game, out_moves, finished);
  } else {
    string candidates = construct_candidates(game, visited_game_states);
    for (const char candidate : candidates) {
      game.move(candidate); // add candidate to possible solution
      backtrack(game, visited_game_states, finished, out_moves);
      if (finished) {
        return; // terminate early
      }
      game.undo_move();
    }
  }
}

/*************** end assignment ***************/

int main() {
  {
    // let's test your solution with a simple example first
    array<uint8_t, 16> example{2, 3, 4,  0,  1,  5,  7,  8,
                               9, 6, 10, 12, 13, 14, 11, 15};
    Game game(example);
    Game game_copy = game;

    // variables for backtracking
    bool finished = false;
    // write here the sequence of moves needed to solve the problem
    vector<char> out_moves;
    // a set for holding already visited states
    unordered_set<bitset<128>> visited_game_states;
    // start backtracking
    backtrack(game_copy, visited_game_states, finished, out_moves);

    // test solution for correctness
    for (char out_move : out_moves) {
      game.move(out_move);
    }
    assert(game.is_in_final_state());
    cout << "simple example : " << out_moves.size() << " moves\n";
    cout << "------------------------------------" << endl;
  }

  // the hardest of the 16! / 2 solvable positions --> 80 moves optimal
  // http://kociemba.org/themen/fifteen/fifteensolver.html
  vector<array<uint8_t, 16>> hardest_problems{
      {0, 12, 9, 13, 15, 11, 10, 14, 3, 7, 2, 5, 4, 8, 6, 1},
      {0, 12, 10, 13, 15, 11, 14, 9, 3, 7, 2, 5, 4, 8, 6, 1},
      {0, 11, 9, 13, 12, 15, 10, 14, 3, 7, 6, 2, 4, 8, 5, 1},
      {0, 15, 9, 13, 11, 12, 10, 14, 3, 7, 6, 2, 4, 8, 5, 1},
      {0, 12, 9, 13, 15, 11, 10, 14, 3, 7, 6, 2, 4, 8, 5, 1},
      {0, 12, 14, 13, 15, 11, 9, 10, 3, 7, 6, 2, 4, 8, 5, 1},
      {0, 12, 10, 13, 15, 11, 14, 9, 3, 7, 6, 2, 4, 8, 5, 1},
      {0, 12, 11, 13, 15, 14, 10, 9, 3, 7, 6, 2, 4, 8, 5, 1},
      {0, 12, 10, 13, 15, 11, 9, 14, 7, 3, 6, 2, 4, 8, 5, 1},
      {0, 12, 9, 13, 15, 11, 14, 10, 3, 8, 6, 2, 4, 7, 5, 1},
      {0, 12, 9, 13, 15, 11, 10, 14, 8, 3, 6, 2, 4, 7, 5, 1},
      {0, 12, 14, 13, 15, 11, 9, 10, 8, 3, 6, 2, 4, 7, 5, 1},
      {0, 12, 9, 13, 15, 11, 10, 14, 7, 8, 6, 2, 4, 3, 5, 1},
      {0, 12, 10, 13, 15, 11, 14, 9, 7, 8, 6, 2, 4, 3, 5, 1},
      {0, 12, 9, 13, 15, 8, 10, 14, 11, 7, 6, 2, 4, 3, 5, 1},
      {0, 12, 9, 13, 15, 11, 10, 14, 3, 7, 5, 6, 4, 8, 2, 1},
      {0, 12, 9, 13, 15, 11, 10, 14, 7, 8, 5, 6, 4, 3, 2, 1}};

  TIMERSTART(total_time)
  double average_moves = 0; // for computing the average moves per problem
  for (size_t i = 0; i < hardest_problems.size(); ++i) {
    // create game and a copy of the game
    // why? --> backtrack on copy and validate moves on the game
    Game game(hardest_problems[i]);
    Game game_copy = game;

    // variables for backtracking
    bool finished = false;
    // write here the sequence of moves needed to solve the problem
    vector<char> out_moves;
    // a set for holding already visited states
    unordered_set<bitset<128>> visited_game_states;
    // start backtracking
    backtrack(game_copy, visited_game_states, finished, out_moves);

    // test solution for correctness
    for (char out_move : out_moves) {
      game.move(out_move);
    }
    assert(game.is_in_final_state());
    cout << "problem " << i << ": " << out_moves.size() << " moves\n";
    assert(out_moves.size() <= 200);
    average_moves += out_moves.size();
  }
  cout << "------------------------------------" << endl;
  cout << "average moves per problem: "
       << size_t(round(average_moves / hardest_problems.size())) << endl;
  TIMERSTOP(total_time)
  return 0;
}
