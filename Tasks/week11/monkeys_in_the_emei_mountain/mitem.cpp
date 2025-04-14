#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <sstream>
#include <queue>
#include "../../helper.hpp"

struct monkey
{
    int thirsty_from;
    int thirsty_until;
    int thirst;
};

struct well
{
    int capacity;
    int minimal_time = 50000;
    int maximal_time = 0;
    std::vector<monkey> monkeys;
};

struct solution
{
    std::vector<std::pair<int, int>> monkey_allocations;

    void addAllocation(const std::pair<int, int> &alloc)
    {
        monkey_allocations.push_back(alloc);
    }

    std::string getPrint()
    {
        std::string out = "";
        for (auto &alloc : monkey_allocations)
        {
            out += "(" + std::to_string(alloc.first) + "," + std::to_string(alloc.second) + ") ";
        }
        // remove last space
        out.pop_back();

        return out;
    }
};

struct solutions
{
    bool possible;
    std::vector<solution> solutions;

    void addSolution(const solution &sol)
    {
        solutions.push_back(sol);
    }

    std::string getPrint()
    {
        if (!possible)
        {
            return "No";
        }
        std::string out = "Yes\n";
        for (auto &sol : solutions)
        {
            out += std::to_string(sol.monkey_allocations.size()) + " ";
            out += sol.getPrint() + "\n";
        }

        // remove last newline
        out.pop_back();

        return out;
    }
};

std::vector<well> getCases(const std::string &input)
{
    std::vector<well> wells;
    well current_well = {};
    std::istringstream iss(input);
    std::string line;

    while (std::getline(iss, line, '\n'))
    {
        if (line == "0")
        {
            break;
        }
        // split line by space
        std::istringstream iss_line(line);
        // n = num_monkeys, m = well_capacity
        int n, m;
        iss_line >> n >> m;

        current_well.capacity = m;

        for (int i = 0; i < n; i++)
        {
            std::getline(iss, line, '\n');
            std::istringstream iss_monkey(line);
            int from, until, units;
            iss_monkey >> units >> from >> until;

            until--;

            if (from < current_well.minimal_time)
            {
                current_well.minimal_time = from;
            }
            if (until > current_well.maximal_time)
            {
                current_well.maximal_time = until;
            }

            monkey current_monkey = {from, until, units};
            current_well.monkeys.push_back(current_monkey);
        }

        wells.push_back(current_well);
        current_well = {};
    }

    return wells;
}

/* ============================================== MaxFlow from lecture ============================================== */

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
    // std::vector<std::vector<int>> E = E;

    // initialize some variables for computing the flow:
    int total_flow_so_far = 0;
    std::vector<int> current_path;
    int flow_of_current_path;
    while (true)
    {
        // look for a new shortest path that uses the remaining capacities:
        current_path = get_path(E, s, t);
        if (current_path.empty())
        {
            // case no path was found. Algorithm is finished:
            break;
        }
        // compute maximum flow on current path: find edge with minimum remaining capacity
        flow_of_current_path = E[current_path[0]][current_path[1]];
        // iterate through path, check each edge of path and find the minimum remaining capacity:
        for (int i = 1; i < static_cast<int>(current_path.size()) - 1; i++)
        {
            if (flow_of_current_path > E[current_path[i]][current_path[i + 1]])
            {
                flow_of_current_path = E[current_path[i]][current_path[i + 1]];
            }
        }
        // add new flow to total flow:
        total_flow_so_far += flow_of_current_path;
        // adjust the remaining capacities:
        for (int i = 0; i < static_cast<int>(current_path.size()) - 1; i++)
        {
            E[current_path[i]][current_path[i + 1]] -= flow_of_current_path;
            E[current_path[i + 1]][current_path[i]] += flow_of_current_path;
        }
    }
    return total_flow_so_far;
}

/* =============================================== Actual calculations ============================================== */

std::vector<std::vector<int>> constructMatrix(const well &well)
{
    int n = well.monkeys.size();
    int m = well.maximal_time - well.minimal_time + 1;
    int size = n + m + 2; // monkeys, time units + sink +source
    int idx_time = n + 1; // index of first itme unit in matrix

    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 0));

    // add edges from source to monkeys
    for (int i = 1; i <= n; i++)
    {
        matrix[0][i] = well.monkeys[i - 1].thirst;
    }
    // add edges from time to sink
    for (int i = idx_time; i < size - 1; i++)
    {
        matrix[i][size - 1] = well.capacity;
    }
    // add edges from monkeys to time
    for (int i = 1; i <= n; i++)
    {
        for (int j = well.monkeys[i - 1].thirsty_from - well.minimal_time + idx_time; j <= well.monkeys[i - 1].thirsty_until - well.minimal_time + idx_time; j++)
        {
            matrix[i][j] = 1;
        }
    }

    return matrix;
}

solutions getSolution(const well &well)
{
    solutions sols = {};
    // Idea: construct a matrix with source, monkeys, time units and sink
    // source is connected to monkeys with their thirst
    // monkeys are connected to time units with capacity 1
    // time units are connected to sink with well capacity
    auto matrix = constructMatrix(well);
    // Modify matrix to get schedules and maxflow
    int maxflow = compute_MaxFlow_EK(matrix, 0, matrix.size() - 1);
    // if maxflow is not equal to number of all monkeys thirst, return no solution
    int all_thirst = 0;
    for (auto &monkey : well.monkeys)
    {
        all_thirst += monkey.thirst;
    }
    if (maxflow != all_thirst)
    {
        sols.possible = false;
        return sols;
    }
    // else, construct solution
    sols.possible = true;

    int n = well.monkeys.size();
    int start_interval_idx = 0;
    int end_interval_idx = 0;

    // For each monkey
    for (int j = 1; j <= n; j++)
    {
        solution sol = {};
        // Go over the time units

        int i = n + 1;
        while (i < static_cast<int>(matrix[j].size()))
        {
            // If there is a flow from monkey to time unit
            if (matrix[i][j] == 1 && start_interval_idx == 0)
            {
                start_interval_idx = i;
            }
            // If there is no flow from monkey to time unit
            else if (matrix[i][j] == 0 && start_interval_idx != 0)
            {
                end_interval_idx = i - 1;
                sol.addAllocation(std::make_pair(
                    start_interval_idx - (n + 1) + well.minimal_time,
                    end_interval_idx - (n + 1) + well.minimal_time + 1));
                start_interval_idx = 0;
            }
            i++;
        }

        sols.addSolution(sol);
    }

    return sols;
}

int main()
{
    
    std::cout << "!========READING FROM FILE========!" << std::endl;
    std::string file_input = Helper::readFileIntoString("input.txt");
    std::vector<well> cases = getCases(file_input);

    // get solutions for all wells
    for (auto &well : cases)
    {
        // Print "Case i: " with i starting from 1
        static int case_number = 1;
        std::cout << "Case " << case_number++ << ": ";
        auto sol = getSolution(well);
        std::cout << sol.getPrint() << std::endl;
    }
    return EXIT_SUCCESS;
}