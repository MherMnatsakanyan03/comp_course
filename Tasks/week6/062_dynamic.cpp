// Dynamic programming is a very important problem solving strategy in
// competitive programming problems.
//
// In the backtracking assignment you had to work with code you didn't write.
// In this assignment you write everything yourself from the beginning.
//
// TODO: Read chapter 7 from the "Competitive Programmer’s Handbook"
// https://cses.fi/book/book.pdf --> pp. 65 - 76
// TODO: Choose one of the three following problems presented in the book:
//       - Longest increasing subsequence
//       - Paths in a grid
//       - Knapsack (all possible sums)
//       Implement for your chosen problem with dynamic programming
//       1. an iterative solution
//                and
//       2. a recursive solution
//       Test your implementations thoroughly. Start by writing your test cases
//       before you implement the algorithms.
//       Document your solutions thoroughly by writing meaningful comments.

// KnapSack problem

/*
    The term knapsack refers to problems where a set of objects is given, and
    subsets with some properties have to be found. Knapsack problems can often be
    solved using dynamic programming.
    In this section, we focus on the following problem: Given a list of weights
    [w1, w2, . . . , wn], determine all sums that can be constructed using the weights.

    For example, if the weights are [1, 3, 3, 5], the following sums are possible:
    0 1 2 3 4 5 6 7 8 9 10 11 12
    X X   X X X X X X X     X  X
    In this case, all sums between 0 . . . 12 are possible, except 2 and 10. For
    example, the sum 7 is possible because we can select the weights [1, 3, 3].
    To solve the problem, we focus on subproblems where we only use the ﬁrst k
    weights to construct sums. Let possible(x, k) = true if we can construct a sum x
    using the ﬁrst k weights, and otherwise possible(x, k) = false. The values of the
    function can be recursively calculated as follows:

    possible(x, k) = possible(x − wk, k − 1) ∨ possible(x, k − 1)

    The formula is based on the fact that we can either use or not use the weight wk
    in the sum. If we use wk, the remaining task is to form the sum x − wk using the
    ﬁrst k − 1 weights, and if we do not use wk, the remaining task is to form the sum
    x using the ﬁrst k − 1 weights. As the base cases,
    possible(x, 0) = true if x = 0, else false
    because if no weights are used, we can only form the sum 0.
    The following table shows all values of the function for the weights [1, 3, 3, 5]
    (the symbol ”X” indicates the true values):
    k\x 0 1 2 3 4 5 6 7 8 9 10 11 12
    0   X
    1   X X
    2   X X   X X
    3   X X   X X   X X
    4   X X   X X X X X X X     X  X
    After calculating those values, possible(x, n) tells us whether we can con-
    struct a sum x using all weights.
    Let W denote the total sum of the weights. The following O(nW) time dynamic
    programming solution corresponds to the recursive function:

    possible[0][0] = true;
    for (int k = 1; k <= n; k++) {
        for (int x = 0; x <= W; x++) {
            if (x-w[k] >= 0) possible[x][k] |= possible[x-w[k]][k-1];
                possible[x][k] |= possible[x][k-1];
        }
    }

    However, here is a better implementation that only uses a one-dimensional
    array possible[x] that indicates whether we can construct a subset with sum x.
    The trick is to update the array from right to left for each new weight:

    possible[0] = true;
    for (int k = 1; k <= n; k++) {
        for (int x = W; x >= 0; x--) {
            if (possible[x]) possible[x+w[k]] = true;
        }
    }

    Note that the general idea presented here can be used in many knapsack
    problems. For example, if we are given objects with weights and values, we can
    determine for each weight sum the maximum value sum of a subset.
 */

#include <iostream>
#include <vector>
#include <cassert>

// Iterative solution
std::vector<bool> knapsackIterative(const std::vector<int> &weights, int W)
{
    std::vector<bool> possible(W + 1, false); // +1 to include 0
    possible[0] = true;                       // Base case, we can always form the sum 0
    // Iterate through all number of weights
    for (size_t k = 0; k < weights.size(); ++k)
    {
        // Iterate through all possible sums
        for (int x = W; x >= 0; --x)
        {
            // If we can form the sum x with the previous weights, we can also form it with the current weight
            if (possible[x - weights[k]])
            {
                possible[x] = true;
            }
        }
    }
    return possible;
}

// Recursive solution
bool knapsackRecursiveHelper(std::vector<std::vector<int>> &memo, int x, int k, const std::vector<int> &weights)
{
    if (x == 0) // Base case, we can always form the sum 0
        return true;
    if (k == 0) // If no items left and x != 0, we cannot form the sum
        return false;
    if (memo[x][k] != -1) // If the value is already calculated, return it
        return memo[x][k];

    // Check if we can form the sum x with the previous weights
    bool withoutCurrent = knapsackRecursiveHelper(memo, x, k - 1, weights);
    // Check if we can form the sum x with the previous weights and the current weight
    bool withCurrent = (x >= weights[k - 1]) ? knapsackRecursiveHelper(memo, x - weights[k - 1], k - 1, weights) : false;

    // Store the result in the memoization table, so we don't have to recalculate it
    memo[x][k] = (withoutCurrent || withCurrent);
    return memo[x][k];
}

std::vector<bool> knapsackRecursive(const std::vector<int> &weights, int W)
{
    std::vector<bool> possible(W + 1, false);
    // Initialize memoization table, -1 means not calculated yet
    std::vector<std::vector<int>> memo = std::vector<std::vector<int>>(W + 1, std::vector<int>(weights.size() + 1, -1));
    for (int x = 0; x <= W; ++x)
    {
        possible[x] = knapsackRecursiveHelper(memo, x, weights.size(), weights);
    }
    return possible;
}

int main()
{
    { // Edge case: No elements
        std::cout << "Edge case: No elements" << std::endl;
        std::vector<int> weights = {};
        int W = 5;
        std::vector<bool> iterativeResult = knapsackIterative(weights, W);
        std::vector<bool> recursiveResult = knapsackRecursive(weights, W);
        std::vector<bool> expected = {true, false, false, false, false, false};
        for (int i = 0; i <= W; ++i)
        {
            assert((iterativeResult[i] == expected[i]) && (recursiveResult[i] == expected[i]));
            std::cout << "Sum " << i << ": " << (iterativeResult[i] ? "Possible" : "Not Possible") << std::endl;
        }
        std::cout << std::endl;
    }
    { // Edge case: Single element equal to the total weight
        std::cout << "Edge case: Single element equal to the total weight" << std::endl;
        std::vector<int> weights = {10};
        int W = 10;
        std::vector<bool> iterativeResult = knapsackIterative(weights, W);
        std::vector<bool> recursiveResult = knapsackRecursive(weights, W);
        std::vector<bool> expected = {true, false, false, false, false, false, false, false, false, false, true};
        for (int i = 0; i <= W; ++i)
        {
            assert((iterativeResult[i] == expected[i]) && (recursiveResult[i] == expected[i]));
            std::cout << "Sum " << i << ": " << (iterativeResult[i] ? "Possible" : "Not Possible") << std::endl;
        }
        std::cout << std::endl;
    }
    { // Test case from the task description
        std::cout << "Test case from the task description" << std::endl;
        std::vector<int> weights = {1, 3, 3, 5};
        int W = 12;

        std::vector<bool> iterativeResult = knapsackIterative(weights, W);
        std::vector<bool> recursiveResult = knapsackRecursive(weights, W);
        std::vector<bool> expected = {true, true, false, true, true, true, true, true, true, true, false, true, true};

        for (int i = 0; i <= W; ++i)
        {
            assert((iterativeResult[i] == expected[i]) && (recursiveResult[i] == expected[i]));
            std::cout << "Sum " << i << ": " << (iterativeResult[i] ? "Possible" : "Not Possible") << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "All test cases passed!" << std::endl;
    return 0;
}
