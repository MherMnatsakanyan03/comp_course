#include <iostream>
#include <cassert>
#include <cstdint>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>

#define __ __LONG_MAX__

using matrix_t = std::vector<std::vector<long>>;

long current_weight(const matrix_t &matrix)
{
    // calculate the length by summing all the weights of the none-negative edges
    // on the upper triangle of the matrix
    long weight = 0;
    
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = i + 1; j < matrix.size(); ++j)
        {
            if (matrix[i][j] != __)
            {
                weight += matrix[i][j];
            }
        }
    }

    return weight;
}

long minimal_network(const matrix_t &matrix, long n)
{
    // Vector to check if a node is included in the Minimum Spanning Tree (MST)
    std::vector<bool> inMST(n, false);

    // Vector to store the minimum edge weight to add a node to the MST
    std::vector<long> key(n, __);

    // Start with the first node, so set its key to 0
    key[0] = 0;

    // Priority queue to pick the minimum weight edge at each step
    // The priority queue stores pairs (weight, vertex)
    std::priority_queue<std::pair<long, long>, std::vector<std::pair<long, long>>, std::greater<>> pq;
    pq.push({0, 0}); // Start with the first node (index 0) with weight 0

    long total_weight = 0; // Initialize the total weight of the MST

    // Loop until the priority queue is empty
    while (!pq.empty())
    {
        // Extract the vertex with the minimum key value
        long u = pq.top().second;
        pq.pop();

        // If the vertex is already included in the MST, skip it
        if (inMST[u])
            continue;

        // Include the vertex in the MST
        inMST[u] = true;

        // Add the key value to the total weight of the MST
        total_weight += key[u];

        // Update the key values for the adjacent vertices
        for (long v = 0; v < n; ++v)
        {
            // Update the key only if v is not in MST, there is an edge from u to v,
            // and the weight of the edge (u, v) is smaller than the current key of v
            if (matrix[u][v] != __ && !inMST[v] && matrix[u][v] < key[v])
            {
                key[v] = matrix[u][v];
                pq.push({key[v], v});
            }
        }
    }

    return total_weight; // Return the total weight of the MST
}

matrix_t readMatrixFromFile(const std::string &filename, long &n)
{
    std::ifstream file(filename);
    std::string line;
    matrix_t matrix;

    // Read the matrix from the file
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string value;
        std::vector<long> row;

        while (std::getline(iss, value, ','))
        {
            long weight = std::stol(value);
            row.push_back(weight == -1 ? __ : weight);
        }

        // Add the row to the matrix
        matrix.push_back(row);
    }

    // Set n to the size of the matrix
    n = matrix.size();

    return matrix;
}

int main()
{
    {
        matrix_t matrix = {{
            {__, 16, 12, 21, __, __, __},
            {16, __, __, 17, 20, __, __},
            {12, __, __, 28, __, 31, __},
            {21, 17, 28, __, 18, 19, 23},
            {__, 20, __, 18, __, __, 11},
            {__, __, 31, 19, __, __, 27},
            {__, __, __, 23, 11, 27, __},
        }};

        assert(current_weight(matrix) == 243);
        assert(minimal_network(matrix, 7) == 93);
        long savings = current_weight(matrix) - minimal_network(matrix, 7);
        assert(savings == 150);
    }

    long n;
    matrix_t matrix = readMatrixFromFile("network.txt", n);

    long current = current_weight(matrix);
    long minimal = minimal_network(matrix, n);
    std::cout << "Current weight: " << current << std::endl;
    std::cout << "Minimal weight: " << minimal << std::endl;

    std::cout << "Saving: " << current - minimal << std::endl;

    std::cout << "All tests passed successfully!\n";
    return EXIT_SUCCESS;
}
