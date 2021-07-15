/// @file: std_gtest.h
/// @auth: 
/// @date: 2021-07-15

/// @auth:
/// @date: 2021-07-15

#pragma once

#include <gtest/gtest.h>

#include <algorithm>        // std::sort
#include <chrono>           // std::chrono::str
#include <cmath>
#include <fstream>
#include <iostream>         // std::cout
#include <list>             // std::list
#include <map>              // std::unordered_map
#include <memory>           // shared and unique pointers
#include <queue>            // std::priority_queue
#include <random>
#include <set>
#include <stack>
// #include <stdlib.h>         /* div, div_t */
// #include <stdio.h>  /* defines FILENAME_MAX */
#include <string>           // std::thread, std::this_thread::sleep_for
#include <thread>           // std::thread, std::this_thread::sleep_for
#include <unistd.h>
#include <unordered_map>    // std::unordered_map
#include <unordered_set>
#include <vector>

using namespace std::chrono;
typedef std::chrono::high_resolution_clock hr_clock;

using std::cerr;
using std::cout;
using std::endl;
using std::fixed;
using std::string;


/// Definition for singly-linked list.
struct ListNode {
    int      val;
    ListNode *next;
    ListNode() : val(0), next(nullptr)
    {
    }
    ListNode(int x) : val(x), next(nullptr)
    {
    }
    ListNode(int x, ListNode * next) : val(x), next(next)
    {
    }
};

/// Utility function to print the ListNode linked list
inline void print_list(const ListNode *head, const string& label = "", const int max_size = 10)
{
    printf("%s[ ", label.c_str());
    const ListNode *node = head;
    int            size  = 0;

    while (node) {
        printf("%d ", node->val);
        node  = node->next;
        size += 1;
        if (size >= max_size) {
            printf("...");
            break;
        }
    }
    printf("]\n");
}

/// Definition for a binary tree node.
struct TreeNode {
    int      val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr)
    {
    }
    TreeNode(int x) : val(x), left(nullptr), right(nullptr)
    {
    }
    TreeNode(int x, TreeNode * left, TreeNode * right)
        : val(x), left(left), right(right)
    {
    }
};


/// Determines if two values are 'close enough' based on a scaled tolerance
template < typename T >
bool isclose(T a, T b,
             T rel_epsilon = std::numeric_limits < T > ::epsilon(),
             typename std::enable_if < std::is_floating_point < T > ::value,
             T > ::type * = 0
             ) {
    T value_range[] = { T(1.0), a, b };

    return(std::abs(a - b) <= rel_epsilon * *std::max_element(value_range, value_range + 3));
}

/// TODO: change to use iterators so can handle more than just Eigen types.
/// NOTE: Container of type ContainerT should contain values of type ValueT.
/// template<typename ContainerT, typename ValueT>
/// bool VectorEpsEq(ContainerT a, ContainerT b, ValueT relTol =
///         std::numeric_limits<ValueT>::epsilon()) {
template < typename T >
bool isclose_vec(T a, T b,
                 double relTol = std::numeric_limits < double > ::epsilon())
{
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); i++) {
        if (!isclose(double(a(i)), double(b(i)), relTol)) {
            return false;
        }
    }
    return true;
}

template < typename T >
T median_odd(std::vector < T > vec)
{
    const auto median_it = vec.begin() + vec.size() / 2;

    std::nth_element(vec.begin(), median_it, vec.end());
    return *median_it;
}

/**
 * [median_even description]
 * @param  vec [description]
 * @return     [description]
*/
template < typename T >
T median_even(std::vector < T > vec)
{
    const auto median_it1 = vec.begin() + int(vec.size() / 2) - 1;
    const auto median_it2 = vec.begin() + int(vec.size() / 2);

    assert(median_it1 == median_it2 - 1);
    return (*median_it1 + *median_it2) / 2;
}

/**
 * [compile  description]
 * @param  [name] [description]
 * @return        [description]
*/
 * (float, double, long double) or it will not compile (as in: `error: no matching function`).
*/
template < typename T >
T median(std::vector < T > vec,
         typename std::enable_if < std::is_floating_point < T > ::value,
         T > ::type * = 0)
{
    if (vec.empty()) {
        return std::numeric_limits < T > ::signaling_NaN();
    }
    return (vec.size() % 2) ? median_odd(vec) : median_even(vec);
}
