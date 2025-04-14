// enable assertions in release build
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../helper.hpp"
#include <iostream>
#include <cassert>
#include <cstdint>

#define ASSERT_EQUAL(actual, expected) Helper::assertEqual<uint64_t>(actual, expected, __FILE__, __LINE__)

using namespace std;

struct Node
{
  uint64_t data;
  Node *next;
};

/************** begin assignment **************/
// Write a program that takes the head of a singly linked list and
// finds out if the list is a palindrom.
// Example:
//        1 -> 2 -> 3 -> 2 -> 1        (is a palindrom)
//        1 -> 2 -> 3 -> 3 -> 2 -> 1   (is a palindrom)
//        1 -> 2 -> 3 -> 3 -> 3 -> 1   (is not a palindrom)
//
// Here, the head of the list is also a valid node.
// If you reorder elements in the list put them back in correct order
// before returning from the function.
// (Optimal: O(1) space, O(n) time)

// Hints for the optimal solution (not easy!):
// - adapt the runner technique to get to the second half of the list
// - also count how many elements you have in one half
// - reverse the first half of the list in-place (write an additional reverse function)
// - compare if the two sub-lists are equal
// - reverse back the first half in-place
//
// If you don't succeed to implement this assignment with optimal specs,
// that's ok. Use extra memory and you're done with a few lines
// of code ;).

void reverse(Node *&node, int count)
{
  if (node == nullptr || count == 0)
  {
    return;
  }
  Node *prev = nullptr;
  Node *current = node;
  Node *next = nullptr;
  // Idea: save the following list
  //       reverse the nose of the current node
  //       now look from the prespective of previous node, i.e current becomes the new nose
  while (current && count > 0)
  {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
    count--;
  }
  
  node->next = current;
  node = prev;
}

bool is_palindrom(Node *node)
{
  // two pointers, one moves twice as fast as the other
  int count = 0;
  Node *slow = node;
  Node *fast = node;

  // find the middle of the list
  while (fast && fast->next)
  {
    slow = slow->next;
    fast = fast->next->next;
    count++;
  }

  // skip the middle element if the list has odd number of elements
  if (fast)
  {
    slow = slow->next;
  }

  // reverse the first half of the list
  reverse(node, count);

  // compare the two halves
  Node *current = node;
  Node *current2 = slow;
  while (current && current2)
  {
    if (current->data != current2->data)
    {
      // reverse back the first half of the list
      reverse(node, count);
      return false;
    }
    current = current->next;
    current2 = current2->next;
  }

  // reverse back the first half of the list
  reverse(node, count);
  return true;
}
/*************** end assignment ***************/

int main()
{
  // test reverse function
  // empty list
  {
    Node *head = nullptr;
    reverse(head, 0);
    assert(head == nullptr);
  }
  {
    Node *head = new Node{1, nullptr};
    reverse(head, 1);
    ASSERT_EQUAL(head->data, 1);
    // clean up
    delete head;
  }
  {
    Node *head = new Node{1, nullptr};
    Node *node1 = new Node{2, nullptr};
    Node *node2 = new Node{3, nullptr};
    Node *node3 = new Node{4, nullptr};
    Node *node4 = new Node{5, nullptr};
    head->next = node1;
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    reverse(head, 5);
    ASSERT_EQUAL(head->data, 5);
    ASSERT_EQUAL(head->next->data, 4);
    ASSERT_EQUAL(head->next->next->data, 3);
    ASSERT_EQUAL(head->next->next->next->data, 2);
    ASSERT_EQUAL(head->next->next->next->next->data, 1);
    // clean up
    while (head)
    {
      Node *tmp = head;
      head = head->next;
      delete tmp;
    }
  }
  // Test if only half will be reversed
  {
    Node *head = new Node{1, nullptr};
    Node *node1 = new Node{2, nullptr};
    Node *node2 = new Node{3, nullptr};
    Node *node3 = new Node{4, nullptr};
    Node *node4 = new Node{5, nullptr};
    head->next = node1;
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    reverse(head, 3);
    ASSERT_EQUAL(head->data, 3);
    ASSERT_EQUAL(head->next->data, 2);
    ASSERT_EQUAL(head->next->next->data, 1);
    ASSERT_EQUAL(head->next->next->next->data, 4);
    ASSERT_EQUAL(head->next->next->next->next->data, 5);
    // clean up
    while (head)
    {
      Node *tmp = head;
      head = head->next;
      delete tmp;
    }
  }
  {
  }
  // test correctness
  {
    Node *head = nullptr; // empty list is also a palindrom
    assert(is_palindrom(head));
  }
  {
    Node head{1, nullptr};
    assert(is_palindrom(&head));
  }
  {
    Node head{2, nullptr};
    Node node{2, nullptr};
    head.next = &node;
    assert(is_palindrom(&head));
  }
  {
    Node head{1, nullptr};
    Node node{2, nullptr};
    head.next = &node;
    assert(!is_palindrom(&head));
  }
  {
    Node head{1, nullptr};
    Node node1{2, nullptr};
    Node node2{1, nullptr};
    head.next = &node1;
    node1.next = &node2;
    assert(is_palindrom(&head));
  }
  {
    Node head{1, nullptr};
    Node node1{2, nullptr};
    Node node2{3, nullptr};
    head.next = &node1;
    node1.next = &node2;
    assert(!is_palindrom(&head));
  }
  {
    Node head{1, nullptr};
    Node node1{2, nullptr};
    Node node2{2, nullptr};
    Node node3{1, nullptr};
    head.next = &node1;
    node1.next = &node2;
    node2.next = &node3;
    assert(is_palindrom(&head));
  }
  {
    Node head{1, nullptr};
    Node node1{2, nullptr};
    Node node2{2, nullptr};
    Node node3{0, nullptr};
    head.next = &node1;
    node1.next = &node2;
    node2.next = &node3;
    assert(!is_palindrom(&head));
  }
  {
    Node head{1, nullptr};
    Node node1{2, nullptr};
    Node node2{3, nullptr};
    Node node3{2, nullptr};
    Node node4{1, nullptr};
    head.next = &node1;
    node1.next = &node2;
    node2.next = &node3;
    node3.next = &node4;
    assert(is_palindrom(&head));
  }
  {
    Node head{1, nullptr};
    Node node1{2, nullptr};
    Node node2{3, nullptr};
    Node node3{3, nullptr};
    Node node4{2, nullptr};
    Node node5{1, nullptr};
    head.next = &node1;
    node1.next = &node2;
    node2.next = &node3;
    node3.next = &node4;
    node4.next = &node5;
    assert(is_palindrom(&head));
  }
  {
    // create a list
    Node *head = new Node{1, nullptr};
    Node *current = head;
    for (uint64_t i = 2; i <= 10; ++i)
    {
      current->next = new Node{i, nullptr};
      current = current->next;
    }
    for (uint64_t i = 10; i > 0; --i)
    {
      current->next = new Node{i, nullptr};
      current = current->next;
    }
    assert(is_palindrom(head));
    assert(is_palindrom(head)); // list should be the same, test again
    head->next->next->next->data = 7;
    assert(!is_palindrom(head));
    head->next->next->next->data = 4;
    assert(is_palindrom(head));
    // delete list
    current = head;
    while (current)
    {
      Node *tmp = current;
      current = current->next;
      delete tmp;
    }
  }
  {
    // create a list
    Node *head = new Node{1, nullptr};
    Node *current = head;
    for (uint64_t i = 2; i <= 10; ++i)
    {
      current->next = new Node{i, nullptr};
      current = current->next;
    }
    current->next = new Node{11, nullptr};
    current = current->next;
    for (uint64_t i = 10; i > 0; --i)
    {
      current->next = new Node{i, nullptr};
      current = current->next;
    }
    assert(is_palindrom(head));
    assert(is_palindrom(head)); // list should be the same, test again
    head->next->next->next->data = 7;
    assert(!is_palindrom(head));
    head->next->next->next->data = 4;
    assert(is_palindrom(head));
    // delete list
    current = head;
    while (current)
    {
      Node *tmp = current;
      current = current->next;
      delete tmp;
    }
  }
  cout << "all tests passed" << endl;
}
