#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include "../../helper.hpp"

static const std::string input = "1 10\n100 200\n201 210\n900 1000\n";

int cycle_length(int n) {
    int length = 1;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        length++;
    }
    return length;
}

int max_cycle_length(int i, int j) {
    int max_length = 0;
    for (int n = i; n <= j; n++) {
        max_length = std::max(max_length, cycle_length(n));
    }
    return max_length;
}

std::string stringify(int i, int j, int max_cycle_length) {
    return std::to_string(i) + " " +  std::to_string(j) + " " +  std::to_string(max_cycle_length);
}


std::string max_cycle_length_input(std::string in) {
    std::stringstream ss(in);
    std::string out;
    int i, j;
    while (ss >> i >> j) {
        out += stringify(i, j, max_cycle_length(i, j)) + "\n";
    }

    // remove last newline
    if (!out.empty()) {
        out.pop_back();
    }

    return out;
}


int main() {
    std::string output;
    int i, j;

    { // Testcase 1: empty intervall
        i = 1;
        j = 0;

        output = stringify(i, j, max_cycle_length(i, j));
        assert(output == "1 0 0");
    }
    { // Testcase 2: one number
        i = 1;
        j = 1;

        output = stringify(i, j, max_cycle_length(i, j));
        assert(output == "1 1 1");
    }
    {   // Testcase test string:
        output = max_cycle_length_input(input);
        assert(output == "1 10 20\n100 200 125\n201 210 89\n900 1000 174");
    }

    std::cout << "!========READING FROM FILE========!" << std::endl;
    std::string file_input = Helper::readFileIntoString("input.txt");

    std::string f_output = max_cycle_length_input(file_input);
    std::cout << f_output << std::endl;

    return EXIT_SUCCESS;
}