#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <vector>

// A point consists out of an array of coordinates and a payload of arbitrary type
template <size_t dimension, typename T = int>
using Point = std::pair<std::array<double, dimension>, T>;

// A region is a cartesian product of d closed intervals
template <size_t dimension>
using Region = std::array<std::pair<double, double>, dimension>;

template <size_t dimension, typename T = int>
struct Node
{
    // The point which is representative for the node.
    // If the node is a leaf, the actual point is interesting. Otherwise, only the coordinate given in axis.
    Point<dimension, T> point;
    size_t axis;

    // unique pointers to children nodes for memory safety
    // both null if it is a leaf
    std::unique_ptr<Node<dimension, T>> left_child;
    std::unique_ptr<Node<dimension, T>> right_child;
};

template <size_t dimension, typename T = int>
class KDTree
{
    // Computes the usual Euclidean distance between the two points p1 and p2
    double distance(Point<dimension, T> p1, Point<dimension, T> p2) const
    {
        double d = 0;
        for (size_t i = 0; i < dimension; ++i)
        {
            d += (p1.first[i] - p2.first[i]) * (p1.first[i] - p2.first[i]);
        }

        return std::sqrt(d);
    }

    // For a given node, verify if a node is a leaf, e.g. both child pointers are null
    bool isLeaf(const std::unique_ptr<Node<dimension, T>> &node) const
    {
        return node->left_child == nullptr && node->right_child == nullptr;
    }

    // Returns a integer value depending if interval a and interval b
    // Do not intersect: return 0
    // do intersect:    return 1
    // a is completly contained in b: return 2
    int intersect(std::pair<double, double> interval_a, std::pair<double, double> interval_b) const
    {
        if (interval_a.first >= interval_b.first && interval_a.second <= interval_b.second)
        {
            return 2;
        }
        else if (interval_a.second < interval_b.first || interval_a.first > interval_b.second)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    // Do the intersection test for a d-dimensional region
    // Same return signature as in the test for intervals
    int containsRegion(const Region<dimension> &smaller_region, const Region<dimension> &larger_region) const
    {
        int intersection_state = 2;
        for (size_t i = 0; i < dimension; ++i)
        {
            intersection_state = std::min(intersection_state, intersect(smaller_region[i], larger_region[i]));
        }
        return intersection_state;
    }

    // Go through the subtree rooted at the node and report all points within this tree
    std::list<Point<dimension, T>> reportSubtree(const std::unique_ptr<Node<dimension, T>> &node) const
    {
        if (isLeaf(node))
        {
            return std::list<Point<dimension, T>>({node->point});
        }
        auto points = reportSubtree(node->left_child);
        auto right_points = reportSubtree(node->right_child);
        points.insert(points.end(), right_points.begin(), right_points.end());
        return points;
    }

    // Recursive function to build up the kd tree
    //
    std::unique_ptr<Node<dimension, T>> construct(std::vector<Point<dimension, T>> &points, size_t start_index, size_t stop_index, size_t depth)
    {
        // Create a leaf if we consider only one point out of the point set.
        if (stop_index - start_index == 1)
        {
            return std::make_unique<Node<dimension, T>>(points[start_index], 0, nullptr, nullptr);
        }

        // Partition by alternating hyperplane

        // Determine the axis
        size_t axis = depth % dimension;

        // Compute the middle index of the current point subset and partition it (with quickselect)
        // such that the hyperplane divides the point set in two halfs (at the median)
        size_t middle_index = ((stop_index - 1) - start_index) / 2;
        std::nth_element(points.begin() + start_index, points.begin() + start_index + middle_index, points.begin() + stop_index,
                         [axis](const Point<dimension, T> left, const Point<dimension, T> right)
                         {
                             return left.first[axis] < right.first[axis];
                         });
        // Create a new node
        auto node = std::make_unique<Node<dimension, T>>(points[start_index + middle_index], axis, nullptr, nullptr);

        // Construct for both halfs of the points a subtree and make them children of the new node
        auto left = construct(points, start_index, start_index + middle_index + 1, depth + 1);
        node->left_child = std::move(left);
        auto right = construct(points, start_index + middle_index + 1, stop_index, depth + 1);
        node->right_child = std::move(right);

        return node;
    }

    bool pointInRegion(const Region<dimension> &region, const Point<dimension, T> &point) const
    {
        for (size_t i = 0; i < dimension; ++i)
        {
            if (point.first[i] < region[i].first || point.first[i] > region[i].second)
            {
                return false;
            }
        }
        return true;
    }

    std::list<Point<dimension, T>> rangeSearch(const Region<dimension> &bounding_box, const std::unique_ptr<Node<dimension, T>> &root, Region<dimension> root_region) const
    {
        std::list<Point<dimension, T>> points;

        // (1) Check if root is a leaf and if so, should it be reported?
        if (isLeaf(root))
        {
            if (pointInRegion(bounding_box, root->point))
            {
                points.push_back(root->point);
                return points;
            }
            return points;
        }

        // (2) Is the region of the left child of the root completely contained in the search range?
        // if so, report the whole subtree, otherwise perform range search on the left child
        // Hint: how can the region of the left child easily be computed?

        // Interval for the axis of the root
        Region<dimension> left_region = root_region;
        left_region[root->axis].second = root->point.first[root->axis];
        auto containsLeft = containsRegion(left_region, bounding_box);

        if (containsLeft == 2)
        {
            points.splice(points.end(), reportSubtree(root->left_child));
        }
        else if (containsLeft == 1)
        {
            points.splice(points.end(), rangeSearch(bounding_box, root->left_child, root_region));
        }

        // (3) Repeat step (2) for the right subtree.
        Region<dimension> right_region = root_region;
        right_region[root->axis].first = root->point.first[root->axis];
        auto containsRight = containsRegion(right_region, bounding_box);

        if (containsRight == 2)
        {
            points.splice(points.end(), reportSubtree(root->right_child));
        }
        else if (containsRight == 1)
        {
            points.splice(points.end(), rangeSearch(bounding_box, root->right_child, root_region));
        }

        return points;
    }

    // Search for the nearest neighbor of point p in the tree
    Point<dimension, T> nearestNeighbour(Point<dimension, T> p, const std::unique_ptr<Node<dimension, T>> &root) const
    {
        // (1) Report point if root is a leaf
        if (isLeaf(root))
        {
            return root->point;
        }

        // (2) Determine which subtree to search
        size_t axis = root->axis;
        const std::unique_ptr<Node<dimension, T>> *next_branch = nullptr;
        const std::unique_ptr<Node<dimension, T>> *other_branch = nullptr;

        if (p.first[axis] < root->point.first[axis])
        {
            next_branch = &root->left_child;
            other_branch = &root->right_child;
        }
        else
        {
            next_branch = &root->right_child;
            other_branch = &root->left_child;
        }

        // (3) Recursively search in the next branch
        Point<dimension, T> candidate = nearestNeighbour(p, *next_branch);

        // (4) Check if we need to search the other branch
        double distance_to_hyperplane = std::abs(p.first[axis] - root->point.first[axis]);
        if (distance_to_hyperplane < distance(p, candidate))
        {
            Point<dimension, T> possible_new_candidate = nearestNeighbour(p, *other_branch);
            if (distance(p, possible_new_candidate) < distance(p, candidate))
            {
                candidate = possible_new_candidate;
            }
        }

        return candidate;
    }

    std::unique_ptr<Node<dimension, T>> m_root;

public:
    KDTree()
        : m_root{nullptr}
    {
    }

    // Method to construct a balanced kd-tree
    // Calls the recursive method and sets the root to the returned node
    void construct(std::vector<Point<dimension, T>> &points)
    {
        m_root = construct(points, 0, points.size(), 0);
    }

    // Perform a range search for the given bounding box given at the root
    std::list<Point<dimension, T>> rangeSearch(Region<dimension> bounding_box) const
    {
        // The root covers the whole space. So its bounding box is (-inf, +inf)^d
        Region<dimension> root_bounding_box;
        root_bounding_box.fill(std::make_pair(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()));

        return rangeSearch(bounding_box, m_root, root_bounding_box);
    }

    // Searching if a Tree contains a given point can be easily done by performing the range search with the coordinates of the point as lower/upper bounds
    bool containsPoint(Point<dimension, T> p) const
    {
        Region<dimension> bounding_box;
        for (size_t i = 0; i < dimension; ++i)
        {
            bounding_box[i].first = p.first[i];
            bounding_box[i].second = p.first[i];
        }
        return !(rangeSearch(bounding_box).empty());
    }

    // Perform a nearest neighbour search on the whole tree
    Point<dimension, T> nearestNeighbour(Point<dimension, T> p) const
    {
        return nearestNeighbour(p, m_root);
    }
};

enum class IrisSpecies
{
    setosa,
    versicolor,
    virginica
};

std::vector<Point<4, IrisSpecies>> loadIrisDataset(std::string filename)
{
    std::ifstream file;
    file.open(filename);

    std::vector<Point<4, IrisSpecies>> data;
    if (file.good())
    {
        std::string line;
        std::getline(file, line);
        while (std::getline(file, line))
        {
            IrisSpecies species;
            std::string species_str = line.substr(16);
            if (species_str == "setosa")
            {
                species = IrisSpecies::setosa;
            }
            else if (species_str == "versicolor")
            {
                species = IrisSpecies::versicolor;
            }
            else
            {
                species = IrisSpecies::virginica;
            }
            data.push_back(Point<4, IrisSpecies>({std::stod(line.substr(0, 3)), std::stod(line.substr(4, 3)), std::stod(line.substr(8, 3)), std::stod(line.substr(12, 3))}, species));
        }
    }
    return data;
}

int main()
{
    KDTree<2> tree;

    std::vector<Point<2>> points{{{6, 11}, 0}, {{25, 11}, 0}, {{29, 1}, 0}, {{1, 4}, 0}, {{11, 6}, 0}, {{10, 8}, 0}, {{3, 13}, 0}};
    tree.construct(points);

    auto range = tree.rangeSearch(Region<2>{std::make_pair(10., 20.), std::make_pair(5., 10.)});

    assert(range.size() == 2);
    assert(std::find(range.begin(), range.end(), Point<2>({10, 8}, 0)) != range.end());
    assert(std::find(range.begin(), range.end(), Point<2>({42, 21}, 0)) == range.end());

    assert(tree.containsPoint(std::make_pair<std::array<double, 2>, int>({29, 1}, 0)));
    assert(!tree.containsPoint(std::make_pair<std::array<double, 2>, int>({20, 11}, 0)));

    auto nearest = tree.nearestNeighbour(Point<2>({{9, 3}, 0}));
    assert(nearest == Point<2>({11, 6}, 0));

    nearest = tree.nearestNeighbour(Point<2>({{26, 11}, 0}));
    assert(nearest == Point<2>({25, 11}, 0));

    auto train_data = loadIrisDataset("iris_train.csv");
    auto test_data = loadIrisDataset("iris_test.csv");

    KDTree<4, IrisSpecies> iris_tree;
    iris_tree.construct(train_data);

    double accuracy = 0;
    for (size_t i = 0; i < test_data.size(); ++i)
    {
        IrisSpecies predictedLabel = iris_tree.nearestNeighbour(test_data[i]).second;
        if (predictedLabel == test_data[i].second)
        {
            accuracy += 1;
        }
    }
    accuracy /= test_data.size();

    std::cout << "Accuracy: " << accuracy << std::endl;
    assert(accuracy == 1.);

    return 0;
}
