#include <iostream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <queue>
#include <sstream>
#include "../../helper.hpp"

#define ASSERT_EQUAL(actual, expected) Helper::assertEqual<size_t>(actual, expected, __FILE__, __LINE__)

std::vector<int> get_path(std::vector<std::vector<int>> &E, int s, int t)
{
    // use BFS to find a path from s to t
    // initialize number of nodes:
    int n = E.size();
    // initialize a FIFO-queue:
    std::queue<int> q;
    // initialize data structure of visitied nodes:
    // -1: unvisited, >= 0: in queue or visited, this also stores the index of the parent node
    std::vector<int> visited(n, -1);

    // start with node s, set s as its own parent to mark that it has been visited
    q.push(s);
    visited[s] = s;
    // go through the graph until queue is empty or t is reached:
    int current_node = s;
    while (!q.empty() && current_node != t)
    {
        // get first element from queue and remove it from queue:;
        current_node = q.front();
        q.pop();

        // add all unqueued neighbors of the current node to the queue:
        for (int i = 0; i < n; i++)
        {
            if (E[current_node][i] > 0)
            {
                if (visited[i] < 0)
                {
                    q.push(i);
                    // set neighbor as queued:
                    visited[i] = current_node;
                }
            }
        }
    }

    // case no path to t was found:
    if (current_node != t)
    {
        // return empty path
        return std::vector<int>(0);
    }

    // else: reconstruct path from s to t by parents stored in visited:
    current_node = t;
    // init the path, it only contains t at first:
    std::vector<int> path(1, t);
    while (!(current_node == s))
    {
        current_node = visited[current_node];
        path.insert(path.begin(), current_node);
    }
    return path;
}

int compute_MaxFlow_EK(std::vector<std::vector<int>> &E, int s, int t)
{
    // initialize the data structure for the remaining capacities:
    std::vector<std::vector<int>> remaining_capacities = E;

    // initialize some variables for computing the flow:
    int total_flow_so_far = 0;
    std::vector<int> current_path;
    int flow_of_current_path;
    while (true)
    {
        // look for a new shortest path that uses the remaining capacities:
        current_path = get_path(remaining_capacities, s, t);
        if (current_path.empty())
        {
            // case no path was found. Algorithm is finished:
            break;
        }
        // compute maximum flow on current path: find edge with minimum remaining capacity
        flow_of_current_path = remaining_capacities[current_path[0]][current_path[1]];
        // iterate through path, check each edge of path and find the minimum remaining capacity:
        for (size_t i = 1; i < current_path.size() - 1; i++)
        {
            if (flow_of_current_path > remaining_capacities[current_path[i]][current_path[i + 1]])
            {
                flow_of_current_path = remaining_capacities[current_path[i]][current_path[i + 1]];
            }
        }
        // add new flow to total flow:
        total_flow_so_far += flow_of_current_path;
        // adjust the remaining capacities:
        for (size_t i = 0; i < current_path.size() - 1; i++)
        {
            remaining_capacities[current_path[i]][current_path[i + 1]] -= flow_of_current_path;
            remaining_capacities[current_path[i + 1]][current_path[i]] += flow_of_current_path;
        }
    }
    return total_flow_so_far;
}

std::vector<std::string> getCases(const std::string &input)
{
    std::vector<std::string> cases;
    std::string current_case = "";
    std::istringstream iss(input);
    std::string line;

    while (std::getline(iss, line, '\n'))
    {
        if (line == "0 0")
        {
            break;
        }
        // split line by space
        std::istringstream iss_line(line);
        // m = first word, w = second word
        int m, w;
        iss_line >> m >> w;
        int first = m - 2;
        int second = w;

        for (int i = 0; i < first; i++)
        {
            std::getline(iss, line, '\n');
            current_case += line + "\n";
        }

        for (int i = 0; i < second; i++)
        {
            std::getline(iss, line, '\n');
            current_case += line + "\n";
        }

        cases.push_back(current_case);
        current_case = "";
    }

    return cases;
}

std::vector<std::vector<int>> caseToMatrix(const std::string &input)
{
    std::istringstream iss(input);
    std::string line;

    std::vector<std::string> machines;
    std::vector<std::string> wires;

    while (std::getline(iss, line, '\n'))
    {
        if (line.length() == 3)
        {
            machines.push_back(line);
        }
        else
        {
            wires.push_back(line);
        }
    }

    int m = machines.size() + 2;

    std::vector<std::vector<int>> matrix(2 * m, std::vector<int>(2 * m, 0));

    // add infinite capacity to source and sink
    matrix[0][1] = 10000;
    matrix[1][0] = 10000;
    matrix[2 * m - 2][2 * m - 1] = 10000;
    matrix[2 * m - 1][2 * m - 2] = 10000;

    for (auto &machine : machines)
    {
        // self cost
        int machine_number = machine[0] - '0';
        int machine_cost = machine[2] - '0';
        matrix[2 * machine_number - 1][2 * machine_number - 2] = machine_cost;
        matrix[2 * machine_number - 2][2 * machine_number - 1] = machine_cost;
    }

    for (auto &wire : wires)
    {
        int machine1 = wire[0] - '0';
        int machine2 = wire[2] - '0';
        int cost = wire[4] - '0';
        matrix[2 * machine1 - 1][2 * machine2 - 2] = cost;
        matrix[2 * machine2 - 2][2 * machine1 - 1] = cost;
    }

    return matrix;
}

int main()
{
    std::cout << "!========READING FROM FILE========!" << std::endl;
    std::string file_input = Helper::readFileIntoString("input.txt");
    std::vector<std::string> cases = getCases(file_input);
    
    for (auto &c : cases)
    {
        std::vector<std::vector<int>> matrix = caseToMatrix(c);
        int result = compute_MaxFlow_EK(matrix, 0, matrix.size() - 1);
        std::cout << result << std::endl;
    }

    return EXIT_SUCCESS;
}