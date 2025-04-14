#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../../helper.hpp"

#define ASSERT_EQUAL(actual, expected) Helper::assertEqual<size_t>(actual, expected, __FILE__, __LINE__)

struct edge
{
    char a;
    char b;
};


std::vector<std::string> extractCases(const std::string &input)
{
    // Initialize a vector to store the separated cases
    std::vector<std::string> cases;

    // Find the first newline character
    size_t pos = input.find("\n");
    std::stoi(input.substr(0, pos));

    // Initialize a string stream with the remaining input
    std::string remainingInput = input.substr(pos + 2); // Skip the first line and the initial double newline
    std::stringstream ss(remainingInput);

    std::string caseData;
    std::string line;
    while (std::getline(ss, line))
    {
        if (line.empty())
        {
            // If we encounter an empty line, we consider it as the end of a case
            cases.push_back(caseData);
            caseData.clear();
        }
        else
        {
            // Otherwise, we accumulate the lines into the current case
            if (!caseData.empty())
            {
                caseData += "\n";
            }
            caseData += line;
        }
    }
    // Add the last case if not already added
    if (!caseData.empty())
    {
        cases.push_back(caseData);
    }

    return cases;
}

std::vector<edge> extractEdges(const std::string &caseData)
{
    std::vector<edge> edges;
    std::stringstream ss(caseData);
    std::string line;
    // Skip the first line
    std::getline(ss, line);
    while (std::getline(ss, line))
    {
        edge e;
        e.a = line[0];
        e.b = line[1];
        edges.push_back(e);
    }
    return edges;
}

int findSubgraphs(std::vector<edge> &edges) {
    int count = 0;
    std::vector<std::vector<char>> graph;

    for (auto &edge : edges) {
        int found = 0;
        std::vector<int> subgraphIndices;

        // Step 2: Check if the nodes are already in a subgraph
        for (size_t i = 0; i < graph.size(); ++i) {
            auto &subgraph = graph[i];
            if (std::find(subgraph.begin(), subgraph.end(), edge.a) != subgraph.end() ||
                std::find(subgraph.begin(), subgraph.end(), edge.b) != subgraph.end()) {
                found++;
                subgraphIndices.push_back(i);
            }
        }

        // Step 3: If no, then create a new subgraph
        if (found == 0) {
            graph.push_back({ edge.a, edge.b });
            count++;
        } else {
            // Step 4: If edge found in one subgraph, add the other node to the subgraph
            int mergeIndex = subgraphIndices[0];
            auto &mergeSubgraph = graph[mergeIndex];
            if (std::find(mergeSubgraph.begin(), mergeSubgraph.end(), edge.a) == mergeSubgraph.end()) {
                mergeSubgraph.push_back(edge.a);
            }
            if (std::find(mergeSubgraph.begin(), mergeSubgraph.end(), edge.b) == mergeSubgraph.end()) {
                mergeSubgraph.push_back(edge.b);
            }

            // Step 5: If edge found in multiple subgraphs, merge the subgraphs
            for (size_t i = 1; i < subgraphIndices.size(); ++i) {
                int otherIndex = subgraphIndices[i];
                auto &otherSubgraph = graph[otherIndex];
                mergeSubgraph.insert(mergeSubgraph.end(), otherSubgraph.begin(), otherSubgraph.end());
                graph.erase(graph.begin() + otherIndex);
                count--;
            }
        }
    }

    return count;
}

int main()
{
    { // Testcase 0
        const std::string input0 = "0\n\n";
        auto cases = extractCases(input0);
        ASSERT_EQUAL(cases.size(), 0);
    }

    { // Testcase 1
        const std::string input1 = "1\n\nE\nAB\nCE\nDB\nEC";
        auto cases = extractCases(input1);
        
        ASSERT_EQUAL(cases.size(), 1);
        auto edges = extractEdges(cases[0]);
        ASSERT_EQUAL(edges.size(), 4);
        auto subgraphs = findSubgraphs(edges);
        ASSERT_EQUAL(subgraphs, 2);
    }

    { // Testcase 2
        const std::string input2 = "2\n\nD\nAB\nCD\n\nE\nAB\nCE\nDB\nEC";
        auto cases = extractCases(input2);
        ASSERT_EQUAL(cases.size(), 2);

        auto edges1 = extractEdges(cases[0]);
        ASSERT_EQUAL(edges1.size(), 2);
        auto subgraphs1 = findSubgraphs(edges1);
        ASSERT_EQUAL(subgraphs1, 2);

        auto edges2 = extractEdges(cases[1]);
        ASSERT_EQUAL(edges2.size(), 4);
        auto subgraphs2 = findSubgraphs(edges2);
        ASSERT_EQUAL(subgraphs2, 2);
    }

    std::cout << "!========READING FROM FILE========!" << std::endl;
    std::string file_input = Helper::readFileIntoString("input.txt");
    auto cases = extractCases(file_input);
    for (auto &caseData : cases) {
        auto edges = extractEdges(caseData);
        auto subgraphs = findSubgraphs(edges);
        std::cout << subgraphs << std::endl;
    }

    return EXIT_SUCCESS;
}