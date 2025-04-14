#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <sstream>
#include <algorithm>
#include <random>
#include <numeric>
#include <ctime>
#include "../helper.hpp"

std::vector<std::vector<int>> get_input(const std::string &in)
{
    std::vector<std::vector<int>> result;
    std::istringstream iss(in);

    int case_count;

    iss >> case_count;
    // skip first newline
    iss.ignore(1);

    // Read each case
    for (int i = 0; i < case_count; i++)
    {
        int n;
        iss >> n;
        std::vector<int> case_data(n);
        // skip newline
        iss.ignore(1);
        for (int j = 0; j < n; j++)
        {
            iss >> case_data[j];
        }
        result.push_back(case_data);
    }

    return result;
}

std::vector<int> get_permutation(size_t size)
{
    std::vector<int> result(size);

    // Fill the vector with numbers 1 to size
    std::iota(result.begin(), result.end(), 1);

    // Create a random device and generator
    std::random_device rd;
    std::mt19937 g(rd());

    // Shuffle the vector
    do
    {
        std::shuffle(result.begin(), result.end(), g);
    } while (std::any_of(result.begin(), result.end(), [&, i = 0](int x) mutable
                         { return x == ++i; }));

    return result;
}

std::pair<std::vector<int>, std::vector<int>> get_output(const std::vector<int> &cards)
{
    // Idea: choose first permutation based on the restraints randomly and check,
    // if a second permutation can be created from the first one with the restraints
    // if not, choose another first permutation and try again
    // Restrain: the i-th element of each permutation cannot be i
    std::pair<std::vector<int>, std::vector<int>> result;
    size_t n = cards.size();
    std::vector<int> new_list(n);

    // Attempt to find valid permutations 200 times
    for (int tries = 0; tries < 200; ++tries)
    {
        // First permutation
        std::vector<int> first_permutation = get_permutation(n);

        // Write the first permutation to the new list
        for (size_t i = 0; i < n; i++)
        {
            new_list[i] = cards[first_permutation[i] - 1];
        }

        // Second permutation: Chance of it being fitting are ~1/e (source: online)
        std::vector<int> second_permutation(n);
        bool valid = true;

        // Check if the second permutation can be created from the first one
        for (size_t i = 0; i < n; i++)
        {
            second_permutation[new_list[i] - 1] = i + 1;

            if (second_permutation[i] == static_cast<int>(i + 1))
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            result.first = first_permutation;
            result.second = second_permutation;
            return result;
        }
    }

    throw std::runtime_error("After 200 tries couldn't find valid permutations");
}

int main()
{
    std::cout << "!========READING FROM FILE========!" << std::endl;
    std::string file_input = Helper::readFileIntoString("input.txt");
    std::vector<std::vector<int>> cases = get_input(file_input);

    for (const auto &c : cases)
    {
        try
        {
            std::pair<std::vector<int>, std::vector<int>> result = get_output(c);
            std::cout << "Possible\n";
            for (int i : result.first)
            {
                std::cout << i << " ";
            }
            std::cout << std::endl;
            for (int i : result.second)
            {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Impossible" << '\n';
        }
    }

    return EXIT_SUCCESS;
}