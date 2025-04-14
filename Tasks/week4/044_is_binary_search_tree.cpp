// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <iostream>
#include <climits>

using namespace std;

struct Binary_Tree_Node
{
  int data;
  Binary_Tree_Node *left;
  Binary_Tree_Node *right;
};

/************** begin assignment **************/
// Implement a function to check if a binary tree is a binary search tree (BST).
// A binary search tree satisfies the condition:
// left->data <= current->data < right->data.
// More precisely, the condition is that all left nodes must be less than or
// equal to the current node, which must be less than all the right nodes.
//
// Let's state again clearly the properties for a binary search tree:
// - The left subtree of a node contains only nodes with keys lesser than the
//   node's key.
// - The right subtree of a node contains only nodes with keys greater than the
//   node's key.
// - The left and right subtree each must also be a binary search tree.
//
// Be careful with duplicate values:
//    20                  20
//   /    valid BST         \    invalid BST
// 20                       20
//
// Optimal: O(n) time, if binary tree is balanced O(log n) space,
// worst case O(n) space
// --> It's challenging to do it optimal although there is not much code ...
// Try to solve it somehow.
//
// Before you start coding the function "is_binary_search_tree", write test
// cases in the main function to test the "is_binary_search_tree" function.
// Start with simple tests (like nullptr, one node, two nodes, three nodes)
// before writing more complicated tests.
// The style of writing tests before actual coding is called "Test-driven
// development" (TDD). You have indirectly familiarized yourself with it in the
// first exercises of this course, now you can try TDD yourself.
// (If you need an idea how to start writing tests, take a look at the main
// function in "is_symmetric.cpp".)

// write a helper function if you need to
// TODO: write code here

bool is_bst_helper(const Binary_Tree_Node *node, int min, int max)
{
  if (node == nullptr)
  {
    return true;
  }
  if (node->data <= min || node->data > max)
  {
    return false;
  }
  return is_bst_helper(node->left, min, node->data) && is_bst_helper(node->right, node->data, max);
}

bool is_binary_search_tree(const Binary_Tree_Node *node)
{
  return is_bst_helper(node, INT_MIN, INT_MAX);
}

int main()
{
  // TODO: Thoroughly test function "is_binary_search_tree"
  // (include also duplicate values in the tests).
  // Test 1: nullptr
  {
    Binary_Tree_Node *root = nullptr;
    assert(is_binary_search_tree(root) == true);
  }
  // Test 2: one node
  {
    Binary_Tree_Node root{1, nullptr, nullptr};
    assert(is_binary_search_tree(&root) == true);
  }
  // Test 3: two nodes
  {
    Binary_Tree_Node root{1, nullptr, nullptr};
    Binary_Tree_Node left{2, nullptr, nullptr};
    root.left = &left;
    assert(is_binary_search_tree(&root) == false);
  }
  {
    Binary_Tree_Node root{1, nullptr, nullptr};
    Binary_Tree_Node left{2, nullptr, nullptr};
    root.right = &left;
    assert(is_binary_search_tree(&root) == true);
  }
  // Test 4: three nodes
  {
    Binary_Tree_Node root{1, nullptr, nullptr};
    Binary_Tree_Node left{2, nullptr, nullptr};
    Binary_Tree_Node right{3, nullptr, nullptr};
    root.left = &left;
    root.right = &right;
    assert(is_binary_search_tree(&root) == false);
  }
  {
    Binary_Tree_Node root{2, nullptr, nullptr};
    Binary_Tree_Node left{1, nullptr, nullptr};
    Binary_Tree_Node right{3, nullptr, nullptr};
    root.left = &left;
    root.right = &right;
    assert(is_binary_search_tree(&root) == true);
  }
  {
    Binary_Tree_Node root{2, nullptr, nullptr};
    Binary_Tree_Node left{1, nullptr, nullptr};
    Binary_Tree_Node right{2, nullptr, nullptr};
    root.left = &left;
    root.right = &right;
    assert(is_binary_search_tree(&root) == false);
  }
  {
    Binary_Tree_Node root{2, nullptr, nullptr};
    Binary_Tree_Node left{2, nullptr, nullptr};
    Binary_Tree_Node right{3, nullptr, nullptr};
    root.left = &left;
    root.right = &right;
    assert(is_binary_search_tree(&root) == true);
  }
  // Complex Trees
  {
    Binary_Tree_Node root{10, nullptr, nullptr};
    Binary_Tree_Node left{5, nullptr, nullptr};
    Binary_Tree_Node left_left{3, nullptr, nullptr};
    Binary_Tree_Node left_right{7, nullptr, nullptr};
    Binary_Tree_Node left_left_left{2, nullptr, nullptr};
    Binary_Tree_Node left_left_left_left{1, nullptr, nullptr};

    root.left = &left;
    left.left = &left_left;
    left.right = &left_right;
    left_left.left = &left_left_left;
    left_left_left.left = &left_left_left_left;

    assert(is_binary_search_tree(&root) == true);
  }
  {
    Binary_Tree_Node root{10, nullptr, nullptr};
    Binary_Tree_Node left{5, nullptr, nullptr};
    Binary_Tree_Node left_left{3, nullptr, nullptr};
    Binary_Tree_Node left_right{7, nullptr, nullptr};
    Binary_Tree_Node left_left_left{2, nullptr, nullptr};
    Binary_Tree_Node left_left_left_left{1, nullptr, nullptr};
    Binary_Tree_Node left_left_left_right{4, nullptr, nullptr}; // 4 is greater than 3

    root.left = &left;
    left.left = &left_left;
    left.right = &left_right;
    left_left.left = &left_left_left;
    left_left_left.left = &left_left_left_left;
    left_left_left.right = &left_left_left_right;

    assert(is_binary_search_tree(&root) == false);
  }

  std::cout << "All tests passed!" << std::endl;
}
/*************** end assignment ***************/
