#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include "../helper.hpp"

// Source: https://cp-algorithms.com/graph/2SAT.html
// How it works:
//     - convert the problem to a graph, where the CNF is transformed to an implication graph
//     - for each conjucture (a or b), add two implications: -a -> b and -b -> a
//     - run a dfs on the graph and assign components to each vertex
//     - if a vertex and its negation are in the same component, the 2-SAT is unsatisfiable
//     - otherwise, the components are sorted in topological order, that is we go from "left to right"
//     - if component(x) < component(not x), then x is false, otherwise x is true

struct TwoSatSolver
{
    int n_vars;
    int n_vertices;
    std::vector<std::vector<int>> adj, adj_t;
    std::vector<bool> used;
    std::vector<int> order, comp;
    std::vector<bool> assignment;

    TwoSatSolver(int _n_vars) : n_vars(_n_vars),
                                n_vertices(2 * n_vars),
                                // adj(n_vertices),
                                // adj_t(n_vertices),
                                used(n_vertices),
                                order(),
                                comp(n_vertices, -1),
                                assignment(n_vars)
    {
        order.reserve(n_vertices);
    }
    
    void dfs1(int v)
    {
        used[v] = true;
        for (int u : adj[v])
        {
            if (!used[u])
                dfs1(u);
        }
        order.push_back(v);
    }

    void dfs2(int v, int cl)
    {
        comp[v] = cl;
        for (int u : adj_t[v])
        {
            if (comp[u] == -1)
                dfs2(u, cl);
        }
    }

    bool solve_2SAT()
    {
        order.clear();
        used.assign(n_vertices, false);
        for (int i = 0; i < n_vertices; ++i)
        {
            if (!used[i])
                dfs1(i);
        }

        comp.assign(n_vertices, -1);
        for (int i = 0, j = 0; i < n_vertices; ++i)
        {
            int v = order[n_vertices - i - 1];
            if (comp[v] == -1)
                dfs2(v, j++);
        }

        assignment.assign(n_vars, false);
        for (int i = 0; i < n_vertices; i += 2)
        {
            if (comp[i] == comp[i + 1])
                return false;
            assignment[i / 2] = comp[i] > comp[i + 1];
        }
        return true;
    }

    void add_disjunction(int a, bool na, int b, bool nb)
    {
        // na and nb signify whether a and b are to be negated
        a = 2 * a ^ na;
        b = 2 * b ^ nb;
        int neg_a = a ^ 1;
        int neg_b = b ^ 1;
        adj[neg_a].push_back(b);
        adj[neg_b].push_back(a);
        adj_t[b].push_back(neg_a);
        adj_t[a].push_back(neg_b);
    }

    static void example_usage()
    {
        TwoSatSolver solver(3);                     // a, b, c
        solver.add_disjunction(0, false, 1, true);  //     a  v  not b
        solver.add_disjunction(0, true, 1, true);   // not a  v  not b
        solver.add_disjunction(1, false, 2, false); //     b  v      c
        solver.add_disjunction(0, false, 0, false); //     a  v      a
        assert(solver.solve_2SAT() == true);
        auto expected = std::vector<bool>{true, false, true};
        assert(solver.assignment == expected);
    }
};

/*
n people
m people in the government

vote: +-i +-j for i,j in [1,n], + for keep, - for remove

voter satisfied, if at least one wish is satisfied

task: form council of m people, so that all voters are satisfied
*/

typedef std::vector<std::vector<int>> AdjList;

std::vector<std::vector<std::string>> getCases(const std::string &input)
{
    std::vector<std::vector<std::string>> cases;
    std::istringstream iss(input);
    std::string line;

    // skip first line
    std::getline(iss, line, '\n');

    while (std::getline(iss, line, '\n'))
    {
        int n, m;
        std::istringstream iss_line(line);
        iss_line >> n >> m;

        std::vector<std::string> current_case;
        // add first line to current_case
        current_case.push_back(line);

        // for each case, we need to read n lines
        for (int i = 0; i < n; i++)
        {
            std::getline(iss, line, '\n');
            current_case.push_back(line);
        }

        cases.push_back(current_case);
    }

    return cases;
}

std::pair<AdjList, AdjList> caseToAdjLists(const std::vector<std::string> &i_case)
{
    // first line is n m
    std::string first_line = i_case[0];
    std::istringstream iss_first_line(first_line);
    int n, m;
    iss_first_line >> n >> m;

    AdjList edges(2 * m);
    AdjList reverse_edges(2 * m);

    for (int i = 1; i < n + 1; i++)
    {
        std::istringstream iss(i_case[i]);
        // get all single characters aside from spaces
        int x, y;
        char sign_x, sign_y;
        iss >> sign_x >> x >> sign_y >> y;

        // the index is 2 * x + (sign_x == '+') and 2 * y + (sign_y == '+')
        // thats because we first stor the positive votes, then the negative votes
        // so the amount for each member of council doubles, written pos1, neg1, pos2, neg2, ...
        x = 2 * (x - 1) + (sign_x == '-');
        y = 2 * (y - 1) + (sign_y == '-');

        // for each vote, we need to add two implications
        // save the vote "x or y" as "-x -> y" and "-y -> x"
        // if we xor the votes with 1, we get the opposite vote
        edges[x ^ 1].push_back(y);
        edges[y ^ 1].push_back(x);
        // we need the reverse edges
        reverse_edges[y].push_back(x ^ 1);
        reverse_edges[x].push_back(y ^ 1);
    }

    return {edges, reverse_edges};
}

int main()
{
    std::cout << "!========READING FROM FILE========!" << std::endl;
    std::string file_input = Helper::readFileIntoString("input.txt");

    auto cases = getCases(file_input);
    for (auto &i_case : cases)
    {
        auto [edges, reverse_edges] = caseToAdjLists(i_case);
        TwoSatSolver solver(edges.size() / 2);
        solver.adj = edges;
        solver.adj_t = reverse_edges;

        bool result = solver.solve_2SAT();
        if (!result) {
            std::cout << "Case " << (&i_case - &cases[0] + 1) << ": NO" << std::endl;
        } else {
            std::cout << "Case " << (&i_case - &cases[0] + 1) << ": YES" << std::endl;
            
            int count = 0;
            for (bool b : solver.assignment) {
                if (b) count++;
            }
            std::cout << count << " ";
            for (size_t i = 0; i < solver.assignment.size(); i++) {
                if (solver.assignment[i])
                    std::cout << i + 1 << " ";
            }
            std::cout << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
